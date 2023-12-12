#pragma once

#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <functional>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <filesystem>
#include "toml.hpp"

#define DEBUG
#ifdef DEBUG
#define LOG(x) std::cout << "LOG: " << x << std::endl;
#else
#define LOG(x)
#endif

void judge(int num,const std::string& s);
void judge(int num,const std::string& s,int standard);
void judge(int num,const std::string& s,int standard,std::function<void()> callback);

void println(const std::string& s);

template<class T>
T read_config(const std::string& table,const std::string& key){
    auto config = toml::parse_file( "/home/snow/code/netserver/config/config.toml" );
    T res=config[table][key].value_or(T());
    LOG(res)
    return res;
}

inline void sock_judge(int fd,int num,const std::string& s){
    if (num==-1) {
        perror(s.c_str());
        close(fd);
    }
}



// http------------------------

// 搜索某个文件是否存在
inline bool searchFile(const std::string& filePath, const std::string& targetFile) {
    std::filesystem::path directoryPath(filePath);

    // 检查文件是否存在
    if (std::filesystem::exists(directoryPath)) {
        // 遍历目录下的文件和子目录
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            // 检查是否为目标文件
            std::string filename = entry.path().filename().string();
            if (filename == targetFile || filename == std::filesystem::path(targetFile).filename()) {
                return true;  // 目标文件存在
            }
        }
    }

    return false;  // 目标文件不存在
}

inline std::string get_time(){
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::gmtime(&t);

    // 创建一个输出字符串流
    std::ostringstream oss;

    // 将格式化的时间写入字符串流
    oss << std::put_time(tm, "Date: %a, %d %b %Y %T GMT\r\n");

    // 将字符串流转换为字符串
    return oss.str();
}