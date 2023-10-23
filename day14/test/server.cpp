#include "Connection.h"
#include "EventLoop.h"
#include "Server.h"
#include <iostream>
#include <ostream>
#include "Socket.h"

int main(){
    EventLoop *loop = new EventLoop();
    Server *server=new Server(loop);
    server->OnConnect([](Connection *conn){
        conn->Read();
        if (conn->GetState()==Connection::State::Closed) {
            conn->close();
            return ;
        }
        std::cout<<"Msg from client "<<conn->GetSocket()->getFd()<<": "<<conn->ReadBuffer()<<std::endl;
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });

    loop->loop();
    delete server;
    delete loop;
    return 0;
}