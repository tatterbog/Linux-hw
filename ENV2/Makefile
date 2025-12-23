OBJS = main.o Complex.o Sort.o

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $(TARGET) $(OBJS)

main.o: main.cpp Complex.h Sort.h
	g++ -c main.cpp

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp

Sort.o: Sort.cpp Sort.h Complex.h
	g++ -c Sort.cpp

clean:
	rm -f *.o $(TARGET)
