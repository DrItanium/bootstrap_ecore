GROUP(-liberty ) /* libraries to link in */

MEMORY 
{
	/*
	 * The ecore processor starts internal execution at 0xFFFD0000
	 * This is mapped to an internal PROGMEM section on the mega2560 (or other
	 * large scale microcontrollers
	 *
	 * It is only 64k in length but should be just enough to get stuff working
	 */
	rom (rx)  : ORIGIN = 0xFFFD0000, l = 64k
	/*
	 * Only expose a small section of onboard sram to be used to bootstrap
	 * the system and test things out. It comes _after_ the rom section.
	 * It assists in testing various routines and aspects of the emulation for
	 * correctness.
	 */
	ram (!rx) : ORIGIN = 0xFFFE0000, l = 2k
} 

/* Define a symbol that your code can reference. The linker adds the symbol. */ 

/* Layout the different ELF sections into memory using the above described
 * memory spaces */ 
SECTIONS
{
	/* this is the text section */ 
	.text : 
	{ 
		CREATE_OBJECT_SYMBOLS /* C++ needs this */ 

		*(.text) 	/* place the actual contents of the text section here */ 

		_etext = .; /* define a symbol named _etext and assign it to the
					   address following the end of the text section data */ 

	/* the arrow syntax tells the linker that the text section will go into the
	 * memory location named "rom", it is sequential as we will see in the next
	 * few sections*/ 
	} >rom 
	/* setup the bss section, tag it's start and end positions to allow the
	 * i960 to manually zero out the memory on the sdcard's ram.bin file */ 
	.bss : 
	{ 
		__bss_start__ = .; 
		*(.bss) 
		*(COMMON) /* can't find much documentation on this but it is a good idea to have this here */ 
		__bss_end__ = .; 
	} >ram 
	.data :
	{
		__data_start__ = .; 
		KEEP(*(.data)) 
		__data_end__ = .;
		_edata = .; 
	} >ram 
}



/* Currently, I am manually decomposing the elf binary into ROM (.text) and DAT
 * (.data) files from the ELF using objcopy.  

 I have the chipset map the data section to 0x2000'0000 so we need to
 reflect that in the linker script (start_ip in the sx_init.s uses this). 

 I also compute the data section's length ahead of time to save instructions.  



 It is the responsibility of the i960 to copy the contents of the mapped data
 section into ram starting at address 0x8000'0000 on startup. Failure to do so
 leads to the inability to properly allocate memory (no malloc) and other
 various oddities. 



 The variable cs1 is specific to the i960 and is used in it's boot process.
 The processor on startup loads the first eight words (32 bytes) and performs
 a check sum with them. The processor will refuse to boot if the checksum
 algorithm does not yield a zero from combining these eight words together.
 Thus cs1 is used to make sure that 0 is the final result. The i960 SA/SB
 reference manual is where I got the cs1 line 



 The BSS Section length is used on startup to "copy" the bss into ram like the
 data section (although BSS copy happens first because it comes before data)..
 In reality this just zeroing out ram on the SDCard (ram.bin) to prevent bad
 things from happening during program execution 

 */ 

theDataSectionLength = (__data_end__ - __data_start__); 
cs1 = -(system_address_table + prcb_ptr + start_ip) ;
theBSSSectionLength = (__bss_end__ - __bss_start__); 

/* This directive allows you to denote the file that will go onto the front of
 * the text section, the problem is that with CMake this moves around. So
 * instead, I just make sure that sx_init.s is the first file in the list of
 * dependencies in CMakeFiles.txt. It does the same thing as the linker
 * appplies objects on a first come first serve basis. Very nice! */ 

/* STARTUP ( CMakeFiles/hitagimon.dir/sx_init.s.o ) */ 

/*

How to dump the actual boot code: 


i960-elf-objcopy --only-section=.text -O binary hitagimon hitagimon.bin 


You have to make sure that you don't dump out the data section because that can
get very large! This is because, objcopy is turning the dynamic elf into a
statically mapped binary blob. Since ram starts at 0x8000'0000 the blob must
have ram mapped at that exact location for bss and data. Thus the blob becomes
over 2 gigabytes in size! This returns the baffling error "not enough space on
device" from i960-elf-objcopy.  

 */ 
