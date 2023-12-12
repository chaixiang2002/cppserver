#include "include/squeue.h"
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstdio>
#include <sys/ipc.h>
#include <sys/sem.h>

bool csemp::init(key_t key,unsigned short value,short sem_flg){
    if (m_semid!=-1) 
        return false;
    
    
    if ((m_semid=semget(key, 1, 0666))==-1) {// 先看看该信号量，是否已经存在？
        if (errno==ENOENT) {// 不存在，就创建
            if ((m_semid=semget(key, 1, 0666|IPC_CREAT |IPC_EXCL))==-1) {//IPC_CREAT 没有就创建；IPC_EXCL确保只有一个进程创建，其他进程则会变成获取
                if (errno==EEXIST) {// 如果此时存在，就获取
                    if ((m_semid=semget(key, 1, 0666))==-1) {
                        perror("init 1 semget()");
                        return false;
                    }
                    return true;
                }
                else {
                    perror("init 2 semget()");
                    return false;
                }
            }

            // 只有创建该信号量的进程才会运行下面的几行
            union semun sem_union;
            sem_union.val=value;
            if(semctl(m_semid, 0, SETVAL,sem_union)<0){
                perror("init 2 semget()");
                return false;    
            } 
        }
        else {
            perror("init 3 semget()");
        }
    }
    return true;
}

bool csemp::wait(short sem_op){
    if (m_semid==-1) {
        return false;
    }
    struct sembuf sem_b;
    sem_b.sem_num=0;
    sem_b.sem_op=sem_op;
    sem_b.sem_flg=m_sem_flg;
    if (semop(m_semid, &sem_b, 1)==-1) {
        perror("V semop()");
        return false;
    }
    return true;
}
bool csemp::post(short sem_op){
    if (m_semid==-1) {
        return false;
    }
    struct sembuf sem_b;
    sem_b.sem_num=0;
    sem_b.sem_op=sem_op;
    sem_b.sem_flg=m_sem_flg;
    if (semop(m_semid, &sem_b, 1)==-1) {
        perror("V semop()");
        return false;
    }
    return true;
}
int csemp::getvalue(){
    return semctl(m_semid, 0, GETVAL);
}
bool csemp::destroy(){
    if (m_semid==-1) {
        return false;
    }
    if(semctl(m_semid, 0, IPC_RMID)==-1){
        perror("destroy semctl");
    }
    return true;
}