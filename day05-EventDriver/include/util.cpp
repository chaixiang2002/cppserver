#include "util.h"
#include "stdlib.h"
#include "stdio.h"

void errif(bool condition,const char *errmsg){
    if(condition){
        perror("errmsg");
        exit(EXIT_FAILURE);
    }
}
