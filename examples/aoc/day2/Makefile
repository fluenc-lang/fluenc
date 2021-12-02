OBJ = main.o print.o

%.o: %.dz
	../../../dz $<

%.o: %.c
	gcc -c $<

day2: $(OBJ)
	ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o $^ -lc /usr/lib/crtn.o -o $@

clean:
	rm -f *.o
