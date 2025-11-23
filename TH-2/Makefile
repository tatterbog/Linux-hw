OBJS = main.o parallel_scheduler.o

TARGET = demo

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $(TARGET) $(OBJS)

main.o: main.cpp parallel_scheduler.h
	g++ -c main.cpp

parallel_scheduler.o: parallel_scheduler.h parallel_scheduler.cpp
	g++ -c parallel_scheduler.cpp
