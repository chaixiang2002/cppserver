#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/socket.h>
#include "mysocket.h"
#include "util.h"
using namespace std;


bool recv_file(int sockfd, file_info a_fileinfo){
    ofstream fout;
    fout.open(a_fileinfo.name,ios::binary);
    if (fout.is_open()==false) {
        print("EORROR: fout.is_open() failed");
        return false;
    }

    int filesize=a_fileinfo.size;
    int totalbytes=0;
    int onread=0;
    char buf[4096];

    while (true) {
        memset(&buf, 0, sizeof(buf));
        onread=(filesize-totalbytes)>4096?4096:filesize-totalbytes;
        misjudgment(recv(sockfd, buf, onread, 0),"EORROR: recv file_bytes failed");
        fout.write(buf, onread);

        totalbytes+=onread;
        if (totalbytes==filesize) {
            break;
        }
    }

    LOG("succfully write file")
    return true;
}

void task(int sockfd){
    string buf;
    int i=3,readn,sendn;
    file_info a_fileinfo;
    while (i--) {
        buf.clear();
        // 1. 接受即将传输的文件消息
        readn=recv(sockfd, &a_fileinfo, sizeof(a_fileinfo), 0);
        LOG("1. 接受即将传输的文件消息");
        if (readn==-1) {
            print("EORRER: recv 1 failed");
            return;
        }else if (readn>0) {
            cout<< "msg from lient "<<sockfd<< " : "<<a_fileinfo.name<<endl;
            // 2. 回复已准备好的消息
            buf.clear();
            // buf.resize(1024);
            buf.append("prepare ok").append(a_fileinfo.name);
            sendn=send(sockfd, buf.c_str(), buf.size(), 0);
            LOG("2. 回复已准备好的消息");
            if (sendn==-1) {
                print("EORRER: send 2 failed");
                return;
            }else {
                //3. 开始接受文件...
                LOG("3. 开始接受文件...")
                if (recv_file(sockfd,a_fileinfo)==false) {
                    print("EORRER: recv_file 3 failed");
                    return;
                }else {
                    // 4. 发送 接受文件完毕
                    buf.clear();
                    buf.append("all ok").append(a_fileinfo.name);
                    LOG(buf)
                    sendn=send(sockfd,buf.c_str(),buf.size(),0);
                    LOG("4. 发送 接受文件完毕---------")
                    misjudgment(sendn,"EORRER: send 4 failed");
                                return;

                }
            }
        }

    }
}

void fun(){
    auto [ip,port]=read_config("server");
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    serv_sock._bind(AF_INET, ip.c_str(),port);
    serv_sock._listen(SOMAXCONN);
    shared_ptr<mysocket> clnt_mysock=serv_sock._accpet();

    task(clnt_mysock->getsockfd());
}

int main(int argc,char *argv[]){
    fun();
    return 0;
}