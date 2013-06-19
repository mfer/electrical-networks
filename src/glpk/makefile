CC=gcc
CFLAGS=-lm -lglpk
TP = okalg
OBJ = $(TP).o 

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TP): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm $(TP) *.o 
