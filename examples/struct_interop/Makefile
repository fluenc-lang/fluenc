OBJ = main.o utils.o

%.o: %.fc
	../../fcc $<

%.o: %.c
	gcc -c $<

hello_world: $(OBJ)
	ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o $^ -lc /usr/lib/crtn.o -o $@

clean:
	rm -f *.o
