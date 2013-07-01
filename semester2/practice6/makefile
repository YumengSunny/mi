# Vorschlag eines Makefile zu Termin5 SS2011
# Quelldatei
FILE = exercise6
# Toolchain
TOOLCHAIN = arm-eb63-elf-
# Compiler
COMPILER = gcc
# Linker/Binder
LINKER = ld
# Debugger
DEBUGGER = insight
# Optimierungsstufe
OPTI = 0

# Bauen
all:
# uebersetzen der Quelldatei
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) $(FILE).c -I ../h

# Erzeugen der Assemblerdatei aus der Quelldatei
    $(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) $(FILE).c
    $(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) seriell.c
    $(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) swi.c

# Erzeugen der benoetitgen Objektdateien
# eigener SoftWareInterrupt-Handler
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) swi.c -o swi.o
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) seriell.c -o seriell.o
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ser_io.S -o ser_io.o
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/boot_ice.S -o boot_ice.o -I ../h

# Erzeugen der benoetitgen Objektdateien
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) swi.s -o swi.o
    $(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/boot_ice.S -o boot_ice.o

# Binden fuer die RAM-Version
    $(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o seriell.o ser_io.o $(FILE).o -o $(FILE).elf  /opt/arm-eb63-elf/lib/gcc/arm-eb63-elf/4.4.6/libgcc.a

# Debugger starten
debug:
    $(TC)$(DEBUGGER) $(FILE).elf

# Aufraeumen
clean:
    rm *.o
    rm *.s
    rm *.elf
    rm *.rom


