CC = g++
CC_FLAGS = -Wall
CC_OMP = -fopenmp

all: omp.out   

clean:
	rm result.jpg omp.out 

omp.out: src/mainKmeansP.cpp src/image_io.h src/image_io.cpp  src/Kmeans_omp.cpp
	$(CC) $(CC_FLAGS) $(CC_OMP) -o omp.out src/mainKmeansP.cpp src/image_io.cpp src/Kmeans_omp.cpp -lm

