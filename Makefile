CC=g++         # the compiler
LIBS=-lm       # the libraries we will ref
CFLAGS = -std=c++11

volimage: tool.o Driver.o
	$(CC) $(CFLAGS) tool.o Driver.o -o volimage $(LIBS) 

tool.o: tool.cpp tool.h
	$(CC) $(CFLAGS) -c tool.cpp

Driver.o: Driver.cpp
	$(CC) $(CFLAGS) -c Driver.cpp

clean: 
	rm -f *.o
	rm volimage
