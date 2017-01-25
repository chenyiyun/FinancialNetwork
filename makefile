CC=gcc
CFLAGS=-I. -Wall -Werror -std=c99
DEPS = param.h lapacke.h matrixCalculator.h node.h Queue.h random.h traverse.h genetic.h
OBJ = matrixCalculator.o node.o Queue.o random.o traverse.o genetic.o main.o
LIBRARIES=liblapacke.a liblapack.a librefblas.a
LOADER=gfortran

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

financial: $(OBJ) $(LIBRARIES)
	$(LOADER) -o $@ $^ $(LIBRARIES)

clean:
	rm -f *.o
