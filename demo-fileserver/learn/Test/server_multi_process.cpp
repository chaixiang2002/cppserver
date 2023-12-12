#include <csignal>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>
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
   while (true)
    {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes=read(sockfd,buf,sizeof(buf));// <unistd.h>
        if (read_bytes>0)
        {
            printf("message from client fd %d: %s\n",sockfd,buf);
            write(sockfd,buf,sizeof(buf));
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


void father_exit(int sig){
    //
    signal(SIGINT, SIG_IGN);//由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
    signal(SIGTERM, SIG_IGN);//请求中止进程，kill命令缺省发送
    std::cout<<"父：退出,sig="<<getpid()<<std::endl;
    kill(0, SIGTERM);// 向同一组的所有进程发送15，
    exit(0);
}

void child_exit(int sig){
    //
    signal(SIGINT, SIG_IGN);//由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
    signal(SIGTERM, SIG_IGN);//请求中止进程，kill命令缺省发送
    std::cout<<"子：退出,sig="<<getpid()<<std::endl;
    exit(0);
}
void fun(){
    // ignore all signal
    for (int i=0; i<=64; i++) {
        signal(i, SIG_IGN);
    }

    signal(SIGINT, father_exit);//由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
    signal(SIGTERM, father_exit);//请求中止进程，kill命令缺省发送


    auto [ip,port]=read_config("server");
    mysocket serv_sock(AF_INET,SOCK_STREAM);
    serv_sock._bind(AF_INET, ip.c_str(),port);
    serv_sock._listen(SOMAXCONN);

    while (true) {
        shared_ptr<mysocket> clnt_mysock=serv_sock._accpet();
        LOG("建立新连接")
        int pid=fork();
        if (pid==-1) {
            perror("accpet()");// 系统资源不足
            return;
        }
        if (pid>0) {// 父进程
            close(clnt_mysock->getsockfd());// 父进程接受之后不用管客户端sock
            continue;
        }
        // 后面都是子进程
        close(serv_sock.getsockfd());// 子进程不用管服务器端sock

        signal(SIGTERM, child_exit);
        signal(SIGINT, SIG_IGN);
        task(clnt_mysock->getsockfd());
        return;//记得 子进程结束后关闭，不要再循环


        // int pid=fork();
        // if (pid==-1) {
        //     perror("accpet()");// 系统资源不足
        //     return;
        // }

        // if (pid>0) { // 父进程
        //     sleep(2);
        //     continue; 
        // }else {// 子进程
        //     signal(SIGTERM, child_exit);
        //     signal(SIGINT, SIG_IGN);

        // }
        // shared_ptr<mysocket> clnt_mysock=serv_sock._accpet();
        // task(clnt_mysock->getsockfd());
    }

}

int main(int argc,char *argv[]){
    fun();
    return 0;
}