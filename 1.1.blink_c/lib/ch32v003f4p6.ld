MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 16K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{    
    .text : {
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } > FLASH
    
    .data : AT(ADDR(.text) + SIZEOF(.text)){
    _data_start = .;
    *(.data*)
    . = ALIGN(4);
    _data_end = .;
  } > RAM
    
   .bss : {
    _bss_start = .;
    *(.bss*)
    . = ALIGN(4);
    _bss_end = .;
  } > RAM
}

PROVIDE(_stack_end = ORIGIN(RAM) + LENGTH(RAM));
PROVIDE(_data_load = LOADADDR(.data));