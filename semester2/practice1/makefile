# Quellendatei
FILE = practice1
# Toolchain
TC = arm-eb63-elf-
# Compiler
GCC = gcc
# Optimierungsstufe
OPTI = 1

all:
# ‹bersetzen und binden der Quelldatei
	$(TC)$(GCC) -g -O$(OPTI) $(FILE).c -o $(FILE).elf
# Erzeugen des Assemblercode aus der C-Datei
	$(TC)$(GCC) -S -O$(OPTI) $(FILE).c

