#define R32_RCC_APB2PCENR   (*(volatile unsigned int*)0x40021018)
#define AFIOEN  0b1     // PD port module clock enable bit for I/O
#define IOPDEN  0b1     // I/O auxiliary function module clock enable bit

#define R32_GPIOD_CFGLR     (*(volatile unsigned int*)0x40011400)
#define CNF4    0b00    // Universal push-pull output mode
#define MODE4   0b01    // Output mode, maximum speed 10MHz

#define R32_GPIOD_OUTDR     (*(volatile unsigned int*)0x4001140C)
#define PullUp      0b1     // Pull-up input
#define PullDown    0b0     // Pull-down input

int main(void) {
    // PB2 Peripheral Clock Enable Register
    R32_RCC_APB2PCENR |= ((IOPDEN << 5) | AFIOEN);
    
    // Port Configuration Register Lo
    R32_GPIOD_CFGLR  = (R32_GPIOD_CFGLR & ~(0xF << 16)) | (MODE4 << 16);
    
    // Port Output Register
    R32_GPIOD_OUTDR |= (PullDown << 4);

    while(1){
        // Port Output Register
        R32_GPIOD_OUTDR = (PullUp << 4);

        for(unsigned int i = 0; i < 500000; i++){
            asm volatile("nop");
        }

        // Port Output Register
        R32_GPIOD_OUTDR = (PullDown << 4);

        for(unsigned int i = 0; i < 500000; i++){
            asm volatile("nop");
        }
    }
}