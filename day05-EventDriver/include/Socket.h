#pragma once //???????
class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    void connect(InetAddress*);

    int accept(InetAddress*);
    int getFd();
};


