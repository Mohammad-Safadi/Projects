//
// Created by m7mds on 19/01/2022.
// BASIC MALLOC

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <math.h>


struct MallocMetadata{
    size_t size;
    bool is_free;
    MallocMetadata* next;
    MallocMetadata* prev;
};



void* smalloc(size_t size) ;

void* scalloc(size_t num, size_t size);

void sfree(void* p);

void* srealloc(void* oldp, size_t size);

size_t _num_free_blocks();

size_t _num_free_bytes();

size_t _num_allocated_blocks();

size_t _num_allocated_bytes();
size_t _num_meta_data_bytes();
size_t _size_meta_data();

MallocMetadata* list_begin = nullptr;//first metadata structure
MallocMetadata* list_end = nullptr;
MallocMetadata* iterator = nullptr;


void* smalloc(size_t size) {
    auto max=pow(10,8);
    if (size == 0 || size > max) {  
		      return nullptr;
    }
    if (list_begin == nullptr) {
        void *pointer;
        pointer = sbrk(size + sizeof(MallocMetadata));
        if (pointer == (void *) -1) {
            return nullptr;
        }
        MallocMetadata *metadata = (MallocMetadata *) pointer;
        metadata->size = size;
        metadata->is_free = false;
        metadata->next = nullptr;
        metadata->prev = nullptr;

        list_begin = metadata;
        list_end = metadata;
        return (void *) (((unsigned char *) pointer) + sizeof(MallocMetadata));

    } else {
        // list_begin != nullptr
        iterator = list_begin;
        while (iterator != nullptr) {
            if (iterator->is_free) {
                if (iterator->size >= size) {
                    iterator->is_free = false;
                    return (void *) (((unsigned char *) iterator) + sizeof(MallocMetadata));
                }
            }
            iterator = iterator->next;
        }
        // if we got here then there are no free blocks==>sbrk
        void *pointer = sbrk(size + sizeof(MallocMetadata));
        if (pointer == (void *) -1) {
            return nullptr;
        }

        MallocMetadata *ptr = (MallocMetadata *) pointer;

        ptr->size = size;
        ptr->is_free = false;
        ptr->next = nullptr;
        ptr->prev = list_end;
        list_end->next = ptr;
        list_end = ptr;
        return (void *) (((unsigned char *) pointer) +
                         sizeof(MallocMetadata));//return pointer to first block excluding metadata blocks
    }
}

void* scalloc(size_t num, size_t size){
    void* allocated = smalloc(num * size);
    //return values are the same as the smalloc's return val
    if(allocated == nullptr){
        return nullptr;
    }
    std::memset(allocated,0,size*num);

    return (void*)allocated;
}

void sfree(void* p){
    if( p == nullptr){
        return;
    }
    // p points to the actual block not to his meta data
    /*
    MallocMetadata* block = (MallocMetadata*)p;
    if( block->is_free){
        return;
    }
     */
    ((MallocMetadata*)((unsigned char *)p - sizeof(MallocMetadata)))->is_free=true;
}

void* srealloc(void* oldp, size_t size){
    auto max=pow(10,8);
    if (size == 0 || size > max) {
		return nullptr;
    }
    if( oldp != nullptr){
        MallocMetadata* OldPtrToMeta=(MallocMetadata*)(((unsigned char*)oldp) - sizeof(MallocMetadata));
        if( OldPtrToMeta->size >= size){
            return oldp;
        }else{

            void* PointerToFirst= smalloc(size);
            if(PointerToFirst == (void*)-1){
                return nullptr;
            }else{
                std::memmove(PointerToFirst,oldp,size);
                sfree(oldp);
                return PointerToFirst;
            }
        }
    }else{
        return smalloc(size);
    }
}

size_t _num_free_blocks(){
    MallocMetadata* counter = list_begin;
    size_t freeBlocksNum=0;
    while( counter != nullptr){
        if (counter->is_free){
          freeBlocksNum++;
        }
        counter=counter->next;
    }
    return freeBlocksNum;
}

size_t _num_free_bytes(){
    MallocMetadata* counter = list_begin;
    size_t freeBytesNum=0;
    while( counter != nullptr){
        if (counter->is_free){
            freeBytesNum += counter->size;
        }
        counter=counter->next;
    }
    return freeBytesNum;
}

size_t _num_allocated_blocks(){
    MallocMetadata* counter = list_begin;
    size_t Allocated=0;
    while( counter != nullptr){
        Allocated++;
        counter=counter->next;
    }
    return Allocated;
}
size_t _num_allocated_bytes(){
    MallocMetadata* counter = list_begin;
    size_t Allocated=0;
    while( counter != nullptr){
        Allocated += counter->size;
        counter = counter->next;
    }
    return Allocated;
}
size_t _num_meta_data_bytes(){
    size_t num = _num_allocated_blocks();
    return sizeof(MallocMetadata) * num;
}

size_t _size_meta_data(){
    return sizeof(MallocMetadata);
}




