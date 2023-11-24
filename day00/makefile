src=$(wildcard ./*.cpp)
target=$(patsubst %.cpp,%.out,$(src))
CC=g++
CPPFLAGS= -o

server:$(target)
	./server.out


%.out:%.cpp
	$(CC) $< $(CPPFLAGS) $@ 
	

clean:
	rm -rf *.out *out server client