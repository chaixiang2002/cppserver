#pragma once

#include <cstring>
#include <string>
#include <toml.hpp>
#include "squeue.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <utility>

#define DEBUG
#ifdef DEBUG
#define LOG(x) std::cout << "LOG: " << x << std::endl;
#else
#define LOG(x)
#endif

inline std::pair<std::string, int> read_config(std::string user){
    auto config = toml::parse_file( "/home/snow/code/cppserver/demo-fileserver/config/config.toml" );
    std::string ser_ip=config[user]["ip"].value_or("[]");
    int ser_port=config[user]["port"].value_or(0);
    // cout<< ser_ip<<ser_port<<endl;
    LOG(ser_ip);
    LOG(ser_port);
    return {ser_ip,ser_port};
}



inline void misjudgment(int ret,const char *s){
    if (ret==-1) {
        perror(s);
        // cout<< s<<" failed"<<endl;
    }
}

inline void misjudgment(int ret,const char *s,std::function<void ()> fun){
    if (ret==-1) {
        perror(s);
        fun();
        std::cout<< s<<" failed"<<std::endl;
        // exit(0);
    }
}

inline void misjudgment(int ret,int jud,const char *s){
    if (ret==jud) {
        perror(s);
        // cout<< s<<" failed"<<endl;
    }
}


inline void isLegal(int argc, char* argv[]){
    std::cout << "Number of arguments: " << argc << std::endl;
    for(int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
    if (argc<=1) {
        std::cout << "缺少参数，程序已退出！ " << argc << std::endl; 
        exit(-1);
    }
}

inline void print(const char* s){
    std::cout << s << std::endl; 
}

inline bool str_eq(const char* a,const char* b){
    return strcmp(a, b)==0?1:0;
}

// 文件传输
struct file_info{
    char name[256];
    int size;
};

inline struct file_info read_config_which_file(){
    auto config = toml::parse_file( "/home/snow/code/cppserver/demo-fileserver/config/config.toml" );
    file_info a_file_info;
    std::string filename=config["client"]["filename"].value_or("[]");

    strcpy(a_file_info.name, filename.c_str());
    a_file_info.size=config["client"]["filesize"].value_or(0);

    return a_file_info;
}