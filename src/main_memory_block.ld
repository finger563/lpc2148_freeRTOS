/***********************************************************************/
/*  ROM.ld:  Linker Script File                                        */
/***********************************************************************/

/*
   Based on scripts found in the LPC yahoo-group and the Keil examples
   modified by Martin Thomas, Kaiserslautern, Germany
   http://www.siwawi.arubi.uni-kl.de/avr_projects
*/

/*
ENTRY(_boot)
STACK_SIZE = 0x800; -> see startup-file and .stack-section
*/

/* Memory Definitions */
MEMORY
{
    ROM (rx) : ORIGIN = 0x00000000, LENGTH = 512k
    RAM (rw) : ORIGIN = 0x40000000, LENGTH = 32k
}

/* Section Definitions */
SECTIONS
{
    __HEAPSIZE__	= 1*1024;

    /* first section is .text which is used for code */
    .text :
    {
        KEEP(*(.vectorg))
        . = ALIGN(4);
        KEEP(*(.init))             /* Startup code from .init-section */
        *(.text .text.*)           /* remaining code */
        *(.gnu.linkonce.t.*)
        *(.glue_7)
        *(.glue_7t)
        *(.gcc_except_table)
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
    }
    > ROM

    . = ALIGN(4);

    /* .ctors .dtors are used for c++ constructors/destructors */
    /* added by Martin Thomas 4/2005 based on Anglia Design example */
    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
        . = ALIGN(4);
        PROVIDE(__ctors_end__ = .);
        . = ALIGN(4);
    }
    >ROM

    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        . = ALIGN(4);
        PROVIDE(__dtors_end__ = .);
        . = ALIGN(4);
    }
    >ROM

    . = ALIGN(4);
    /* mthomas - end */

    _etext = . ;
    PROVIDE (etext = .);

    /* .data section which is used for initialized data */
    .data : AT (_etext)
    {
        _data = .;
        KEEP(*(.vectmapped)) /* mthomas: when code in ROM and vectors remapped */
        . = ALIGN(4);
        *(.fastrun) /* mthomas: for "RAMFUNC" */
        . = ALIGN(4);
        SORT(CONSTRUCTORS) /* mt 4/2005 */
        . = ALIGN(4);
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d*)
        . = ALIGN(4);
    }
    > RAM

    . = ALIGN(4);
    _edata = . ;
    PROVIDE (edata = .);

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        __bss_start = . ;
        __bss_start__ = . ;
        *(.bss)
        *(.gnu.linkonce.b*)
        *(COMMON)
        . = ALIGN(4);
    }
    > RAM

    . = ALIGN(4);
    __bss_end__ = . ;
    PROVIDE (__bss_end = .);

    __heap_load_start__ = ALIGN(__bss_end , 4);
    .heap ALIGN(__bss_end , 4) (NOLOAD) :
    {
      __heap_start__ = .;
      *(.heap)
      . = MAX(__heap_start__ + __HEAPSIZE__ , .);
    } > RAM
    __heap_end__ = . ;


    .stack ALIGN(256) :
    {
        *(.stack)
        PROVIDE (_stack = .);
        . = ALIGN(4);
    }
    > RAM

    __exidx_start = .;
    .ARM.exidx :
    {
	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    }
    > RAM
    __exidx_end = .;

    _end = . ;
    PROVIDE (end = .);
}
