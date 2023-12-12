#include "mysocket.h"
#include "util.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <string>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <utility>
using namespace std;

void tell_task(int sockfd){
    while (true)
        {
            char buf[1024];  //在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
            bzero(&buf, sizeof(buf));
            scanf("%s", buf);
            ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
            if(write_bytes == -1){
                printf("socket already disconnected, can't write any more!\n");
                break;
            }
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if(read_bytes > 0){
                printf("message from server: %s\n", buf);
            }else if(read_bytes == 0){
                printf("server socket disconnected!\n");
                break;
            }else if(read_bytes == -1){
                close(sockfd);
                print("socket read error");
            }
        }
}


bool sendfile(int sockfd,const string &filename,const size_t filesize){
    // 以2进制打开文件
    
    ifstream fin(filename,ios::binary);
    if (fin.is_open()==false) {
        print("fin.is_open() failed");
        return false;
    }

    int onread=0;   // 每次调用fin。read打算读的字节数
    int totalbytes=0; // 已经读了的字节数
    char buffer[7]; // 

    while (true) {
        memset(&buffer, 0, sizeof(buffer));
        // 计算该次要运多少
        if(filesize-totalbytes>7) onread=7;
        else    onread=filesize-totalbytes;
        // 读取数据
        fin.read(buffer,onread);

        // 发送
        if (send(sockfd, buffer, sizeof(buffer), 0)==-1) {
            return false;
        }

        totalbytes+=onread;

        if (totalbytes==filesize) {
            break;
        }

    }
    return true;
}

void transfile(int sockfd,int argc,char *argv[]){
    // int i=3;
    // while (i--) {
    //     // 1. 先发送 文件名和大小
    //     file_info a_fileinfo;
    //     memset(&a_fileinfo, 0, sizeof(a_fileinfo));
    //     strcpy(a_fileinfo.name,argv[1]);
    //     a_fileinfo.size=atoi( argv[2]);

    //     if(send(sockfd, (void*)&a_fileinfo, sizeof(a_fileinfo), 0)==-1)
    //         return;
    //     LOG("发送消息结构体");

    // }

    // while (true)
    // {   
        LOG("先发送 文件名和大小");
        // 1. 先发送 文件名和大小
        file_info a_fileinfo;
        memset(&a_fileinfo, 0, sizeof(a_fileinfo));
        strcpy(a_fileinfo.name,argv[1]);
        a_fileinfo.size=atoi( argv[2]);

        if(send(sockfd, (void*)&a_fileinfo, sizeof(a_fileinfo), 0)==-1)
            return;
        LOG("发送消息结构体");

        string buf(1024,0);
        ssize_t read_bytes = read(sockfd, buf.data(), buf.size());
        if(read_bytes > 0){
            printf("message from server: %s\n", buf.c_str());
        
            LOG("接受服务端的确认包");   
            // 2. 接受服务端的确认包
            string buf_tmp(1024,0);
            buf_tmp.clear();
            buf_tmp.append("已准备接受文件：").append(a_fileinfo.name);
            buf.resize(1024);
            buf_tmp.resize(1024);

            if (buf==buf_tmp) {

                LOG("发送文件");   
                // 3. 发送文件
                if (sendfile(sockfd, a_fileinfo.name, a_fileinfo.size)) {
                    print("recvfile_ failed");
                    return;
                }

                LOG("确认服务器是否收到文件");   
                // 4. 确认服务器是否收到文件
                if (read(sockfd, buf.data(), buf.size())==-1) {
                    print("recvfile_ failed");
                    return;
                }
                buf_tmp.clear();
                buf_tmp.append("已成功接受文件：").append(a_fileinfo.name);
                buf.resize(1024);
                buf_tmp.resize(1024);
                if(buf_tmp!=buf){
                    print("没有收到文件成功接受的消息");
                    return;
                }
                print("全部成功");
            }else {
                print("buf!=buf_tmp");
                LOG(buf);
                LOG(buf_tmp);
            }
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
// --------------------            // break;
        }else if(read_bytes == -1){
            close(sockfd);
            print("socket read error");
        }

        sleep(2);
    // }

}
void fun(int argc,char *argv[]){
    // read config
    auto[ip,port]=read_config("client");

    //  create sock
    mysocket clnt_sock(AF_INET,SOCK_STREAM);

    //  sock connect  
    clnt_sock._connect(AF_INET,ip.c_str(), port);

    //  Write business logic task;
    std::function<void(int)> clnt_task=[argc, argv](int sockfd){
        tell_task(sockfd);
        // transfile(sockfd,argc,argv);
    };

    // Assign task
    clnt_sock.set_callback_fun(clnt_task);
    // run task
    clnt_sock.fun(clnt_sock.getsockfd());
}





int main(int argc,char *argv[]){
    // if (argc!=2) {
    //     print("参数不合法");
    // }
    
    fun(argc,argv);
    return 0;
}