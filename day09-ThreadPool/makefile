# src=$(wildcard ./*.cpp)
# src=server.cpp \
# 	client.cpp
# OBJECT=$(patsubst %.cpp,%.o,$(src))
other=$(wildcard include/*.cpp)


CC=g++
CPPFLAGS= -std=c++11 -pthread -o

run:$(other)
	clear
	$(CC) server.cpp $(other) $(CPPFLAGS) server
	$(CC) client.cpp $(other)  $(CPPFLAGS) client
	./server


# %.o:%.cpp
# 	$(CC) $(CPPFLAGS) $< -o $@ 
test:
	clear
	$(CC) ThreadPoolTest.cpp $(other) $(CPPFLAGS) test
	./test

clean:
	clear
	rm -rf *.out *out server client *.o test