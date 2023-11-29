#include "Buffer.h"
#include "iostream"
// Buffer::Buffer()
// {
// }

// Buffer::~Buffer()
// {
// }

void Buffer::setBuf(const char* _str){
    buf.clear();
    buf.append(_str);
}
void Buffer::append(const char* _str,int _size){
    for (int i = 0; i < _size; i++)
    {
        if (_str[i]=='\0') break;
        buf.push_back(_str[i]);
    }
}
ssize_t Buffer::size(){
    return buf.size();
}
const char* Buffer::ToStr(){
    return buf.c_str();
}
void Buffer::clear(){
    buf.clear();
}
void Buffer::getline(){
    buf.clear();
    while (buf.empty())
    {
        std::getline(std::cin, buf);
    }
}
