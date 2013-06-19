CC=gcc
CFLAGS=
TP = nr
DEPS = lubksb.h ludcmp.h nrutil.h
OBJ = $(TP).o lubksb.o ludcmp.o nrutil.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TP): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm $(TP) *.o 
