OBJ = sdl.o

%.o: %.c
	gcc -c $<

target: $(OBJ)
	../../fcc

clean:
	rm -f *.o
	rm tic-tac-toe
