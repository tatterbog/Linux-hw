LIB_NAME = libparallel_scheduler.so
LIB_CPP = parallel_scheduler.cpp
LIB_H = parallel_scheduler.h


all: $(LIB_NAME) main

$(LIB_NAME): parallel_scheduler.o
	g++ -shared -o $(LIB_NAME) parallel_scheduler.o -lpthread

parallel_scheduler.o: $(LIB_CPP) $(LIB_H)
	g++ -c -fPIC $(LIB_CPP) -lpthread

main: main.o $(LIB_NAME) 
	g++ -o main main.o -L. -lparallel_scheduler -lpthread -Wl,-rpath,.

main.o: main.cpp $(LIB_H)
	g++ -c main.cpp

clean: 
	rm -f *.o *.so main
