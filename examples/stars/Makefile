OBJ = sdl.o

%.o: %.c
	gcc -c $<

target: $(OBJ)
	../../build/fcc

clean:
	rm -f *.o
	rm stars
