cmake_minimum_required(VERSION 3.10)
project(ecore_bootstrap VERSION 0.1)
enable_language(CXX C ASM-ATT)
set(CMAKE_CXX_STANDARD 98)

set(tools /home/jwscoggins/sys/i960-elf-gcc-3.4.6)
set(CMAKE_C_COMPILER ${tools}/bin/i960-elf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/i960-elf-g++)
set(CMAKE_ASM-ATT_COMPILER ${tools}/bin/i960-elf-as)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DBARE_METAL")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DBARE_METAL --std=gnu99")
set(LINKER_SCRIPT "start.ld")
add_compile_options(-Wall)
add_link_options(-Wl,--as-needed -nostdlib -static -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} -ffreestanding)
if (DEFINED i960SB)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msb -mnumerics")
	set(CMAKE_ASM-ATT_FLAGS "${CMAKE_ASM_FLAGS} -ASB")
	add_link_options(-msb -mnumerics)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msa")
	set(CMAKE_ASM-ATT_FLAGS "${CMAKE_ASM_FLAGS} -ASA")
	add_link_options(-msa)
endif()
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed -nostdlib -static -T ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} -msb -mnumerics -ffreestanding")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
add_executable(bootstrap_ecore
		sx_init.s # this must ALWAYS be first!
		f_table.s
		i_handle.s
		i_table.s
		f_handle.cc
		ISR.cc
		hitagimain.cc
		Interrupts.cc
		Interrupts.h
		Faults.cc
		Faults.h
		)


# target_link_libraries(bootstrap_ecore gcc )
