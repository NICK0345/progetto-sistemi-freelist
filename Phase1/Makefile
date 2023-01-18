# Cross toolchain variables
# If these are not in your path, you can make them absolute.
XT_PRG_PREFIX = mipsel-linux-gnu-
CC = $(XT_PRG_PREFIX)gcc
LD = $(XT_PRG_PREFIX)ld

# uMPS3-related paths

# Simplistic search for the umps3 installation prefix.
# If you have umps3 installed on some weird location, set UMPS3_DIR_PREFIX by hand.

ifneq ($(wildcard /usr/bin/umps3),)
	UMPS3_DIR_PREFIX = /usr
else
	UMPS3_DIR_PREFIX = /usr/local
endif

UMPS3_DATA_DIR = $(UMPS3_DIR_PREFIX)/share/umps3
UMPS3_INCLUDE_DIR = $(UMPS3_DIR_PREFIX)/include/umps3
UMPS3_INCLUDE_DIR2 = $(UMPS3_DIR_PREFIX)/include/umps3/umps
UMPS3_INCLUDE_DIR3 = /usr/include

# Compiler options
CFLAGS_LANG = -ffreestanding 
CFLAGS_MIPS = -mips1 -mabi=32 -mno-gpopt -EL -G 0 -mno-abicalls -fno-pic -mfp32
CFLAGS = $(CFLAGS_LANG) $(CFLAGS_MIPS) -I$(UMPS3_INCLUDE_DIR) -I$(UMPS3_INCLUDE_DIR3) -I$(UMPS3_INCLUDE_DIR2) -Wall -O0

# Linker options
LDFLAGS = -G 0 -nostdlib -T $(UMPS3_DATA_DIR)/umpscore.ldscript -m elf32ltsmip

# Add the location of crt*.S to the search path
VPATH = $(UMPS3_DATA_DIR)
BINDIR  := ./bin
SRCDIR  := ./src
FILES = p1test pcb ash crtso libumps
OBJECTS = $(addprefix $(BINDIR)/, $(addsuffix .o,$(FILES)))

.PHONY : all clean

$(shell mkdir -p $(BINDIR))
$(shell mkdir -p $(SRCDIR))

all : $(BINDIR)/kernel.core.umps

$(BINDIR)/kernel.core.umps : $(BINDIR)/kernel
	umps3-elf2umps -k $<
$(BINDIR)/kernel : $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

clean :
	-rm -f $(BINDIR)/*.o $(BINDIR)/kernel $(BINDIR)/kernel.*.umps
# Pattern rule for assembly modules
$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(BINDIR)/%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@
