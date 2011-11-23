VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0
NAME = "Bday Edition"

CROSS_COMPILE = arm-arago-linux-gnueabi-

CC = ${CROSS_COMPILE}gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJFMT	= binary

SRCDIR = src
BINDIR = bin

INCLUDES = $(SRCDIR)/include
CFLAGS =-mcpu=cortex-m3 -mthumb -nostdlib -Wall -g -I$(INCLUDES)
LDFLAGS =-nostartfiles -fno-exceptions -Tfirmware.ld

EXECUTABLE=am335x-pm-firmware.elf

.PHONY: all clean

SOURCES = $(shell find $(SRCDIR) -name *.c)
OBJECTS = $(SOURCES:.c=.o)

$(EXECUTABLE): $(OBJECTS)
	@echo "Compiling..."
	@echo $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINDIR)/$@

all: $(EXECUTABLE)
	@echo "Linking..."
	$(OBJCOPY) -O$(OBJFMT) $(BINDIR)/$(EXECUTABLE) $(BINDIR)/$(EXECUTABLE:.elf=.bin)

clean:
	@echo "Cleaning up..."
	-$(shell find . -name *.o -exec rm {} \;)
	-$(shell rm -f $(BINDIR)/$(EXECUTABLE))
	-$(shell rm -f $(BINDIR)/$(EXECUTABLE:.elf=.bin))
	@echo "Done!"
