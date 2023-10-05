# src=$(wildcard ./*.cpp)
# src=server.cpp \
# 	client.cpp
# OBJECT=$(patsubst %.cpp,%.o,$(src))
other=$(wildcard include/*.cpp)


CC=g++
CPPFLAGS= -std=c++11 -o

run:$(other)
	$(CC) server.cpp $(other) $(CPPFLAGS) server
	$(CC) client.cpp $(other)  $(CPPFLAGS) client
	./server


# %.o:%.cpp
# 	$(CC) $(CPPFLAGS) $< -o $@ 
	

clean:
	rm -rf *.out *out server client *.o