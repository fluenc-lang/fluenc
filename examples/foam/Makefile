OBJ = sdl.o

%.o: %.c
	gcc -c $<

target: $(OBJ)
	../../build/fcc/fcc build

clean:
	rm -f *.o
	rm foam
