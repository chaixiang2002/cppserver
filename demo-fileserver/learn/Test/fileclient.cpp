#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "mysocket.h"
#include "util.h"
using namespace std;


bool send_file(int sockfd, file_info a_fileinfo){
    ifstream fin;
    fin.open(a_fileinfo.name,ios::binary);
    if (fin.is_open()==false) {
        print("EORROR: fin.is_open() failed");
        return false;
    }

    int filesize=a_fileinfo.size;
    int totalbytes=0;
    int onread=0;
    char buf[4096];

    while (true) {
        memset(&buf, 0, sizeof(buf));
        onread=(filesize-totalbytes)>4096?4096:filesize-totalbytes;
        fin.read(buf, onread);
        misjudgment(send(sockfd, buf, onread, 0),"EORROR: recv file_bytes failed");
        
        totalbytes+=onread;
        if (totalbytes==filesize) {
            break;
        }
    }

    LOG("succfully send file")
    return true;
}

void task(int sockfd){
    string buf;
    int i=3,readn,sendn;
    file_info a_fileinfo=read_config_which_file();
    while (i--) {
        buf.clear();
        // 1. 发送即将传输的文件消息
        sendn=send(sockfd, &a_fileinfo, sizeof(a_fileinfo), 0);
        LOG(a_fileinfo.name)
        LOG("1. 发送即将传输的文件消息");
        if (sendn==false) {
            print("ERROR: send 1 failed");
            return;
        }else {
            // 2. 接受服务端的准备消息
            buf.clear();
            buf.resize(1024);
            LOG("2. 接受服务端的准备消息")
            // readn=recv(sockfd, buf.data(), sizeof(buf.c_str()),0);
            readn=recv(sockfd, buf.data(), buf.size(),0);
            if (readn==-1) {
                print("ERROR: read 2 failed");
                return;
            }else if (readn>0) {
                LOG(buf);
                string tmp;
                tmp.append("prepare ok").append(a_fileinfo.name);
                tmp.resize(1024);
                if (buf!=tmp) {
                    LOG(buf)
                    LOG(tmp)
                    LOG(buf.size())
                    LOG(tmp.size())
                    print("ERROR: read 2 failed -- buf!=tmp");
                    return; 
                }else {
                    // 3. 发送文件
                    LOG("3. 发送文件")
                    if (send_file(sockfd, a_fileinfo)==false) {
                        print("ERROR: send_file 3 failed");
                        return; 
                    }else {
                        // 4. 等待服务端全部完成消息
                        LOG("4. 等待服务端全部完成消息")
                        buf.clear();
                        buf.resize(1024);
                        readn=recv(sockfd, buf.data(), buf.size(),0);
                        if (readn==-1) {
                            print("ERROR: read 4 failed");
                            return; 
                        }else if(readn>0){
                            tmp.clear();
                            tmp.append("all ok").append(a_fileinfo.name);
                            tmp.resize(1024);
                            if (buf!=tmp) {
                                LOG(buf)
                                LOG(tmp)
                                LOG(buf.size())
                                LOG(tmp.size())
                                print("ERROR: read 4 failed----buf!=tmp");
                                return; 
                            }else {
                                LOG("ALL  is Over !!!-------")
                                return;
                            }
                        }else{
                            printf("2 readn: %d\n",readn);
                            return;
                        }
                    }
                }
            }else{
                printf("1 readn: %d\n",readn);
                return;
            }
        }

    }
}

void fun(){
    auto [ip,port]=read_config("client");
    mysocket sock(AF_INET,SOCK_STREAM);
    sock._connect(AF_INET, ip.c_str(), port);

    task(sock.getsockfd());
}

int main(int argc,char *argv[]){
    fun();
    return 0;
}