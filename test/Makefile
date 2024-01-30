#CC = gcc 
#CC = icc 
CPP = g++


CFLAGS = -std=c++14 -O3 -g 
CSUFFIX = -mavx -mavx2 -mfma -mpopcnt -msse4 -mavx512bw -mavx512vl -lpthread -fopenmp -static-libstdc++

all: 
	$(CPP) $(CFLAGS) -o chunk_csv_test.out ../src/*.cpp test.cpp $(CSUFFIX)

clean:
	rm -f *.out *.o
