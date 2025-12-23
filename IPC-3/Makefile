all: first second

first: main.o
	g++ -o first main.o

second: main2.o
	g++ -o second main2.o

main.o: main.cpp shared_array.h
	g++ -c main.cpp

main2.o: main2.cpp shared_array.h
	g++ -c main2.cpp

clean:
	rm -f *.o first second

