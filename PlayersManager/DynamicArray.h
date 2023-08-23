//
// Created by m7mds on 26/12/2021.
//


// Created by m7mds on 11/06/2021.
//

#ifndef MEVNI2_DYNAMICARRAY_H
#define MEVNI2_DYNAMICARRAY_H

#include <cstdlib>
#include "UnionFind.h"
#include "List.h"
#include "Nodetype.h"

#define InitialSize 64
class HashTable {
public:
    LinkedList<RTAllPlayersKey> **ListArr;
    double stress_factor;
    int size;
    int numOfItems;

    HashTable()
    {
        stress_factor = 0;
        size = InitialSize;
        numOfItems = 0;
        ListArr = new LinkedList<RTAllPlayersKey>*[InitialSize];// check if we should do a scan and ential
        for(int i = 0 ; i < size ; i++){
            ListArr[i] = new LinkedList<RTAllPlayersKey>;
        }
    }

    int Hash(int PlayerId) const{
        return PlayerId%size;
    }

    ~HashTable() {
        for(int i=0; i<this->size;i++){
            delete ListArr[i];
        }
        delete[]ListArr;
    }

    void ExpandHash(){
        int oldSize = size;
        this->size = size * 2;
        auto *new_arr = new LinkedList<RTAllPlayersKey>*[2 * oldSize];

        for(int i = 0 ; i < 2 * oldSize ; i++){
            new_arr[i] = new LinkedList<RTAllPlayersKey>;
        }

        for (int i = 0; i < oldSize; i++) {
            Node<RTAllPlayersKey> *temp = ListArr[i]->head->nextNode;

            while (temp) {
                Node<RTAllPlayersKey> *next = temp->nextNode;


                int index = Hash(temp->key.playerId);

                new_arr[index]->insertByNode(temp);

                ListArr[i]->removeByPtr(temp);

                temp = next;
            }
        }

        for(int i = 0 ; i < oldSize ; i++){
            delete ListArr[i];
        }

        delete[] ListArr; // check if this is the correct way
        ListArr = new_arr;
        stress_factor = (double) numOfItems / size;
    }

    void CompressHashTable() {
        int oldSize = size;
        size = size / 2;

        auto *new_arr = new LinkedList<RTAllPlayersKey>*[oldSize/2];
        for(int i = 0 ; i < oldSize/2 ; i++){
            new_arr[i] = new LinkedList<RTAllPlayersKey>;
        }

        for (int i = 0; i < oldSize; i++) {

            Node<RTAllPlayersKey> *temp = ListArr[i]->head->nextNode;

            while (temp) {

                Node<RTAllPlayersKey> *next = temp->nextNode;
                int index = Hash(temp->key.playerId);

  
                new_arr[index]->insertByNode(temp);

                ListArr[i]->removeByPtr(temp);

                temp = next;
            }
        }
        for(int i = 0 ; i < oldSize ; i++){
            delete ListArr[i];
        }
        delete[] ListArr;
        ListArr = new_arr;
        stress_factor = (double) numOfItems / size;
    }

    bool AddToHashTable(RTAllPlayersKey& ToAdd) {
        int index = Hash(ToAdd.playerId);
        if (ListArr[index]->ListContains(ToAdd.playerId))
            return false;

        ListArr[index]->insert(ToAdd);
        numOfItems++;
        stress_factor = (double) numOfItems / size;
        if (stress_factor > 0.5)
            ExpandHash();
        return true;
    }

    bool DeleteFromHashTable(int playerid) {
        int index = Hash(playerid);
        Node<RTAllPlayersKey>* Player= ListArr[index]->find(playerid);
        if(Player != nullptr) {
            if (this->ListArr[index]->removeById(playerid)) {
                numOfItems--;
                stress_factor = (double) numOfItems / size;
                if (size != InitialSize && stress_factor < 0.25) {
                    CompressHashTable();
                }
                return true;
            }
        }
        return false;

    }
};






//enum exceptions{ MEM_FAIL};
//template<class T>
//class DynamicArray{
//public:
//    LinkedList<T>* array;
//
//    int size;
//    int maxSize;
//    const int multiply=2;
//    const int creationSize=128;
//
//    DynamicArray(){
//        maxSize=creationSize;
//        size=0;
//        //array= malloc(maxSize* sizeof (T));
//        array= new LinkedList<T>*[maxSize];
//        for(int i=0;i<creationSize;i++){
//            array[i]=new LinkedList<T>();
//        }
//    }//ctor
//    DynamicArray(const DynamicArray& other) {
//        array= new LinkedList<T>[other.maxSize];
//        //array= malloc(maxSize* sizeof (T));
//        //array= (T*)malloc(maxSize* sizeof (T));
//
//        if(array==NULL) throw MEM_FAIL;
//        for (int i = 0; i < other.size; ++i) {
//            array[i]=other.array[i];
//        }
//        size=other.size;
//        maxSize=other.maxSize;
//
//    }
//    ~DynamicArray(){
//        for( int i=0; i<maxSize;i++){
//            delete array[i];
//        }
//        delete array;
//    }//dtor
//    //DynamicArray& operator=(const DynamicArray& other);
//    T& operator [] (int index){
//        return array[index];
//
//    }
//    void add( T& item){
//        size++;
//        if(size > maxSize)
//        {
//            expand();//##############
//
//        }
//        array[findIndexInHash((item))]->insert(item);
//
//    }
//    void remove(T& item){
//        {
//            size--;
//            if(size<maxSize/10){
//                dwindle();
//            }
//
//            array[findIndexInHash(item)]->remove(item);
//        }
//    }
//    void dwindle(){
//        {
//            int newsize=maxSize/10;
//            LinkedList<T>** newHash=new LinkedList<T>*[newsize];
//
//            for(int i=0;i<newsize;i++){
//                newHash[i]=new LinkedList<T>();
//            }
//            for(int i = 0 ;i < newsize ; i++){
//
//                auto* ptr=this->array[i]->head->nextNode;
//                while(ptr!= nullptr){
//
//                    int index= findIndexInHash(*ptr->key)% newsize;
//                    //newHash[index]
//                }
//            }
//        }
//    }
//    void expand();
//    int getSize() {
//        return size;
//    }//returns the size
////    void setSize(int newSize);
////    void clear();
////    void deleteElement(int pos);
//    int getIndex( int key){
//// get an index for the element using hash function
//        return (key% maxSize);
//    }
//
//    Node<T>*  findInHash(RTAllPlayersKey& key){
//        int index= getIndex(key.playerId);
//            return array[index]->find(key);
//    }
//    int findIndexInHash(RTAllPlayersKey& key){
//        return getIndex(key.playerId);
//    }
//};





//template<class T>
//DynamicArray &DynamicArray<T>::operator=(const DynamicArray &other) {
//    if(this== &other)
//        return *this;
//    if(other.size==0) this->clear();
//
//    //maxSize=other.maxSize;
//    //setSize(other.size);
//    //size=other.size;
//    for (int i = 0; i < other.size; ++i) {
//        array[i]=other.array[i];
//    }
//    return *this;
//
//
//}








//template<class T>
//void DynamicArray<T>::setSize(int newSize) {
//    size=newSize;
//    if(size!=0)
//    {}
//
//}

//template<class T>
//void DynamicArray<T>::clear() {
//
//}

//template<class T>
//void DynamicArray<T>::deleteElement(int pos) {
//
//}









#endif //MEVNI2_DYNAMICARRAY_H
