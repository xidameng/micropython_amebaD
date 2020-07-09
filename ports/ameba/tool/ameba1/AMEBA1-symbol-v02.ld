

ENTRY(Reset_Handler)

INCLUDE "export-rom_v02.txt"

MEMORY
{
  ROM_USED_RAM (rwx)  : ORIGIN = 0x10000bc8, LENGTH = 0x5438
  RAM (rwx)  : ORIGIN = 0x10006000, LENGTH = 0x6A000
  TCM (rwx) : ORIGIN = 0x1FFF0000, LENGTH = 0x10000
  SDRAM (rwx)  : ORIGIN = 0x30000000, LENGTH = 2M
}

SECTIONS
{
  __rom_bss_start__ = 0x10000300;
  __rom_bss_end__ = 0x10000bc8;
  
  .bootloader :
  {
    KEEP(*(.loader.data*))
  } > ROM_USED_RAM
  
  OVERLAY 0x1FFF0000:
  {
  	.valid
  	{ 
      *mem.o (.bss*)
	    *memp.o (.bss*)
      *(.tcm.heap)
  	}
  	.dummy 
  	{ 
      __ram_image1_text_start__ = .;
      __ram_start_table_start__ = .;
      KEEP(*(SORT(.start.ram.data*)))
      __ram_start_table_end__ = .;
        __image1_validate_code__ = .;
      KEEP(*(.image1.validate.rodata*))
      KEEP(*(.infra.ram.data*))
      KEEP(*(.timer.ram.data*))
      KEEP(*(.cutb.ram.data*))
      KEEP(*(.cutc.ram.data*))
      KEEP(*(.hal.ram.data*))
      __image1_bss_start__ = .;
      .ram_image1.bss$$Base = .;
      __image1_bss_end__ = .;
      .ram_image1.bss$$Limit = .;
      __ram_image1_data_end__ = .;
      
      *(.hal.ram.text*)    
      *(.infra.ram.text*)
  	}
  } > TCM

  .sdram.text :
  {
    __sdram_data_start__ = .;
    *(.sdram.text*)
    *(.p2p.text*)
    *(.wps.text*)
    *(.websocket.text*)
    */py/*.o(.text*)
    */lib/*.o(.text*)
    */lib/oofatfs*.o(.text*)
    */extmod/*.o(.text*)
  } > SDRAM

  .sdram.rodata :
  {
    *(.sdram.rodata*)
    *(.p2p.rodata*)
    *(.wps.rodata*)
    *(.websocket.rodata*)
    */py/*.o(.rodata*)
    */lib/*.o(.rodata*)
    */lib/oofatfs*.o(.text*)
    */extmod/*.o(.rodata*)
  } > SDRAM

  .sdram.data : 
  {
    *(.sdram.data*)
    *(.p2p.data*)
    *(.wps.data*)
    *(.websocket.data*)
    */py/*.o(.data*)
    */lib/*.o(.data*)
    */lib/oofatfs*.o(.text*)
    */extmod/*.o(.data*)
    __sdram_data_end__ = .;
  } > SDRAM

  .sdram.bss :
  {
    __sdram_bss_start__ = .;
    *(.sdram.bss*)
    *(.p2p.bss*)
    *(.wps.bss*)
    *(.websocket.bss*)
    */py/*.o(.bss*)
    */lib/*.o(.bss*)
    */lib/oofatfs*.o(.text*)
    */extmod/*.o(.bss*)
    __sdram_bss_end__ = .;
  } > SDRAM

 .bf_data : 
  {
    __buffer_data_start__ = .;
    *(.bfsram.data*)
    __buffer_data_end__ = .;
    
  } > SDRAM

  .ram.start.table :
  {
    __ram_image2_text_start__ = .;
    __image2_entry_func__ = .;
    .ram.start.table1$$Base = .;
    KEEP(*(SORT(.image2.ram.data*)))
    __image2_validate_code__ = .;
    KEEP(*(.image2.validate.rodata*))
    KEEP(*(.custom.validate.rodata*))
  } > RAM

  .ram.text :
  {
    KEEP(*(.infra.ram.start*))
    *(.mon.ram.text*)
    *(.hal.flash.text*)    
    *(.hal.sdrc.text*)
    *(.hal.gpio.text*)
    *(.text*)    
  } > RAM

  .ram.rodata :
  {
    *(.rodata*)
  } > RAM

  .ram.data : 
  {
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
  	__ram_image2_text_end__ = .;    
  } > RAM
  
  .ram.bss :
  {
    __bss_start__ = .;
    .ram.bss$$Base = .;
    *(.hal.flash.data*)    
    *(.hal.sdrc.data*)    
    *(.hal.gpio.data*)
    *(.bss*)
    *(COMMON)
    *(.bdsram.data*)
    __bss_end__ = .;
    .ram.bss$$Limit = .;
  } > RAM

  .heap :
  {
     __end__ = .;
     end = __end__;
     *(.heap*)
     __HeapLimit = .;
  } > RAM

  /* .stack_dummy section doesn't contains any symbols. It is only
   * used for linker to calculate size of stack sections, and assign
   * values to stack symbols later */
  .stack_dummy :
  {
  	*(.stack)
  } > RAM
  
  /* Set stack top to end of RAM, and stack limit move down by
   * size of stack_dummy section */
  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  __StackLimit = __StackTop - SIZEOF(.stack_dummy);
  PROVIDE(__stack = __StackTop);
  
  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
