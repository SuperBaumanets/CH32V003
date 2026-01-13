.equ R32_RCC_APB2PCENR,     0x40021018
.equ R32_RCC_APB2PCENR_IOPDEN,   (1 << 5)
.equ R32_RCC_APB2PCENR_AFIOEN,   (1)

.equ R32_GPIOD_CFGLR,       0x40011400
.equ MODE,  0b01

.equ R32_GPIOD_OUTDR,    0x4001140C

.equ PULLUP,     0b1
.equ PULLDOWN,   0b0

.global _start
.text
_start: 
    // PB2 Peripheral Clock Enable Register
    li      t0, R32_RCC_APB2PCENR

    lw      t1, 0(t0)
    ori     t1, t1, R32_RCC_APB2PCENR_IOPDEN
    ori     t1, t1, R32_RCC_APB2PCENR_AFIOEN

    sw      t1, 0(t0)

    // Port Configuration Register Lo
    li      t0, R32_GPIOD_CFGLR
    
    lw      t1, 0(t0)
    li      t2, ~(0xFFF << 8)
    and     t1, t1, t2
    li      t2, (0b01 << 16)    // PD4
    or      t1, t1, t2
    li      t2, (0b01 << 12)    // PD3
    or      t1, t1, t2
    li      t2, (0b01 << 8)     // PD2
    or      t1, t1, t2

    sw      t1, 0(t0)

    // Port Output Register
    li      t0, R32_GPIOD_OUTDR
    li      t1, 0x1C    // PD2-4 PullDown
    sw      t1, 0(t0)

main_loop:
    // Port Output Register
    // PD4
    li      t0, R32_GPIOD_OUTDR
    li      t1, 0x10
    sw      t1, 0(t0)

    li      t0, 4800000

sleep_PD4: 
    addi    t0, t0, -1
    bnez    t0, sleep_PD4

    // Port Output Register
    // PD3
    li      t0, R32_GPIOD_OUTDR
    lw      t1, 0(t0)
    ori     t1, t1, (PULLUP << 3)
    sw      t1, 0(t0)

    li      t0, 2000000

sleep_PD3: 
    addi    t0, t0, -1
    bnez    t0, sleep_PD3

    // Port Output Register
    // PD2, PD3, PD4
    li      t0, R32_GPIOD_OUTDR
    li      t1, 0x4
    sw      t1, 0(t0)

    li      t0, 2400000

sleep: 
    addi    t0, t0, -1
    bnez    t0, sleep


    j main_loop
