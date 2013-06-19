CC=gcc
CFLAGS=-lm -lglpk
TP = relax4
OBJ = $(TP).o /usr/local/lib/libglpk.a

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TP): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm $(TP) *.o 
