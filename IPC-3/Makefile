first: main.o
	g++ -o first main.cpp

second: main2.o
	g++ -o second main2.cpp

main.o: main.cpp shared_array.h
	g++ -c main.cpp

main2.o: main2.cpp shared_array.h
	g++ -c main2.cpp
	

.PHONY: clean

clean:
	rm -f *.o main first second
