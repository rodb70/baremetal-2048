################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# Author:  Arda Pekis
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = 2048

# Here you must put a list of all of the object files
# that will be compiled into your program. For example
# if you have main.c and myLib.c then in the following
# line you would put main.o and myLib.o
OFILES = main.o draw.o endScreen.o startScreen.o screens.o keys.o game.o Tile2.o Tile4.o Tile8.o Tile16.o Tile32.o Tile64.o Tile128.o Tile256.o Tile512.o Tile1024.o Tile2048.o Digits.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space (SPACE!) separated list of .h files
HFILES = main.h draw.h endScreen.h startScreen.h screens.h keys.h game.h Tile2.h Tile4.h Tile8.h Tile16.h Tile32.h Tile64.h Tile128.h Tile256.h Tile512.h Tile1024.h Tile2048.h Digits.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################
include res/GBAVariables.mak

.PHONY: all
all : CFLAGS += $(CRELEASE)
all: $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

$(PROGNAME).gba : $(PROGNAME).elf
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf : res/crt0.o $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o $(OFILES)
	@$(CC) $(LINKFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc $(LDDEBUG)
	@rm -f *.d


%.o : %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o : %.s                                                                        
	@echo "[ASSEMBLE] Assembling $<"                                               
	@$(AS) $(MODEL) $< -o $@

.PHONY : vba
vba : CFLAGS += $(CRELEASE)
vba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@vbam $(VBAOPT) $(PROGNAME).gba > /dev/null 2> /dev/null

.PHONY : mgba
mgba : CFLAGS += $(CRELEASE)
mgba : $(PROGNAME).gba
	@echo "[EXECUTE] Running Emulator VBA-M"
	@mgba $(PROGNAME).gba > /dev/null 2> /dev/null

.PHONY : med
med : CFLAGS += $(CRELEASE)
med : $(PROGNAME).gba
	@echo "[EXECUTE] Running emulator Mednafen"
	@mednafen $(PROGNAME).gba > /dev/null 2>&1

.PHONY : clean
clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o *.elf *.gba *.d res/*.o res/*.d

-include $(CFILES:%.c=%.d)
