//
// Created by m7mds on 19/01/2022.
//
#include <unistd.h>

void* smalloc(size_t size){
        if(size==0 || size > 100000000){
            return nullptr;
        }
        void* res = sbrk(size);
        if(res == (void*) -1 ){
            return nullptr;
        }
        return res;
    }
