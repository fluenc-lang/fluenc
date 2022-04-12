OBJ = print.o

%.o: %.c
	gcc -c $<

target: $(OBJ)
	../../../fcc

clean:
	rm -f *.o
	rm day3
