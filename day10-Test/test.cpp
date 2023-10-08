#include "include/Client.h"
// #include <iostream>
#include <unistd.h>
#include <string>
#include <functional>
// #include <basic_string.h>
#include "include/ThreadPool.h"

using std::stoi;

int main(int argc, char *argv[]){
    int threads=100;
    int msgs=100;
    int wait=0;
    
    int o;
    const char *optstring ="t:m:w:";
    /**
     * 使用getopt()函数来解析命令行参数
     * argc表示命令行参数的个数
     * argv是一个指向命令行参数字符串数组的指针
     * optstring是一个包含选项字符的字符串。
     * 
     * optstring被设置为"t:m:w:"，其中每个字符表示一个选项。
     * 冒号表示该选项需要一个参数。具体来说，这里的选项有三个：
     * -t表示线程数，
     * -m表示发送消息数，
     * -w表示等待时间。
    */
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 't':
                threads = stoi(optarg);
                break;
            case 'm':
                msgs = stoi(optarg);
                break;
            case 'w':
                wait = stoi(optarg);
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }

    auto gun=[](int msgs, int wait){
        Client * client=new Client(msgs,wait);
        delete client;
    };

    ThreadPool *pool=new ThreadPool(threads);
    // Client * client=new Client(10000,2);
    std::function<void()> func=std::bind(gun,msgs,wait);
    for (int i = 0; i < threads; i++)
    {
        pool->add(func);
    }
    delete pool;
    return 0;
    // Client * client=new Client(10000,2);
    // return 0;
}