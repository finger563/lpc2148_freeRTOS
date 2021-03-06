/*******************************************************************************
*                                                                              *
* LINKER SCRIPT                                                                *
* LPC2148 - SFE USB BOOTLOADER - APPLICATION                                   *
*                                                                              *
* Heavily influenced by Rowley's Crosswork                                     *
* Visit www.rowley.co.uk - They are good guys                                  *
*                                                                              *
* Used with Rowley's blessing                                                  *
*                                                                              *
*******************************************************************************/


/******************************************************************************/
MEMORY
/******************************************************************************/
{
  /* LPC2148 FOR THE SFE USB BOOTLOADER            */
  /* REGIONS AS DEFINED IN UM10139 REV 01 FIG. 2   */
  FLASH_BOOT      (RX) : ORIGIN = 0x00000000, LENGTH = 64k
  FLASH_APP       (RX) : ORIGIN = 0x00010000, LENGTH = 448k
  SRAM            (WX) : ORIGIN = 0x40000000, LENGTH = 32k
  USB_DMA_RAM     (WX) : ORIGIN = 0x7FD00000, LENGTH = 8k
} /* MEMORY */


/******************************************************************************/
SECTIONS
/******************************************************************************/
{
  /* Absolute section starts and end */
  /* WARNING: INSURE THESE ARE ALWAYS CONSISTENT WITH THE MEMORY DECLARATION */
  __FLASH_segment_start__				= 0x00010000;
  __SRAM_segment_start__				= 0x40000000;

  /* Stack/Heap declarations */
  __STACKSIZE_SYS__						= 0;
  __STACKSIZE_USER__						= 0;
  __STACKSIZE_IRQ__						= 1024;
  __STACKSIZE_FIQ__						= 512;
  __STACKSIZE_SVC__						= 2*1024;
  __STACKSIZE_ABT__						= 0;
  __STACKSIZE_UND__						= 0;
  __HEAPSIZE__							   = 0;
  
  __EXCEPTION_VECTORS_SIZE__        = 60;

  /****************************************************************************/
  /* RAM SECTION                                                             */
  /****************************************************************************/
  __lpc2000ExceptionVectors_ram_load_start__ = __SRAM_segment_start__;
  .lpc2000ExceptionVectors_ram __SRAM_segment_start__ (NOLOAD) :
  {
    __lpc2000ExceptionVectors_ram_start__ = .;
    *(.lpc2000ExceptionVectors_ram)
    . = MAX(__lpc2000ExceptionVectors_ram_start__ + __EXCEPTION_VECTORS_SIZE__ , .);
  }
  __lpc2000ExceptionVectors_ram_end__ = __lpc2000ExceptionVectors_ram_start__ + SIZEOF(.lpc2000ExceptionVectors_ram);
  /****************************************************************************/
  /* RAM SECTIONS                                                             */
  /****************************************************************************/

  /****************************************************************************/
  /* FLASH SECTIONS                                                           */
  /****************************************************************************/
  __lpc2000ExceptionVectors_load_start__ = __FLASH_segment_start__;
  .lpc2000ExceptionVectors __FLASH_segment_start__ :
  {
    __lpc2000ExceptionVectors_start__ = .;
    *(.lpc2000ExceptionVectors .lpc2000ExceptionVectors.*)
  }
  __lpc2000ExceptionVectors_end__ = __lpc2000ExceptionVectors_start__ + SIZEOF(.lpc2000ExceptionVectors);

  __init_start__ = ALIGN(__lpc2000ExceptionVectors_end__ , 4);
  .init ALIGN(__lpc2000ExceptionVectors_end__ , 4) :
  {
    __init_start__ = .;
    *(.init .init.*)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);

  __text_load_start__ = ALIGN(__init_end__ , 4);
  .text ALIGN(__init_end__ , 4) :
  {
    __text_start__ = .;
    *(.text .text.* .glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) :
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) :
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);

  __rodata_load_start__ = ALIGN(__ctors_end__ , 4);
  .rodata ALIGN(__ctors_end__ , 4) :
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);

  __symbol_table_start__ = ALIGN(__rodata_end__ , 4);
  .symbol_table ALIGN(__rodata_end__ , 4) :
  {
    __symbol_table_start__ = .;
    *(.symbol_table)
  }
  __symbol_table_end__ = __symbol_table_start__ + SIZEOF(.symbol_table);
  
  __fast_load_start__ = ALIGN(__symbol_table_end__ , 4);
  .fast __lpc2000ExceptionVectors_ram_end__ : AT(ALIGN(__symbol_table_end__ , 4))
  {
    __fast_start__ = .;
    *(.fast .fast.*)
  }
  __fast_end__ = __fast_start__ + SIZEOF(.fast);

  __fast_load_end__ = __fast_load_start__ + SIZEOF(.fast);

  /****************************************************************************/
  /* END OF FLASH SECTIONS                                                    */
  /****************************************************************************/
  
  /****************************************************************************/
  /* START OF RAM SECTIONS                                                    */
  /****************************************************************************/
  
  .fast_run ALIGN(__lpc2000ExceptionVectors_ram_end__ , 4) (NOLOAD) :
  {
    __fast_run_start__ = .;
    . = MAX(__fast_run_start__ + SIZEOF(.fast), .);
  }
  __fast_run_end__ = __fast_run_start__ + SIZEOF(.fast_run);

  __data_load_start__ = ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4);
  .data __fast_run_end__ : AT(ALIGN(__fast_load_start__ + SIZEOF(.fast) , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);

  __data_load_end__ = __data_load_start__ + SIZEOF(.data);

  .data_run ALIGN(__fast_run_end__ , 4) (NOLOAD) :
  {
    __data_run_start__ = .;
    . = MAX(__data_run_start__ + SIZEOF(.data), .);
  }
  __data_run_end__ = __data_run_start__ + SIZEOF(.data_run);

  __bss_load_start__ = ALIGN(__data_run_end__ , 4);
  .bss ALIGN(__data_run_end__ , 4) (NOLOAD) :
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);

  __non_init_load_start__ = ALIGN(__bss_end__ , 4);
  .non_init ALIGN(__bss_end__ , 4) (NOLOAD) :
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);

  __heap_load_start__ = ALIGN(__non_init_end__ , 4);
  .heap ALIGN(__non_init_end__ , 4) (NOLOAD) :
  {
    __heap_start__ = .;
    *(.heap)
    . = MAX(__heap_start__ + __HEAPSIZE__ , .);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);

  __stack_user_load_start__ = ALIGN(__heap_end__ , 4);
  .stack ALIGN(__heap_end__ , 4) (NOLOAD) :
  {
    __stack_user_start__ = .;
    *(.stack)
    . = MAX(__stack_user_start__ + __STACKSIZE_USER__ , .);
  }
  __stack_user_end__ = __stack_user_start__ + SIZEOF(.stack);

  __stack_irq_load_start__ = ALIGN(__stack_user_end__ , 4);
  .stack_irq ALIGN(__stack_user_end__ , 4) (NOLOAD) :
  {
    __stack_irq_start__ = .;
    *(.stack_irq)
    . = MAX(__stack_irq_start__ + __STACKSIZE_IRQ__ , .);
  }
  __stack_irq_end__ = __stack_irq_start__ + SIZEOF(.stack_irq);

  __stack_fiq_load_start__ = ALIGN(__stack_irq_end__ , 4);
  .stack_fiq ALIGN(__stack_irq_end__ , 4) (NOLOAD) :
  {
    __stack_fiq_start__ = .;
    *(.stack_fiq)
    . = MAX(__stack_fiq_start__ + __STACKSIZE_FIQ__ , .);
  }
  __stack_fiq_end__ = __stack_fiq_start__ + SIZEOF(.stack_fiq);

  __stack_svc_load_start__ = ALIGN(__stack_fiq_end__ , 4);
  .stack_svc ALIGN(__stack_fiq_end__ , 4) (NOLOAD) :
  {
    __stack_svc_start__ = .;
    *(.stack_svc)
    . = MAX(__stack_svc_start__ + __STACKSIZE_SVC__ , .);
  }
  __stack_svc_end__ = __stack_svc_start__ + SIZEOF(.stack_svc);

  __stack_abt_load_start__ = ALIGN(__stack_svc_end__ , 4);
  .stack_abt ALIGN(__stack_svc_end__ , 4) (NOLOAD) :
  {
    __stack_abt_start__ = .;
    *(.stack_abt)
    . = MAX(__stack_abt_start__ + __STACKSIZE_ABT__ , .);
  }
  __stack_abt_end__ = __stack_abt_start__ + SIZEOF(.stack_abt);

  __stack_und_load_start__ = ALIGN(__stack_abt_end__ , 4);
  .stack_und ALIGN(__stack_abt_end__ , 4) (NOLOAD) :
  {
    __stack_und_start__ = .;
    *(.stack_und)
    . = MAX(__stack_und_start__ + __STACKSIZE_UND__ , .);
  }
  __stack_und_end__ = __stack_und_start__ + SIZEOF(.stack_und);
} /* SECTIONS */
