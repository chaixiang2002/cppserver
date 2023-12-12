#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sched.h>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#include "squeue.h"
#include "util.h"
void fun1(){
    std::cout << "Hello, from demo1!\n";
    while (true) {
        sleep(1);
        std::cout << "damo1 正在运行\n";
    }    
}

void pint(int signum){
    std::cout << signum<<": 收到信号!\n";
}
void fun2(){
    int i=10;
    while (i--) {
        signal(i,pint);
    }
    
    while (true) {
        sleep(1);
        std::cout << "damo1 正在运行\n";
    }
}

void fun3(){
    std::cout<<"mian进程编号："<< getpid()<<'\n';

    int ret=system("/home/snow/code/hello");
    if(ret!=0){
        perror("system error! \n");
    }
        std::cout << "-------------------------";
    ret=execl("/home/snow/code/hello","/home/snow/code/hello",0);
    if(ret!=0){
        perror("execl error! \n");
    }

}
void fun4(){
    int i=2;
    while (i--) {
        int pid=fork();
        std::cout<<"pid:"<< pid<<'\n';

        // std::cout<<"1\n";
        std::cout<<"1"<<'\n';
    }
}

void fun5(){
    signal(SIGCHLD, SIG_IGN);

    if(fork()==0) return;

    int i=1000;
    while (i) {
        sleep(1);
    }
}
void fun6(){
    if(fork()==0){// 父
        int sts;
        pid_t pid=wait(&sts);
        std::cout<<"子："<<pid<<std::endl;
        if (WIFEXITED(sts)) {//正常退出
            std::cout<<"子：正常退出，退出状态："<<WEXITSTATUS(sts)<<std::endl;
            
        }else {// 异常退出
            std::cout<<"子：异常退出,退出信号："<<WTERMSIG(sts)<<std::endl;
        }

    }else {// 子
        // sleep(1000);
        exit(0);
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

void fun7(){
    signal(SIGINT, father_exit);//由Interrupt Key产生，通常是CTRL+C或者DELETE。发送给所有ForeGround Group的进程
    signal(SIGTERM, father_exit);//请求中止进程，kill命令缺省发送

    while (true) {
        if (fork()>0) {// 父
            sleep(5);
        }else {
            signal(SIGINT, child_exit);
            signal(SIGTERM,child_exit);
            while (true) {
                std::cout<<"子：正在运行,sig="<<getpid()<<std::endl;
                sleep(3);
            }
        }
    }
}



void fun8(int argc, char* argv[]){
    isLegal(argc,argv);

    // create share memery
    int shmid=shmget(0x5005,sizeof(Data),0640|IPC_CREAT);//IPC_CREAT代表如果没有就创建
    if (shmid==-1) {
        std::cout<<"shmget(0x5005) failed\n";
    }
        std::cout<<"shmget="<<shmid<<"\n";
    
    // link 
    Data *p=(Data*)shmat(shmid, 0, 0);//后两个参数
    if (p==(Data*)-1) {
        std::cout<<"shmat failed\n";
        return ;
    }
    
    // use
    std::cout<<p->id<<"  |   "<<p->name<<'\n';
    p->id=atoi(argv[1]);
    strcpy(p->name, argv[2]);
    // src 所指向的字符串复制到 dest。
    // 需要注意的是如果目标数组 dest 不够大，而源字符串的长度又太长，可能会造成缓冲溢出的情况
    std::cout<<p->id<<"  |   "<<p->name<<'\n';

    // 分离
    shmdt(p);

    // delete
    if (shmctl(shmid, IPC_RMID, 0)==-1) {
        std::cout<<"shmctl 删除失败\n";
    }
}

void fun9_squeue(){
    using ElemType = int;
    squeue<ElemType, 5> que;

    // que.print_queue();

    ElemType elem;

    elem=1;que.push(elem);
    elem=2;que.push(elem);
    elem=3;que.push(elem);
    elem=4;que.push(elem);

    que.print_queue();
}

void fun10_share_mem_sque(){
    using ElemType = int;
    const int QueMaxLength= 10;
    // create
    int shmid=shmget(0x0101, sizeof(squeue<ElemType,QueMaxLength>), 0640|IPC_CREAT);
    if (shmid==-1) {
        std::cout<< "shmget failed" <<std::endl;
    }

    // link
    squeue<ElemType, QueMaxLength> *shm_sque_ptr=(squeue<ElemType, QueMaxLength> *)shmat(shmid, 0, 0);
    if (shm_sque_ptr==(void*)-1) {
        std::cout<< "shmat failed" <<std::endl;
    }

    // use;
    ElemType elem;

    elem=1;shm_sque_ptr->push(elem);
    elem=2;shm_sque_ptr->push(elem);
    elem=3;shm_sque_ptr->push(elem);
    elem=4;shm_sque_ptr->push(elem);
    shm_sque_ptr->print_queue();
    std::cout<< shm_sque_ptr->front() <<std::endl;
    shm_sque_ptr->pop();
    std::cout<< shm_sque_ptr->front() <<std::endl;
    shm_sque_ptr->pop();
    std::cout<< shm_sque_ptr->size() <<std::endl;

    elem=11;shm_sque_ptr->push(elem);
    elem=12;shm_sque_ptr->push(elem);
    elem=13;shm_sque_ptr->push(elem);
    elem=14;shm_sque_ptr->push(elem);
    shm_sque_ptr->print_queue();
}

void fun11_sem_shm(int argc, char* argv[]){
    isLegal(argc,argv);

    // create share memery
    int shmid=shmget(0x5005,sizeof(Data),0640|IPC_CREAT);//IPC_CREAT代表如果没有就创建
    if (shmid==-1) {
        std::cout<<"shmget(0x5005) failed\n";
        return;
    }
        std::cout<<"shmget="<<shmid<<"\n";
    
    // link 
    Data *p=(Data*)shmat(shmid, 0, 0);//后两个参数
    if (p==(Data*)-1) {
        std::cout<<"shmat failed\n";
        return ;
    }
    
    // use

    //---- crate sem to mutex
    csemp mutex;
    if (mutex.init(0x5005)==false) {
        std::cout<< " csemp mutex init(0x5005) failed\n" <<std::endl;
        return;
    }

    //---- 加锁
    mutex.wait();
    std::cout<<"加锁"<<'\n';

    std::cout<<p->id<<"  |   "<<p->name<<'\n';
    p->id=atoi(argv[1]);
    strcpy(p->name, argv[2]);
    std::cout<<p->id<<"  |   "<<p->name<<'\n';
    sleep(6);
    //---- 解锁
    mutex.post();
    std::cout<<"解锁"<<'\n';


    // 分离
    shmdt(p);

    // delete
    if (shmctl(shmid, IPC_RMID, 0)==-1) {
        std::cout<<"shmctl 删除失败\n";
    }

   
}

int main(int argc, char* argv[]){
    // fun8(argc,argv);
    fun11_sem_shm(argc,argv);
    // std::cout<< " failed" <<std::endl;
    
}
