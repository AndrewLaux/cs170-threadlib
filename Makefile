all:
	g++ -g -m32 -c thread.cpp

tests:
	g++ -g -m32 -c thread.cpp
	g++ -g -m32 -o 1 basic_1.c thread.o
	g++ -g -m32 -o 2 basic_2.c thread.o
	g++ -g -m32 -o 3 basic_3.c thread.o
	g++ -g -m32 -o 4 basic_4.c thread.o

run-1:
	./1.exe > basic_1_test.out

run-2:
	./2.exe > basic_2_test.out

run-3:
	./3.exe > basic_3_test.out

run-4:
	./4.exe > basic_4_test.out

clean:
	rm -f *.exe* *.o *test.out 1 2 3 4
