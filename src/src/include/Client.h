
#pragma once

class Socket;
class InetAddress;
class Buffer;

class Client
{
private:
    Socket* sock;
    InetAddress* addr;
    Buffer *sendBuffer;
    Buffer *acceptBuffer;
public:
    Client(int msgs, int wait);
    ~Client();
    void oneClient(int msgs, int wait);
};
