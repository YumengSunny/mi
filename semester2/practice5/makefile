# Vorschlag eines Makefile zu Termin5 SS2011
# Quelldatei
FILE = Termin5Aufgabe1
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
	$(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) $(FILE).c -I ../h
	$(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) seriell.c -I ../h
	$(TOOLCHAIN)$(COMPILER) -S -O$(OPTI) swi.c -I ../h

# Erzeugen der benoetitgen Objektdateien
# eigener SoftWareInterrupt-Handler
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) swi.c -o swi.o -I ../h
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) seriell.c -o seriell.o -I ../h
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ser_io.S -o ser_io.o -I ../h
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/boot_ice.S -o boot_ice.o -I ../h

# Erzeugen der benoetitgen Objektdateien
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/swi.S -o swi.o -I ../h
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/boot_ice.S -o boot_ice.o -I ../h
	
# Binden fuer die RAM-Version 
#	$(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o $(FILE).o -o $(FILE).elf  /opt/$(TOOLCHAIN)/gcc/arm-eb63-elf/4.4.6/lib/libgcc.a
	$(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o $(FILE).o -o $(FILE).elf

# Debugger starten
debug:
	$(TC)$(DEBUGGER) $(FILE).elf

# Aufraeumen
clean:
	rm *.o
	rm *.s
	rm *.elf
	rm *.rom


