#define R32_RCC_APB2PCENR   (*(volatile unsigned int*)0x40021018)
#define AFIOEN  0b1         // PD port module clock enable bit for I/O
#define IOPDEN  0b1         // I/O auxiliary function module clock enable bit

#define R32_GPIOD_CFGLR     (*(volatile unsigned int*)0x40011400)
#define MODE    0b01        // Output mode, maximum speed 10MHz

#define R32_GPIOD_OUTDR     (*(volatile unsigned int*)0x4001140C)
#define PullUp      0b1     // Pull-up input
#define PullDown    0b0     // Pull-down input

int main(void) {
    // PB2 Peripheral Clock Enable Register
    R32_RCC_APB2PCENR |= ((IOPDEN << 5) | AFIOEN);
    
    // Port Configuration Register Lo
    R32_GPIOD_CFGLR  = (R32_GPIOD_CFGLR & ~(0xFFF << 8)) 
        | (MODE << 16)  // PD4
        | (MODE << 12)  // PD3
        | (MODE << 8);  // PD2
    
    // Port Output Register
    R32_GPIOD_OUTDR = 0x1C; // PD2-4 PullDown.

    while(1){
        // Port Output Register
        R32_GPIOD_OUTDR = (PullUp << 4);
        // Примерно 2с
        for(unsigned int i = 0; i < 4800000; i++){
            asm volatile("nop");
        }

        R32_GPIOD_OUTDR |= (PullUp << 3);
        // Примерно 0.25с
        for(unsigned int i = 0; i < 2000000; i++){
            asm volatile("nop");
        }

        R32_GPIOD_OUTDR = 0x4; // PD4 and PD3 PullDown. PD2 PullUp

        // Port Output Register
        for(unsigned int i = 0; i < 2400000; i++){
            asm volatile("nop");
        }
    }
}