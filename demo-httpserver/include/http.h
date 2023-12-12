#pragma once

#include <string>
#include <iostream>
#include <ostream>
#include <filesystem>
#include <map>
#include <fstream>
#include <string>
#include "config.h"
#include "util.h"

inline std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;

        file.close();

        return read_file_contents(ERROR_Page);
    }

    std::string contents;
    std::string line;
    while (std::getline(file, line)) {
        contents += line + "\n";
    }

    file.close();
    return contents;
}

// 解析请求报文，提取请求方法、URL和其他请求头信息
inline std::map<std::string, std::string> parse_request(const std::string& request) {
    std::map<std::string, std::string> request_info;

    // 提取请求方法和URL
    size_t method_end = request.find(' ');
    if (method_end != std::string::npos) {
        request_info["Method"] = request.substr(0, method_end);

        size_t url_end = request.find(' ', method_end + 1);
        if (url_end != std::string::npos) {
            request_info["URL"] = request.substr(method_end + 1, url_end - method_end - 1);
        }
    }

    // 提取其他请求头信息
    size_t pos = request.find("\r\n");
    while (pos != std::string::npos) {
        size_t colon_pos = request.find(':', pos);
        if (colon_pos != std::string::npos) {
            std::string key = request.substr(pos + 2, colon_pos - pos - 2);
            size_t value_start = colon_pos + 2;
            size_t value_end = request.find("\r\n", value_start);
            std::string value = request.substr(value_start, value_end - value_start);
            request_info[key] = value;
        }
        pos = request.find("\r\n", pos + 2);
    }

    return request_info;
}


// 读取文件内容
inline std::string readFile(const std::string& filePath) { 
    // std::string filePath=HTML_directoryPath+filePath2;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        // throw std::runtime_error("Failed to open file: " + filePath);
        return readFile(ERROR_Flie);
    }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());

    return content;
}

//---------------

// 处理客户端请求

inline std::string handleRequest(const std::string& request) {
    // 提取请求路径，根据需要做相应的处理
    // 解析
    std::map<std::string, std::string> request_info = parse_request(request);   
    // 输出请求方法和URL
    std::cout << "Method: " << request_info["Method"] << std::endl;
    std::cout << "URL: " << request_info["URL"] << std::endl;

    // 输出其他请求头信息
    for (const auto& pair : request_info) {
        if (pair.first != "Method" && pair.first != "URL") {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
    // // 404错误
    // if(!searchFile(HTML_directoryPath, request_info["URL"])){
    //     request_info["URL"]="error.html";
    // }

    std::string fileContent="";
    std::string HTTP1_1="HTTP/1.1 200 OK\r\n";
    std::string A_C_A_O="";//"Access-Control-Allow-Origin: *\r\n";
    std::string DATE=get_time();
    std::string Connection_="";//"Connection: keep-alive\r\n";
    std::string Content_Type="Content-Type: ";//
    std::string Content_Length="Content-Length: ";//
    std::string Keep_Alive="";//"Keep-Alive: timeout=5\r\n";

    std::string file_contents="";//
    if (request_info["Accept"]==request_img) {//图片
        file_contents=readFile(HTML_directoryPath+request_info["URL"]);
        Content_Type+="image/webp,image/apng,image/svg+xml,image/*\r\n";

            // file_contents+=Empty_Line;
        Content_Length=Content_Length+std::to_string(file_contents.size())+"\r\n";
        fileContent=HTTP1_1+A_C_A_O+DATE+Connection_+Content_Type+Keep_Alive + "\r\n\r\n"+file_contents;
        LOG("模拟返回图片文件")
    }else if (request_info["Accept"]==request_html) {//html
        file_contents=read_file_contents(HTML_directoryPath+request_info["URL"]);
        Content_Type+="text/html\r\n";

        file_contents+=Empty_Line;
        Content_Length=Content_Length+std::to_string(file_contents.size())+"\r\n";
        fileContent=HTTP1_1+A_C_A_O+DATE+Connection_+Content_Type+Content_Length+Keep_Alive + "\r\n\r\n"+file_contents;
        LOG("模拟返回html")
    }else if (request_info["Accept"]==request_all) {
        file_contents=read_file_contents(HTML_directoryPath+request_info["URL"]);
        Content_Type+="text/html\r\n";

        file_contents+=Empty_Line;
        Content_Length=Content_Length+std::to_string(file_contents.size())+"\r\n";
        fileContent=HTTP1_1+A_C_A_O+DATE+Connection_+Content_Type+Content_Length+Keep_Alive + "\r\n\r\n"+file_contents;
        LOG("模拟返回html")
    }else {
        LOG("其他！！！！！！！！！！！！！！！！！！！！")
    }
    file_contents+=Empty_Line;
    Content_Length=Content_Length+std::to_string(file_contents.size())+"\r\n";
    fileContent=HTTP1_1+A_C_A_O+DATE+Connection_+Content_Type+Content_Length+Keep_Alive + "\r\n\r\n"+file_contents;

    return fileContent;
}