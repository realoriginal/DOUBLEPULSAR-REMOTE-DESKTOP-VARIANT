SC_DEP  := $(wildcard shared/*.c)
SC_OBJ  := $(SC_DEP:%.c=%.o)
SC_LNK  := -Wl,-s,--no-seh,-Tinclude/linked.x64.ld -Tshared/sclink_64.ld

INCLUDE := -Ishared
CFLAGS  := -ffunction-sections -fno-asynchronous-unwind-tables -fno-ident -falign-functions -fPIC -nostdlib -Os -s

all: scbin.exe

scbin.exe: $(SC_OBJ)
	$(CC) -o entry.o -c entry.c $(INCLUDE) $(CFLAGS) 
	nasm -f win64 shared/entry_x64.asm -o shared/entry_x64.o
	nasm -f bin shared/irqlhop_x64.asm -o shared/irqlhop_x64.bin
	$(CC) -o $@ $^ entry.o shared/entry_x64.o $(CFLAGS) $(LFLAGS)
	python3 pedump.py
	cat shared/irqlhop_x64.bin scbin.bin > scbin_irql.bin

.c.o:
	$(CC) -o $@ -c $< $(INCLUDE) $(CFLAGS)

clean:
	rm -rf $(SC_OBJ)
	rm -rf entry.o
	rm -rf shared/entry_x64.o
	rm -rf scbin.exe
	rm -rf scbin.bin
	rm -rf shared/irqlhop_x64.bin
	rm -rf scbin_irql.bin
