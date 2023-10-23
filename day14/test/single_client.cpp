#include "Connection.h"
#include "Socket.h"
#include <iostream>

int main(){
    Socket *sock=new Socket;
    sock->connect("127.0.0.1",1234);
    Connection *conn=new Connection(nullptr,sock);
    
    while (true) {
        conn->GetlineSendBuffer();
        conn->Write();
        if (conn->GetState()==Connection::State::Closed) {
            conn->close();
            // delete conn;
            break;
        }
        conn->Read();
        std::cout<<"Message from server:"<<conn->ReadBuffer() <<std::endl;
    }

    delete conn;
    // delete sock;
    return 0;
}