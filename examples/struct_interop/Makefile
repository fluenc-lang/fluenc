OBJ = utils.o

%.o: %.c
	gcc -c $<

target: $(OBJ)
	../../fcc

clean:
	rm -f *.o
	rm struct_interop
