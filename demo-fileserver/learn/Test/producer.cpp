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
using namespace std;

int fun(){
    // cout<< " failed" <<endl;
    using ElemType=Data;
    const int QueMaxLength= 10;

    // init shm
    int shmid=shmget(0x1001, sizeof(squeue<ElemType, QueMaxLength>),0640|IPC_CREAT);
    if (shmid==-1) {
        cout<< "shmget(0x1001) failed" <<endl;
        return -1;
    }

    //link shm
    squeue<ElemType, QueMaxLength> *que_ptr=(squeue<ElemType, QueMaxLength> *)shmat(shmid, 0, 0);
    if (que_ptr==(void *)-1) {
        cout<< "shmat failed" <<endl;
    }
    que_ptr->init();

    // use
    ElemType ee;

    csemp mutex;mutex.init(0x1001); // 信号量做锁
    csemp cond;cond.init(0x1002,0,0); // 信号量做生产消费模型

    que_ptr->print_queue();
    mutex.wait();
    ee.set(1, "1"); que_ptr->push(ee);
    ee.set(2, "2"); que_ptr->push(ee);
    ee.set(3, "3"); que_ptr->push(ee);
    ee.set(4, "4"); que_ptr->push(ee);
    que_ptr->print_queue();
    mutex.post();
    cond.post(3);//加3

    // depth
    shmdt(que_ptr);

    // delete
    // if (shmctl(shmid, IPC_RMID, 0)==-1) {
    //     std::cout<<"shmctl 删除失败\n";
    // }

    return 0;
}

int main(){
    fun();
    return 0;
}
