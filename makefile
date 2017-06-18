#  Project Name
PROJECT=main

# libs dir
LIBDIR=c:\users\matej\cloudstation\arm\stm32\lib

# STM32 stdperiph lib defines
CDEFS = -DHSE_VALUE=((uint32_t)8000000) -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER

#  List of the objects files to be compiled/assembled
OBJECTS=main.o
OBJECTS+=ser.o
OBJECTS+=rng.o

CMSIS_SOURCES = \
$(LIBDIR)\cmsis\startup_stm32f40_41xxx.o \
$(LIBDIR)\cmsis\system_stm32f4xx.o
#$(LIBDIR)\cmsis\core_cm3.o

STM_SOURCES = \
$(LIBDIR)\stm32f4xx\src\misc.o \
$(LIBDIR)\stm32f4xx\src\stm32f4xx_gpio.o \
$(LIBDIR)\stm32f4xx\src\stm32f4xx_rcc.o \
$(LIBDIR)\stm32f4xx\src\stm32f4xx_usart.o \
$(LIBDIR)\stm32f4xx\src\stm32f4xx_rng.o

OBJECTS+=$(CMSIS_SOURCES)
OBJECTS+=$(STM_SOURCES)

LSCRIPT=$(LIBDIR)/cmsis/stm32f407xe.ld

OPTIMIZATION = s
DEBUG = dwarf-2
#LISTING = -ahls

#  Compiler Options
GCFLAGS = -g$(DEBUG)
GCFLAGS += $(CDEFS)
GCFLAGS += -O$(OPTIMIZATION)
GCFLAGS += -Wall -std=gnu99 -fno-common -mcpu=cortex-m3 -mthumb
GCFLAGS += -I$(LIBDIR)\stm32f4xx\inc -I$(LIBDIR) -I$(LIBDIR)/cmsis
#GCFLAGS += -Wcast-align -Wcast-qual -Wimplicit -Wpointer-arith -Wswitch
#GCFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
LDFLAGS = -mcpu=cortex-m3 -mthumb -O$(OPTIMIZATION) -Wl,-Map=$(PROJECT).map -T$(LSCRIPT)
ASFLAGS = $(LISTING) -mcpu=cortex-m3

#  Compiler/Assembler/Linker Paths
GCC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
REMOVE = del
SIZE = arm-none-eabi-size

#########################################################################

all:: $(PROJECT).hex $(PROJECT).bin stats

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary -j .text -j .data $(PROJECT).elf $(PROJECT).bin

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJECTS)
	$(GCC) $(LDFLAGS) $(OBJECTS) -o $(PROJECT).elf

stats: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

clean:
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(PROJECT).hex
	$(REMOVE) $(PROJECT).elf
	$(REMOVE) $(PROJECT).map
	$(REMOVE) $(PROJECT).bin

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  and assemble .s files to .o

.c.o :
	$(GCC) $(GCFLAGS) -c $< -o $@

.cpp.o :
	$(GCC) $(GCFLAGS) -c $< -o $@

.s.o :
	$(AS) $(ASFLAGS) -o $@ $<
#	$(AS) $(ASFLAGS) -o $(PROJECT)_crt.o $< > $(PROJECT)_crt.lst

#########################################################################
-include $(shell mkdir .dep) $(wildcard .dep/*)
