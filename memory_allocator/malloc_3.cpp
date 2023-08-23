//
// Created by m7mds on 19/01/2022.
//


//
// Created by m7mds on 19/01/2022.
// BASIC MALLOC

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#define KB 1024
#define HIST_SIZE 128
#define MAX_ALLOCATION 10e7
#define MMAP_ALLOCATION_LIMIT (HIST_SIZE*KB)



struct MallocMetadata{
    size_t size;
    bool is_free;
    MallocMetadata* next = nullptr;
    MallocMetadata* prev = nullptr;
    MallocMetadata* next_free = nullptr;
    MallocMetadata* prev_free = nullptr;
};

MallocMetadata* hist[HIST_SIZE]={nullptr};
// i*KB <= hist[i] < (i+1)*KB

//first metadata structure
//ascending order
MallocMetadata* list_begin = nullptr;
MallocMetadata* list_end = nullptr;

MallocMetadata* mmap_head= nullptr;
MallocMetadata* mmap_end= nullptr;

// FREE LIST
void insertToList(MallocMetadata* ToAdd,int index)
{
    if(!ToAdd->is_free){// we only proceed if the block is free
        return;
    }
    index= (index > HIST_SIZE-1) ? HIST_SIZE-1 :index;
    if(hist[index]==nullptr){
        ToAdd->is_free= true;
        ToAdd->prev_free= nullptr;
        ToAdd->next_free= nullptr;
        hist[index]=ToAdd;
        return;
    }

    auto iterator1 = hist[index];
    while(iterator1 != nullptr){
        if(iterator1->size > ToAdd->size || (iterator1->size == ToAdd->size && (char*)ToAdd < (char*)iterator1)){
            break;
        }
        iterator1 = iterator1 ->next_free;
    }

    if(iterator1 == nullptr){ // should be the last element in the list
        auto previous = hist[index];
        while(previous->next_free != nullptr){
            previous=previous->next_free;
        }
        previous->next_free=ToAdd;
        ToAdd->prev_free = previous;
        ToAdd->next_free= nullptr;
        return;
    }

    if(iterator1->prev_free == nullptr){
        ToAdd->next_free=iterator1;
        iterator1->prev_free=ToAdd;
        ToAdd->prev_free= nullptr;
        hist[index]=ToAdd;
        return;
    }

//    if(iterator1->next_free == nullptr){
//        iterator1->next_free=ToAdd;
//        ToAdd->prev_free=iterator1;
//        ToAdd->next_free= nullptr;
//        return;
//    }


    MallocMetadata* prev=iterator1->prev_free;
    prev->next_free=ToAdd;
    ToAdd->next_free=iterator1;
    iterator1->prev_free=ToAdd;
    ToAdd->prev_free=prev;
}

void remove(MallocMetadata* ToRemove, int index) {
    if(list_begin == nullptr ||  hist[index] == nullptr || !ToRemove->is_free){
        return;
    }
    index = (index >= HIST_SIZE)? HIST_SIZE :index;
    ToRemove->is_free=false;
    if (ToRemove->next_free == nullptr && ToRemove->prev_free == nullptr) {
        hist[index]=nullptr;
        ToRemove->next_free= nullptr;
        ToRemove->prev_free = nullptr;
        return;
    }
    else {
        if (ToRemove->next_free && ToRemove->prev_free) {
            ToRemove->prev_free->next_free = ToRemove->next_free;
            ToRemove->next_free->prev_free = ToRemove->prev_free;
            ToRemove->next_free= nullptr;
            ToRemove->prev_free = nullptr;
            return;

        } else {
            if (ToRemove->next_free)//prev = null// Toremove is the first in the list
            {
                hist[index]= ToRemove->next_free;
                ToRemove->next_free->prev_free= nullptr;

                ToRemove->prev_free = nullptr;
                ToRemove->next_free= nullptr;
                return;
            } else {//next = null
                ToRemove->prev_free->next_free= nullptr;
                ToRemove->next_free = nullptr;
                ToRemove->prev_free= nullptr;
            }
        }
    }
}
// FREE LIST


void splitmmap(MallocMetadata* MetaToSplit, size_t size) {
    MallocMetadata *new_metadata = (MallocMetadata *) ((size_t) MetaToSplit + size + sizeof(MallocMetadata));
    new_metadata->size = MetaToSplit->size - size - sizeof(MallocMetadata);
    new_metadata->is_free = true;
    new_metadata->prev = MetaToSplit;
    new_metadata->next = MetaToSplit->next;
    if (MetaToSplit->next != nullptr) {
        MetaToSplit->next->prev = new_metadata;
    }
    MetaToSplit->next = new_metadata;
    MetaToSplit->size = size;
    if(MetaToSplit==list_end){
        list_end=new_metadata;
    }
}
void combinemmap(MallocMetadata* first,MallocMetadata* second){
    first->size += second->size ;
    first->size += sizeof(MallocMetadata);
    first->next = second->next;
    if(second->next != nullptr){
        // linking the two nodes
        second->next->prev = first;
    }
    if(second == list_end){
        list_end = first;
    }// now the first one if the edge
    first->is_free= true;
}

//combine two free blocks to one big free block
// first && second must be free blocks
void combine(MallocMetadata* first,MallocMetadata* second){
    if(first->is_free) {
        remove(first, first->size / KB);
    }
    if(second->is_free) {
        remove(second, second->size / KB);
    }
    first->size += second->size ;
    first->size += sizeof(MallocMetadata);
    first->next = second->next;
    if(second->next != nullptr){
        // linking the two nodes
        second->next->prev = first;
    }
    if(second == list_end){//==> now the first one is the edge
        list_end = first;
    }
    first->is_free= true;
    insertToList(first,first->size/KB);
}



void split(MallocMetadata* MetaToSplit, size_t size) {
    if(MetaToSplit->size - sizeof (MallocMetadata) - size < 128){// we should check this before calling split
        return;
    }
    if(MetaToSplit->is_free) {
        remove(MetaToSplit, MetaToSplit->size / KB);
        MetaToSplit->is_free= true;
    }
    MallocMetadata *new_metadata = (MallocMetadata *) ((size_t) MetaToSplit + size + sizeof(MallocMetadata));
    //new free block fields modifying
    new_metadata->size = MetaToSplit->size - size - sizeof(MallocMetadata);
    new_metadata->is_free = true;
    new_metadata->prev = MetaToSplit;
    new_metadata->next = MetaToSplit->next;
    if (MetaToSplit->next != nullptr) {
        MetaToSplit->next->prev = new_metadata;
    }
    MetaToSplit->next = new_metadata;
    MetaToSplit->size = size;
    if(MetaToSplit==list_end){
        list_end=new_metadata;
    }
    if(MetaToSplit->is_free){
        insertToList(MetaToSplit,MetaToSplit->size/KB);
    }
    insertToList(new_metadata,new_metadata->size/KB);
}

void* smallocSbrk(size_t size) {
    if (size <= 0 || size > MAX_ALLOCATION) {
        return nullptr;
    }
    size_t new_size = size;
    if(size % 8 > 0) {
        new_size = size + (8 - size % 8);
    }

    if (list_begin == nullptr) {//first allocation
        void *pointer;
        pointer = sbrk(size + sizeof(MallocMetadata));
        if (pointer == (void*)-1) {
            return nullptr;
        }

        int move_lhs = 0;
        if((intptr_t)pointer % 8>0) {
            move_lhs = 8 - (intptr_t) pointer % 8;
        }
        void* alloc_end = (void*)((char*)pointer + size + sizeof(MallocMetadata));
        pointer  = (void *)((char *)pointer+move_lhs);

        int move_rhs = 0;
        if((((intptr_t)alloc_end+move_lhs) % 8)>0) {
            move_rhs = 8 - (((intptr_t) alloc_end + move_lhs) % 8);
        }

        if((move_lhs > 0) || (move_rhs > 0)){
            sbrk(move_lhs + move_rhs);
        }


        auto* metadata = (MallocMetadata*) pointer;
        metadata->size = new_size;
        metadata->is_free = false;
        metadata->next = nullptr;
        metadata->prev = nullptr;
        metadata->next_free= nullptr;
        metadata->prev_free= nullptr;
        list_begin = metadata;
        list_end = metadata;
        return (void *) (((char*) pointer) + sizeof(MallocMetadata));
    }

    MallocMetadata* iterator;
    int index= size/KB;
    while(index < HIST_SIZE){
        iterator = hist[index];
        while (iterator != nullptr) {
            if (iterator->size >= size) {
                if(iterator->is_free) {
                    if (iterator->size >= 128 + size + sizeof(MallocMetadata)) {
                        split(iterator, new_size); // is responsible for inserting and removing from the free list
                        remove(iterator,iterator->size/KB);
                        MallocMetadata* splitted=iterator->next;
                        if(splitted->next && splitted->next->is_free){
                            combine(splitted,splitted->next);
                        }
                    }else{
                        remove(iterator,iterator->size/KB);
                        return (void *) (((char *) iterator) + sizeof(MallocMetadata));
                    }
                    iterator->is_free = false; // we take the whole first free block that fits
                    return (void *) (((char *) iterator) + sizeof(MallocMetadata));
                }
            }
            iterator = iterator->next;
        }
        index++;
    }

    MallocMetadata* top=list_begin;
    while(top->next != nullptr){
        top=top->next;
    }
    if(top->is_free){ // CHALLENGE #3
        void* pointer=sbrk(size - top->size);
        if (pointer ==(void*)-1) {
            return nullptr;
        }
        remove(top,top->size/KB);
        top->size=size;
        return (void*)((char*)top + sizeof(MallocMetadata));
    }

    top=list_end;
    if(top->is_free){ // CHALLENGE #3 WILDDDDDD
        void* pointer=sbrk(new_size - top->size);
        if (pointer ==(void*)-1) {
            return nullptr;
        }
        remove(top,top->size/KB);
        top->size=new_size;
        return (void*)((char*)top + sizeof(MallocMetadata));
    }
// if we got here then there is no available free block

    void *pointer = sbrk(new_size + sizeof(MallocMetadata));
    if (pointer == (void *) -1) {
        return nullptr;
    }
    MallocMetadata *ptr = (MallocMetadata *) pointer;
    ptr->size = new_size;
    ptr->is_free = false;
    ptr->next = nullptr;
    ptr->prev = list_end;
    list_end->next=ptr;
    list_end=ptr;
    return (void *) (((char *) pointer) + sizeof(MallocMetadata));
}

void* smallocMmap(size_t size){
    if (mmap_head == nullptr) {
        void* pointer=  mmap(nullptr,size+sizeof(MallocMetadata),PROT_READ | PROT_WRITE
                ,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);
        if(pointer == (void*)-1){
            return nullptr;
        }
        auto* metadata = (MallocMetadata*) pointer;

        metadata->size = size;
        metadata->is_free = false;
        metadata->next = nullptr;
        metadata->prev = nullptr;

        mmap_head = metadata;
        mmap_end = metadata;

        return (void *) (((char*) pointer) + sizeof(MallocMetadata));

    }
    else{
        MallocMetadata* tmp = mmap_head;
        while(tmp!= nullptr){
            if((tmp->size >= size) && (tmp->is_free)){
                if(tmp->size >= 128 + sizeof(MallocMetadata) + size){
                    splitmmap(tmp,size); // ############## ASK #########
                }
                tmp->is_free = false;
                return (void *) (((char*) tmp) + sizeof(MallocMetadata));
            }
            tmp = tmp->next;
        }
        /*
        if(mmap_end->is_free){ // CHALLENGE #3
            void* pointer = mmap(nullptr,size - list_end->size,PROT_READ | PROT_WRITE
                    ,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);
            if(pointer == (void*)-1){
                return nullptr;
            }
            mmap_end->size=size;
            mmap_end->is_free = false;
            return (void*)((size_t)mmap_end + sizeof(MallocMetadata));

        }
        */
        void* pointer=  mmap(nullptr,size+sizeof(MallocMetadata),PROT_READ | PROT_WRITE
                ,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);
        if(pointer == (void*)-1){
            return nullptr;
        }

        auto* tmp1=(MallocMetadata*)pointer;

        tmp1->size = size;
        tmp1->is_free= false;
        tmp1->next = nullptr;
        tmp1->prev = mmap_end;
        mmap_end->next = tmp1;
        mmap_end = tmp1;

        return (void *) (((char*) pointer) + sizeof(MallocMetadata));
    }
}

void* smalloc(size_t size){
    if(size == 0 || size > MAX_ALLOCATION){
        return nullptr;
    }
    size_t new_size = size;
    if(size % 8 > 0) {
        new_size = size + (8 - size % 8);
    }
    if(new_size < MMAP_ALLOCATION_LIMIT) {
        return smallocSbrk(new_size);
    }
    return smallocMmap(new_size);
}


void* scalloc(size_t num, size_t size){
    long need_size = size*num;
    if (num==0 || size == 0 || need_size > MAX_ALLOCATION) {
        return nullptr;
    }
    void* allocated = smalloc(num * size);
    //return values are the same as the smalloc's return val
    if(allocated == nullptr){
        return nullptr;
    }
    std::memset(allocated,0,size*num);
    return (void*)allocated;
}

void sfree(void* p){// combine free blocks and inserts to free list
    if(p == nullptr){
        return;
    }
    auto *p_metadata = (MallocMetadata*)((char*)p - sizeof(MallocMetadata));
    if(p_metadata->is_free){
        return;
    }

    if(p_metadata->size < MMAP_ALLOCATION_LIMIT) {
        //used sbrk to allocated
        if(p_metadata== list_begin){
            if(p_metadata->next == nullptr){//alone in the linked list
                list_end=list_begin;
                p_metadata->is_free=true;
                insertToList(p_metadata, p_metadata->size / KB);
                return;
            }
            else{
                if(p_metadata->next->is_free) {//combine the second
                    combine(p_metadata, p_metadata->next);
                    list_begin = p_metadata;
                    return;
                }else{
                    p_metadata->is_free=true;
                    insertToList(p_metadata, p_metadata->size / KB);
                }
            }
            return;
        }
        else if(p_metadata==list_end){
            if(p_metadata->prev && p_metadata->prev->is_free){
                MallocMetadata* src=p_metadata->prev;
                combine(p_metadata->prev,p_metadata);
                list_end=src;
                src->is_free=true;
            }else {
                p_metadata->is_free = true;
                insertToList(p_metadata, p_metadata->size / KB);
            }
            return;
        }

        p_metadata->is_free=true;
        insertToList(p_metadata,p_metadata->size/KB);

        if (p_metadata->next != nullptr && p_metadata->next->is_free) {
            combine(p_metadata,p_metadata->next);
        }
        if (p_metadata->prev != nullptr && p_metadata->prev->is_free) {
            combine(p_metadata->prev,p_metadata);
        }

    }

    else{
        // allocated using mamap
        if(p_metadata->prev != nullptr && p_metadata->next != nullptr) {// in the middle
            MallocMetadata *prev = p_metadata->prev;
            MallocMetadata *next = p_metadata->next;
            prev->next = next;
            next->prev = prev;
        }
        /////used mmap to allocate
        if(p_metadata->prev == nullptr){// first node
            mmap_head = p_metadata->next;
            if(p_metadata->next != nullptr) {
                mmap_head->prev = nullptr;
            }
        }
        else if(p_metadata->next == nullptr){//last node
            mmap_end = mmap_end->prev;
            if(p_metadata->prev != nullptr ){
                mmap_end->next = nullptr;
            }
        }
        p_metadata->next= nullptr;
        p_metadata->prev= nullptr;
        munmap(p_metadata,p_metadata->size + sizeof (MallocMetadata));
    }
}

// we have to implement a matching combine and split
void* srealloc(void* oldp, size_t size){
// no combination need ==> combine with lower
// ==>  with higher ==> combine all ==> wildrness ==> sbrk(smalloc)
int x;
    if( size ==0 || size > MAX_ALLOCATION){
        return nullptr;
    }

    if( oldp != nullptr){
        size_t new_size = size;
        if((size % 8) > 0){
            new_size = size + ( 8 - (size % 8) );
        }
        size=new_size;
        MallocMetadata* OldPtrToMeta=(MallocMetadata*)(((char*)oldp) - sizeof(MallocMetadata));
        size_t oldsize= OldPtrToMeta->size;
        if( size >= MMAP_ALLOCATION_LIMIT) {
            if(new_size <= OldPtrToMeta->size){
                return oldp;
            }
            auto ptr = smalloc(size);
            if (ptr == nullptr) {
                return nullptr;
            } else {
                std::memcpy(ptr, oldp, OldPtrToMeta->size < size ? OldPtrToMeta->size : size);
                if(ptr == oldp) {
                    return ptr;
                }else{
                    sfree(oldp);
                    return ptr;
                }
            }
        }//allocated using sbrk
        else {
            if (OldPtrToMeta->size >= size) {// using the current block and splitting if possible
                x=OldPtrToMeta->size - size - sizeof(MallocMetadata);
                if(x >= 128){
                    split(OldPtrToMeta,new_size);
                    MallocMetadata* checkFree=OldPtrToMeta->next;// the new splitted block
                    if(checkFree->next != nullptr && checkFree->next->is_free){
                        combine(checkFree,checkFree->next);
                    }
                }
                if (OldPtrToMeta->is_free) {// wont check that in tests
                    remove(OldPtrToMeta, OldPtrToMeta->size / KB);
                }
                OldPtrToMeta->is_free= false;//No need for memcpy, same address
                return oldp;
            }
            else if (OldPtrToMeta->prev && OldPtrToMeta->prev->is_free &&
                     (OldPtrToMeta->size + OldPtrToMeta->prev->size + sizeof(MallocMetadata)) >= size){
                MallocMetadata* addr=OldPtrToMeta->prev;

                combine(OldPtrToMeta->prev,OldPtrToMeta);
                x = addr->size - size - sizeof(MallocMetadata);
                if(x >= 128){
                    split(addr,new_size);
                    MallocMetadata* checkFree = addr->next;
                    if(checkFree->next != nullptr && checkFree->next->is_free){
                        combine(checkFree,checkFree->next);
                    }
                }
                if (addr->is_free) {// wont check that in tests
                    remove(addr, addr->size / KB);
                }
                addr->is_free=false;
                memcpy((void*)((char *)(addr) + sizeof(MallocMetadata)) ,oldp,oldsize);
                return  (void *)((char *)(addr)+ sizeof(MallocMetadata));
                //WAS//return addr+ sizeof(MallocMetadata);
            }
            else if ((OldPtrToMeta->next && OldPtrToMeta->next->is_free) &&
                     ((OldPtrToMeta->size + OldPtrToMeta->next->size + sizeof(MallocMetadata)) >= size)){

                combine(OldPtrToMeta,OldPtrToMeta->next);
                x = OldPtrToMeta->size - size - sizeof(MallocMetadata);
                if(x >= 128){
                    split(OldPtrToMeta,size);
                    MallocMetadata* checkFree=OldPtrToMeta->next;
                    if(checkFree->next != nullptr && checkFree->next->is_free){
                        combine(checkFree,checkFree->next);
                    }
                }

                if (OldPtrToMeta->is_free) {// wont check that in tests
                    remove(OldPtrToMeta, OldPtrToMeta->size / KB);
                }
                OldPtrToMeta->is_free=false;
                return  (void *)(oldp);
            }

            else if((OldPtrToMeta->next and OldPtrToMeta->next->is_free) && (OldPtrToMeta->prev and OldPtrToMeta->prev->is_free)
                    && (OldPtrToMeta->size + OldPtrToMeta->prev->size + OldPtrToMeta->next->size + 2* sizeof(MallocMetadata)) >= size){

                MallocMetadata* addr=OldPtrToMeta->prev;
                size_t sum_sizes=OldPtrToMeta->size + OldPtrToMeta->prev->size + OldPtrToMeta->next->size + 2* sizeof(MallocMetadata);

                combine(OldPtrToMeta,OldPtrToMeta->next);
                combine(OldPtrToMeta->prev,OldPtrToMeta);
                x= sum_sizes - size - sizeof(MallocMetadata);
                if(x >= 128){
                    split(addr,size);
                    MallocMetadata* checkFree=addr->next;
                    if(checkFree->next != nullptr && checkFree->next->is_free){
                        combine(checkFree,checkFree->next);
                    }
                }
                if (addr->is_free) {// wont check that in tests
                    remove(OldPtrToMeta, OldPtrToMeta->size / KB);
                }
                addr->is_free=false;
                memcpy((void*)((char *)addr + sizeof(MallocMetadata)),oldp,oldsize);
                return  (void *)(((char *) addr) + sizeof(MallocMetadata));
            }
            
			if(OldPtrToMeta->next and OldPtrToMeta->next->is_free){
					combine((OldPtrToMeta), OldPtrToMeta->next);
			}
            if(OldPtrToMeta->next == nullptr || OldPtrToMeta==list_end) {

                if(OldPtrToMeta->prev && OldPtrToMeta->prev->is_free ){

                    void * ptr = sbrk(size - OldPtrToMeta->size - OldPtrToMeta->prev->size);
                    ///This is the last node we allocate above it
                    if(ptr == (void*)(-1))
                    {
                        return nullptr;
                    }
                    combine(OldPtrToMeta->prev , OldPtrToMeta);
                    remove(OldPtrToMeta->prev,OldPtrToMeta->prev->size/KB);

                    OldPtrToMeta->prev->size = size;
                    OldPtrToMeta->prev->is_free=false;
                    

                    MallocMetadata* addr=OldPtrToMeta->prev;
                    memcpy((void*)((char *)(addr) + sizeof(MallocMetadata)) ,oldp,oldsize);
                    return  (void *)((char *)(addr)+ sizeof(MallocMetadata));
                    return  oldp;

                }
                void * ptr = sbrk(size - OldPtrToMeta->size);
                ///This is the last node we allocate above it
                if(ptr == (void*)(-1))
                {
                    return nullptr;
                }
                size_t  tmp = size - OldPtrToMeta->size;
                if(OldPtrToMeta->is_free) {
                    remove(OldPtrToMeta, OldPtrToMeta->size / KB);
                }
                OldPtrToMeta->size += tmp;
                OldPtrToMeta->is_free=false;
                return  oldp;
            }
            else
            {



                auto ptr = smalloc(size);
                if (ptr == nullptr){
                    return nullptr;
                }else
                {
                    std::memcpy(ptr,oldp,size);
                    if(oldp == ptr){
                        return ptr;
                    }else{
                        sfree(oldp);
                        return ptr;
                    }
                }
            }

        }

    }else{ // if oldp == nulptr
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
    while( counter != nullptr ){
        Allocated++;
        counter=counter->next;
    }
    counter = mmap_head;
    while(counter != nullptr){
        Allocated ++;
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
    counter = mmap_head;
    while(counter != nullptr){
        Allocated += counter->size;
        counter=counter->next;
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




