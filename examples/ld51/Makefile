OBJ = sdl.o \
	tmx/src/tmx_err.o \
	tmx/src/tmx_hash.o \
	tmx/src/tmx_mem.o \
	tmx/src/tmx_utils.o \
	tmx/src/tmx_xml.o \
	tmx/src/tmx.o \
	tmx_helpers.o

%.o: %.c
	gcc -c $< -I/usr/include/libxml2 -DWANT_ZLIB

target: $(OBJ)
	../../build/fcc

clean:
	rm -f *.o
	rm ld51
