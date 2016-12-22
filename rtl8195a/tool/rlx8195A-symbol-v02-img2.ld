

ENTRY(Reset_Handler)

INCLUDE "export-rom_v02.txt"

MEMORY
{
  FLASH (rwx) : ORIGIN = 0x00000000, LENGTH = 0x200000
  ROM_USED_RAM (rwx)  : ORIGIN = 0x10000bc8, LENGTH = 0x5438
  BD_RAM (rwx)  : ORIGIN = 0x10006000, LENGTH = 0x6A000
  TCM (rwx) : ORIGIN = 0x1FFF0000, LENGTH = 0x10000
  SDRAM_RAM (rwx)  : ORIGIN = 0x30000000, LENGTH = 2M
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
          /**mem.o (.bss*)*/
          /**memp.o (.bss*)*/
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

    .image2.start.table :
    {
      __ram_image2_text_start__ = .;
      __image2_entry_func__ = .;
      .image2.start.table1$$Base = .;
      KEEP(*(SORT(.image2.ram.data*)))
      __image2_validate_code__ = .;
      KEEP(*(.image2.validate.rodata*))
      KEEP(*(.custom.validate.rodata*))
    } > BD_RAM

    .ram_image2.text :
    {
      KEEP(*(.infra.ram.start*))
      *(.mon.ram.text*)
      *(.hal.flash.text*)    
      *(.hal.sdrc.text*)
      *(.hal.gpio.text*)
      *(.text*)    
    } > BD_RAM

    .ram_image2.rodata :
    {
      *(.rodata*)
    } > BD_RAM

    .ram.data : 
    {
      __data_start__ = .;
      *(.data*)
      __data_end__ = .;
    	__ram_image2_text_end__ = .;    
    } > BD_RAM
    
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
    } > BD_RAM

    .bf_data : 
    {
      __buffer_data_start__ = .;
      *(.bfsram.data*)
      __buffer_data_end__ = .;
      
    } > BD_RAM

    .heap :
    {
       __end__ = .;
       end = __end__;
       *(.heap*)
       __HeapLimit = .;
    } > BD_RAM

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy :
	{
		*(.stack)
	} > BD_RAM

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ORIGIN(BD_RAM) + LENGTH(BD_RAM);
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

    .sdr_text :
    {
      __sdram_data_start__ = .;
      *(.sdram.text*)
      *(.p2p.text*)
      *(.wps.text*)
      *(.websocket.text*)
    } > SDRAM_RAM

    .sdr_rodata :
    {
      *(.sdram.rodata*)
      *(.p2p.rodata*)
      *(.wps.rodata*)
      *(.websocket.rodata*)
    } > SDRAM_RAM

    .sdr_data : 
    {
      *(.sdram.data*)
      *(.p2p.data*)
      *(.wps.data*)
      *(.websocket.data*)
      __sdram_data_end__ = .;
    } > SDRAM_RAM

    .sdr_bss :
    {
      __sdram_bss_start__ = .;
      *(.sdram.bss*)
      *(.p2p.bss*)
      *(.wps.bss*)
      *(.websocket.bss*)
      __sdram_bss_end__ = .;
    } > SDRAM_RAM

	.flash_map : 
    {
      __image1_flash_size__ = 0xB000;
      . = . + __image1_flash_size__;
      __image1_flash_end__ = .;

      __image2_flash_start__ = .;
      . = . + SIZEOF(.image2.start.table) + SIZEOF(.ram_image2.text) + +SIZEOF(.ram_image2.rodata) + SIZEOF(.ram.data);
      __image2_flash_end__ = .;

      __sdram_text_flash_start__ = .;
      . = . + SIZEOF(.sdr_text) + SIZEOF(.sdr_rodata) + SIZEOF(.sdr_data);
      __sdram_text_flash_end__ = .;

      /* ALIGN flash address to 4K address because of the Ameba flash driver's restriction */
      . = ALIGN(0x1000);
      __fatfs_start_address__ = .;
      . = ORIGIN(FLASH) + LENGTH(FLASH) - 1;
      __fatfs_end_address__ = .;
      __fatfs_num_blocks__ = (__fatfs_end_address__ - __fatfs_start_address__ + 1) / 4096;
    } > FLASH


}


