#include "util.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include <memory>
#include <ostream>
#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <utility>
#include "mysocket.h"
using namespace std;

void telltask(int clnt_sockfd){
     while (true)
        {
            char buf[1024];
            bzero(&buf,sizeof(buf));
            ssize_t read_bytes=read(clnt_sockfd,buf,sizeof(buf));// <unistd.h>
            if (read_bytes>0)
            {
                printf("message from client fd %d: %s\n",clnt_sockfd,buf);
                write(clnt_sockfd,buf,sizeof(buf));
            } else if(read_bytes == 0){
                printf("client fd %d disconnected\n", clnt_sockfd);
                close(clnt_sockfd);
                break;
            } else if(read_bytes == -1){
                close(clnt_sockfd);
                // errif(true, "socket read error");
            }
            
        }
}

bool recvfile_(int socket,const string &filename,const size_t filesize){
    ofstream fout;
    fout.open(filename,ios::binary);
    LOG(filename);
    if (fout.is_open()==false) {
        print("fout.is_open() failed");
        return false;
    }

    int onread=0;
    int totalbytes=0;
    char buffer[7];

    while (true) {
        // 预计接受字节数
        if (filesize-totalbytes>7) onread=7;
        else onread=filesize-totalbytes;

        // 接受
        if (recv(socket, &buffer, sizeof(buffer), 0)==-1) {
            print("recv failed");
            return false;
        }
        // 
        fout.write(buffer,onread);
        // 计算
        totalbytes+=onread;

        if (totalbytes==filesize) {
            break;
        }
    }
    return true;
}

void recvfile(int sockfd){

 while (true)
        {
            // 1. 先接受 文件名和大小
            file_info a_fileinfo;
            string buf;
            ssize_t read_bytes=read(sockfd,&a_fileinfo,sizeof(a_fileinfo));// <unistd.h>
            if (read_bytes>0)
            {
                printf("收到消息结构体是：%s(%d)\n",a_fileinfo.name,a_fileinfo.size);

                // 2. 回复确认报文
                buf.clear();
                buf.append("已准备接受文件：").append(a_fileinfo.name);
                write(sockfd,buf.c_str(),buf.size());
                LOG(buf.c_str());

                // 3. 接受文件
                if (recvfile_(sockfd, a_fileinfo.name, a_fileinfo.size)==false) {
                    print("recvfile_ failed");
                    return;
                }

                // 4.接受成功
                print("接受文件完毕");

                // 5. 回复成功确认报文
                buf.clear();
                buf.append("已成功接受文件：").append(a_fileinfo.name);
                write(sockfd,buf.c_str(),buf.size());
                LOG(buf);
            } else if(read_bytes == 0){
                printf("client fd %d disconnected\n", sockfd);
                close(sockfd);
                break;
            } else if(read_bytes == -1){
                close(sockfd);
                // errif(true, "socket read error");
            }
            
        }
    

    

}
void fun(int argc,char *argv[]){
    // read config
    auto[ip,port]=read_config("server");
    
    // create sock
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    // bind sock and adrr
    serv_sock._bind(AF_INET, ip.c_str(), port);
    // sock start listen... 
    serv_sock._listen(SOMAXCONN);
    // accpet a new sock
    shared_ptr<mysocket> clnt_mysock=serv_sock._accpet();
    // callback

    //  Write business logic task;
    std::function<void (int)> callback_fun=[](int clnt_sockfd){
       recvfile(clnt_sockfd);
    };
    // Assign task
    clnt_mysock->set_callback_fun(callback_fun);
    // run task
    clnt_mysock->fun(clnt_mysock->getsockfd());
}

int main(int argc,char *argv[]){
    // if (argc!=2) {
    //     print("参数不合法");
    // }
    
    fun(argc,argv);
    return 0;
}