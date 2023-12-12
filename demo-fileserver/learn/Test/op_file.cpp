#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "util.h"
using namespace std;

void fun1_w(const char * s){
    int fd;
    fd=open("data.txt",O_CREAT|O_RDWR|O_TRUNC);//O_CREAT 创建文件，O_RDWR 权限：可读可写 ，O_TRUNC 是否截断文件
    misjudgment(fd, "open");

    printf("fd: %d \n",fd);

    char buf[1024];
    strcpy(buf, s);

    misjudgment(write(fd, buf, strlen(buf)), "write（）");

    sleep(100);


    close(fd);
}

void fun2_r(){
    int fd=open("data.txt", O_RDONLY);
    misjudgment(fd,"open(data.txt)");

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    misjudgment(read(fd, buf, sizeof(buf)),"read()");

    print(buf);
    close(fd);
}


void fun3_file(int num){
    print("this is 3");
    int i;
    cin>>i;
    cout<<"cout"<< i <<endl;
    cerr<<"cerr"<< i <<endl;

    close(num);// 0 是键盘输入  1 标准输出 2 错误输出

    cin>>i;
    cout<<"cout"<< i <<endl;
    cerr<<"cerr"<< i <<endl;
}

void to_fun(int argc,char *argv[]);
int main(int argc,char *argv[]){
    to_fun( argc,argv);
    // fun();
    return 0;
}
void to_fun(int argc,char *argv[]){
    if (argc>1 && str_eq(argv[1], "w")) {
        fun1_w(argv[2]);
    }
    if (argc>1 && str_eq(argv[1], "r")) {
        fun2_r();
    }
    if (argc>1 && str_eq(argv[1], "3")) {
        fun3_file(atoi(argv[2]));
    }
}
