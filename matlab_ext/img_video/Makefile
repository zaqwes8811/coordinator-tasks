CFLAGS=-g  $(CFLAGS)
CCFLAGS=-g `pkg-config opencv --cflags` $(CCFLAGS)
#LDFLAGS= 
LIBS=$(CV_LIB)

# Need it
CC=g++

all: w1.o 
	$(CC) w1.o -o  $@ `pkg-config opencv --libs`

w1.o: w1.cc
	$(CC) -std=c++11 -c w1.cc `pkg-config opencv --cflags`

clean:
	rm *.o w1
