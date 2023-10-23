#pragma once
#include "Macros.h"
#include <string>

class Buffer
{
private:
    std::string buf;
public:
    Buffer()=default;
    ~Buffer()=default;
    DISALLOW_COPY_AND_MOVE(Buffer);
    void append(const char* _str,int _size);
    ssize_t size();
    // const char* c_str();
    const char* ToStr();
    void clear();
    void getline();
    void setBuf(const char* _str);
};


