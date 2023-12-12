#include <iostream>
#include <string>
#include "util.h"
#include "http.h"
#include "config.h"
using namespace std;

void fun(){
    println("hello");
    judge(-1,"asdas");
    LOG(read_config<string>("server", "ip"));
    LOG(read_config<int>("server", "port"));

    LOG(__func__)
    LOG(__FUNCTION__)
    const char* response = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 12\r\n\r\n"
                            "";

    string s="";
    s=s+response+read_file_contents("/home/snow/code/netserver/resource/index.html");
    println(s);
}

int main(int argc,char *argv[]){
    // fun();
    string request_info="asd.html";
    LOG(request_info)
    if(!searchFile(RESOURCE_directoryPath, request_info)){
        request_info="error.html";
    }
    LOG(request_info)

    LOG( searchFile("/home/snow/code/netserver/resource","error.html"));
    LOG( searchFile("/home/snow/code/netserver/resource","/error.html"));
    LOG( searchFile("/home/snow/code/netserver/resource","/asderror.html"));

    return 0;
}