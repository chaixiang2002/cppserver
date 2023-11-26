# src=$(wildcard ./*.cpp)
src=server.cpp \
	client.cpp
target=$(patsubst %.cpp,%.out,$(src))
CC=g++
CPPFLAGS= -lws2_32 -o

server:$(target)
	./$<


%.out:%.cpp
	$(CC) util.cpp $< $(CPPFLAGS) $@ 
	

clean:
	rm -rf *.out *out server client