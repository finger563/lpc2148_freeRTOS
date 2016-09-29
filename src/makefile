################################################################################
################################################################################
################################################################################
#
# MAKEFILE
# PROJECT: LOGODYNAMITE
#
################################################################################
#-------------------------------------------------------------------------------
#
# PROJECT DETAILS
#
#-------------------------------------------------------------------------------
PROJECT_NAME            = LOGOMATIC_FREERTOS
#-------------------------------------------------------------------------------
#
# ENVIRONMENT CONFIGURATION
#
#-------------------------------------------------------------------------------
# Define programs and commands.
TARGET_PREFIX  = arm-none-eabi-
CC             = $(TARGET_PREFIX)gcc
CPP            = $(TARGET_PREFIX)g++
LD             = $(TARGET_PREFIX)g++
OBJCOPY        = $(TARGET_PREFIX)objcopy
OBJDUMP        = $(TARGET_PREFIX)objdump
SIZE           = $(TARGET_PREFIX)size
NM             = $(TARGET_PREFIX)nm
SHELL          = sh
REMOVE         = rm -f
COPY           = cp

# ---------------------------------------------------------------------------
# Flash-Programming support using lpc21isp by Martin Maurer 
# only for Philips LPC and Analog ADuC ARMs
#
# Settings and variables:
#LPC21ISP = lpc21isp
LPC21ISP = lpc21isp
LPC21ISP_PORT = COM4
LPC21ISP_BAUD = 38400
LPC21ISP_XTAL = 12000
LPC21ISP_FLASHFILE = $(PROJECT_NAME).hex
# verbose output:
#LPC21ISP_DEBUG = -debug
# enter bootloader via RS232 DTR/RTS (only if hardware supports this
# feature - see Philips AppNote):
LPC21ISP_CONTROL = -control
# ---------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#
# CONFIGURATION
#
#-------------------------------------------------------------------------------
CSTANDARD               = -std=gnu99
DEBUG                   = dwarf-2
FINAL_OUTPUT_FILE       = FW.SFE
MCU                     = arm7tdmi-s
OPTIMIZATION            = 0
SUBMDL                  = LPC2148
USE_THUMB_MODE          = YES

#-------------------------------------------------------------------------------
#
# PREPROCESSOR
#
#-------------------------------------------------------------------------------
### CDEFS: C/C++ definitions
#### FreeRTOS
CDEFS += -DGCC_ARM7
CDEFS += -DROM_RUN
### ADEFS: Assembler definitions
ADEFS =
#ADEFS += -DLPC2000_EXCEPTION_VECTORS_IN_RAM=1
ADEFS += -DROM_RUN
#-------------------------------------------------------------------------------
#
# SOURCE DIRECTORIES
#
#-------------------------------------------------------------------------------
FREERTOS_SOURCE_DIR        = FreeRTOS/Source/
FREERTOS_PORT_DIR          = $(FREERTOS_SOURCE_DIR)/portable/GCC/ARM7_LPC2000/
SFE_SOURCE_DIR             = lib/
LOGOMATIC_SOURCE_DIR       = logomaticv2/

#-------------------------------------------------------------------------------
#
# SOURCE FILES
#
#-------------------------------------------------------------------------------
# EXTRAINCDIRS: user include directories
EXTRAINCDIRS =
EXTRAINCDIRS += $(FREERTOS_SOURCE_DIR)/include
EXTRAINCDIRS += $(FREERTOS_PORT_DIR)
EXTRAINCDIRS += $(SFE_SOURCE_DIR)
EXTRAINCDIRS += $(LOGOMATIC_SOURCE_DIR)

### CSRC: C source files (*.c)
CSRC =
CSRC += $(SFE_SOURCE_DIR)/syscalls.c
CSRC += $(FREERTOS_SOURCE_DIR)/list.c
CSRC += $(FREERTOS_SOURCE_DIR)/queue.c
CSRC += $(FREERTOS_SOURCE_DIR)/tasks.c
CSRC += $(FREERTOS_PORT_DIR)/port.c
CSRC += $(FREERTOS_SOURCE_DIR)/portable/MemMang/heap_1.c

### CSRCARM: C source files (*.c) that must be compiled in ARM-Mode.
CSRCARM =
CSRCARM += $(FREERTOS_PORT_DIR)/portISR.c

### CPPSRC: C++ source files (*.cpp)
CPPSRC = 
CPPSRC += ./main.cpp
CPPSRC += $(LOGOMATIC_SOURCE_DIR)/logomatic_v2.cpp

### CPPSRCARM: C++ source files (*.cpp) that must be compiled in ARM-Mode.
CPPSRCARM =

### ASRC: Assembler source files (*.S) - Extension has to be capital or file
# will be deleted during clean.
ASRC = 

# ASRCARM: assembler source files (*.S) that be assembled in ARM-Mode..
ASRCARM  = Startup.S #Lpc2000ExceptionVectors.S Arm7CStartup.S
#-------------------------------------------------------------------------------
#
# COMPILER CONFIGURATION
#
#-------------------------------------------------------------------------------
ifeq ($(USE_THUMB_MODE),YES)
#THUMB    = -mthumb
THUMB_IW = -mthumb-interwork
#### FreeRTOS
CDEFS   += -DTHUMB_INTERWORK
endif

# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
#

# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS = $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPTIMIZATION)
CFLAGS += -Wall -Wextra -Wcast-align
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -mlong-calls
#CFLAGS += -ffunction-sections
#CFLAGS += -fdata-sections
CFLAGS += -fno-exceptions
CFLAGS += -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wsign-compare -Waggregate-return 
CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<) 
CFLAGS += -g

# flags only for C
CONLYFLAGS += $(CSTANDARD)
CONLYFLAGS += -Wmissing-prototypes -Wimplicit 
CONLYFLAGS += -Wbad-function-cast -Wstrict-prototypes
CONLYFLAGS += -Wmissing-declarations

### CPPFLAGS: C++ flags only (arm-elf-g++)
CPPFLAGS = 
CPPFLAGS += -fno-rtti
CPPFLAGS += -fno-use-cxa-atexit

# Assembler flags.
#  -Wa,...:    tell GCC to pass this to the assembler.
#  -ahlns:     create listing
#  -g$(DEBUG): have the assembler create line number information
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),--g$(DEBUG)

### EXTRA_LIBS: user libraries
EXTRA_LIBS =

#Support for newlibc-lpc (file: libnewlibc-lpc.a)
#NEWLIBLPC = -lnewlib-lpc
MATH_LIB = -lm
CPLUSPLUS_LIB = -lstdc++

### LDFLAGS: Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -nostartfiles -Wl,-Map=$(PROJECT_NAME).map,--cref
LDFLAGS += --specs=nosys.specs
LDFLAGS += -lnosys
LDFLAGS += -lc
LDFLAGS += $(NEWLIBLPC)
LDFLAGS += $(MATH_LIB)
LDFLAGS += -lc -lgcc
LDFLAGS += $(CPLUSPLUS_LIB)
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS))

# Set Linker-Script Depending On Selected Memory and Controller
#LDFLAGS +=-T$(PROJECT_NAME)_$(SUBMDL).ld
LDFLAGS +=-Tmain_memory_block.ld
### Define Messages
MSG_COMPILATION_SUCCEEDED = Compilation succeeded
MSG_BEGIN = "-------- Build started --------"
MSG_END = "--------  Build over  --------"
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_LINKING_SYMBOL_TABLE = Linking symbol table:
MSG_COMPILING = Compiling C:
MSG_COMPILING_ARM = "Compiling C (ARM-only):"
MSG_COMPILINGCPP = Compiling C++:
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING = Assembling:
MSG_ASSEMBLING_ARM = "Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:
MSG_FORMATERROR = Can not handle output-format
#-------------------------------------------------------------------------------
#
# GENERAL PURPOSE COMPILATION
#
#-------------------------------------------------------------------------------
# Define all object files.
COBJ      = $(CSRC:.c=.o) 
AOBJ      = $(ASRC:.S=.o)
COBJARM   = $(CSRCARM:.c=.o)
AOBJARM   = $(ASRCARM:.S=.o)
CPPOBJ    = $(CPPSRC:.cpp=.o) 
CPPOBJARM = $(CPPSRCARM:.cpp=.o)
# Listing files.
LST = $(ASRC:.S=.lst) $(ASRCARM:.S=.lst) $(CSRC:.c=.lst) $(CSRCARM:.c=.lst)
LST += $(CPPSRC:.cpp=.lst) $(CPPSRCARM:.cpp=.lst)
# Dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -mcpu=$(MCU) $(THUMB_IW) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I. -x assembler-with-cpp $(ASFLAGS)
# Default target.
all: begin gccversion sizebefore build sizeafter finished program end
build: elf bin lss sym hex $(FINAL_OUTPUT_FILE)
hex: $(PROJECT_NAME).hex
elf: $(PROJECT_NAME).elf
lss: $(PROJECT_NAME).lss 
sym: $(PROJECT_NAME).sym

begin:
	@echo
	@echo $(MSG_BEGIN)
	@echo PROJECT: $(PROJECT_NAME)
finished:
	@echo $(MSG_COMPILATION_SUCCEEDED)
end:
	@echo $(MSG_END)
	@echo
ELFSIZE = $(SIZE) -A $(PROJECT_NAME).elf
sizebefore:
	@if [ -f $(PROJECT_NAME).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi
sizeafter:
	@if [ -f $(PROJECT_NAME).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi
gccversion : 
	@$(CC) --version
$(FINAL_OUTPUT_FILE): $(PROJECT_NAME).elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@
program:
	@echo
	@echo $(MSG_LPC21_RESETREMINDER)
	$(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_DEBUG) $(LPC21ISP_FLASHFILE) $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O ihex $< $@
%.bin: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O binary $< $@
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -C $< > $@
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@
.SECONDARY : $(PROJECT_NAME).elf
.PRECIOUS : $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
%.elf:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM)
	@echo
	@echo $(MSG_LINKING) $@
	$(CPP) $(THUMB) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) --output $@ $(LDFLAGS)
%.elfagain:  $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) symbols.o
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(THUMB) $(ALL_CFLAGS) $(AOBJARM) $(AOBJ) $(COBJARM) $(COBJ) $(CPPOBJ) $(CPPOBJARM) symbols.o --output $@ $(LDFLAGS)

$(COBJ) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 
$(COBJARM) : %.o : %.c
	@echo
	@echo $(MSG_COMPILING_ARM) $<
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $< -o $@ 
$(CPPOBJ) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP) $<
	$(CPP) -c $(THUMB) $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 
$(CPPOBJARM) : %.o : %.cpp
	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $<
	$(CPP) -c $(ALL_CFLAGS) $(CPPFLAGS) $< -o $@ 
$(AOBJ) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(THUMB) $(ALL_ASFLAGS) $< -o $@
$(AOBJARM) : %.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING_ARM) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@
clean: begin clean_list finished end
clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) ../Common_WinARM/$(PROJECT_NAME).hex
	$(REMOVE) $(PROJECT_NAME).hex
	$(REMOVE) $(PROJECT_NAME).bin
	$(REMOVE) $(PROJECT_NAME).obj
	$(REMOVE) $(PROJECT_NAME).elf
	$(REMOVE) $(PROJECT_NAME).map
	$(REMOVE) $(PROJECT_NAME).obj
	$(REMOVE) $(PROJECT_NAME).a90
	$(REMOVE) $(PROJECT_NAME).sym
	$(REMOVE) $(PROJECT_NAME).lnk
	$(REMOVE) $(PROJECT_NAME).lss
	$(REMOVE) symbol_table.o
	$(REMOVE) $(COBJ)
	$(REMOVE) $(CPPOBJ)
	$(REMOVE) $(AOBJ)
	$(REMOVE) $(COBJARM)
	$(REMOVE) $(CPPOBJARM)
	$(REMOVE) $(AOBJARM)
	$(REMOVE) $(LST)
	$(REMOVE) $(CSRC:.c=.s)
	$(REMOVE) $(CSRC:.c=.d)
	$(REMOVE) $(CSRCARM:.c=.s)
	$(REMOVE) $(CSRCARM:.c=.d)
	$(REMOVE) $(CPPSRC:.cpp=.s) 
	$(REMOVE) $(CPPSRC:.cpp=.d)
	$(REMOVE) $(CPPSRCARM:.cpp=.s) 
	$(REMOVE) $(CPPSRCARM:.cpp=.d)
	$(REMOVE) $(FINAL_OUTPUT_FILE)
	$(REMOVE) .dep/*
	$(REMOVE) *.o
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program
