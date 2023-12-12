#include "util.h"

using namespace std;

void judge(int num,const string& s){
    if (num==-1) {
        cout<< s <<endl;
    }
}
void judge(int num,const string& s,int standard){
    if (num==standard) {
        cout<< s <<endl;
    }
}
void judge(int num,const string& s,int standard,function<void()> callback){
    if (num==standard) {
        cout<< s <<endl;
        callback();
    }
}



void println(const string& s){
    cout<< s <<endl;
}




