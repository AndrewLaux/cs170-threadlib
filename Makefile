all:
	g++ -c thread.cpp
	g++ -g -o test main.cpp thread.o

clean:
	rm *.exe *.o 