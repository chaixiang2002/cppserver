# src=$(wildcard ./*.cpp)
# src=server.cpp \
# 	client.cpp
# OBJECT=$(patsubst %.cpp,%.o,$(src))
other=$(wildcard include/*.cpp)


CC=g++
CPPFLAGS= -std=c++11 -pthread -g -o

run:$(other)
	clear
	$(CC) server.cpp $(other) $(CPPFLAGS) server
	$(CC) client.cpp $(other)  $(CPPFLAGS) client
	./server


# %.o:%.cpp
# 	$(CC) $(CPPFLAGS) $< -o $@ 
test1:
	clear
	$(CC) ThreadPoolTest.cpp $(other) $(CPPFLAGS) ThreadPoolTest
	./ThreadPoolTest
test2:
	clear
	$(CC) test.cpp $(other) $(CPPFLAGS) test
	

clean:
	clear
	rm -rf  *out server client *.o test ThreadPoolTest