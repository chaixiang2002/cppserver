# src=$(wildcard ./*.cpp)
# src=server.cpp \
# 	client.cpp
# OBJECT=$(patsubst %.cpp,%.o,$(src))
other =	Epoll.cpp \
		InetAddress.cpp \
		Socket.cpp	\
		Channel.cpp
CC=g++
CPPFLAGS= -o

run:$(other)
	$(CC) util.cpp server.cpp $(other) $(CPPFLAGS) server
	$(CC) util.cpp client.cpp $(other)  $(CPPFLAGS) client


# %.o:%.cpp
# 	$(CC) $(CPPFLAGS) $< -o $@ 
	

clean:
	rm -rf *.out *out server client *.o