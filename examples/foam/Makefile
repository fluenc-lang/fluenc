OBJ = main.o sdl.o

%.o: %.dz
	../../fcc $<

%.o: %.c
	gcc -c $<

foam: $(OBJ)
	ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o $^ -lc /usr/lib/crtn.o -lSDL2 -o $@

clean:
	rm -f *.o
