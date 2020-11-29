ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o output.o -lc /usr/lib/crtn.o
./a.out
echo $?
