//
// Created by m7mds on 27/12/2021.
//

#ifndef DATASTRUCTURES_HW2_LIST_H
#define DATASTRUCTURES_HW2_LIST_H
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include "Nodetype.h"
#include "Exceptions.h"
using namespace std;

template <class T>
class Node
{
public:
    RTAllPlayersKey key;
    Node* prevNode;
    Node* nextNode;
    Node ():  key(),  prevNode(NULL),nextNode(NULL){
    }

    Node(const RTAllPlayersKey& key,Node* prev = NULL,Node* next = NULL)
            :key((key)),prevNode(prev),nextNode(next){
    }

    Node(const Node & n1):
            key(n1.key),prevNode(n1.prevNode),nextNode(n1.nextNode){
    }
    ~Node()=default;

    Node& operator=(const Node& node){
        key=node.key;
        nextNode=node.nextNode;
        prevNode=node.prevNode;
        return *this;
    }



};

template<class T >
class LinkedList
{
public:

    Node<T>* head;

    Node<T>* find(int key1);

    LinkedList():head(new Node<T>()){
        //head->key=new T();
        head->nextNode= nullptr;
    }
    ~LinkedList(){
        auto ptr=this->head;
        while(ptr!= nullptr){
            auto temp = ptr;
            ptr=ptr->nextNode;
            delete temp;
        }
    }
    StatusType insertByNode(Node<RTAllPlayersKey>* ToAdd) {
        auto iterator = this->head;
        while (iterator->nextNode != nullptr) {
            iterator = iterator->nextNode;
        }
        iterator->nextNode = new Node<T>(ToAdd->key);
        iterator->nextNode->prevNode=iterator;
        return SUCCESS;
    }

    StatusType insert(RTAllPlayersKey& ToAdd) {

        auto iterator = this->head;
        while (iterator->nextNode != nullptr) {
            iterator = iterator->nextNode;
        }
        iterator->nextNode = new Node<T>(ToAdd);
        iterator->nextNode->prevNode = iterator;
        return SUCCESS;
    }

    bool removeByPtr(Node<RTAllPlayersKey>* ToRemove){

        auto iter=this->head->nextNode;
        if(iter == nullptr){
            return false;
        }
        while(iter->nextNode != nullptr){
            if(iter->key==(ToRemove)->key){
                iter->prevNode->nextNode=iter->nextNode;
                iter->nextNode->prevNode=iter->prevNode;
                delete iter;
                return true ;
            }
            iter=iter->nextNode;
        }
        // means that the deleted node if the last noed in the list
        if(iter->key==(ToRemove)->key){
            iter->prevNode->nextNode = NULL;
            delete iter;
            return true;
        }
        return false;
    }

    bool remove(RTAllPlayersKey& ToRemove) {
        auto iter=this->head->nextNode;
        if(iter == nullptr){
            return false;
        }
        while(iter->nextNode != nullptr){
            if(iter->key==(ToRemove)){
                iter->prevNode->nextNode=iter->nextNode;
                iter->nextNode->prevNode=iter->prevNode;
                delete iter;
                return true;
            }
            iter=iter->nextNode;
        }
        // means that the deleted node if the last noed in the list
        if(iter->key==(ToRemove)){
            iter->prevNode->nextNode = NULL;
            delete iter;
            return true;
        }
        return false;
    }

    bool removeById(int ToRemove) {
        if(this->head->nextNode == nullptr){
            return false;
        }
        auto iter=this->head->nextNode;

        while(iter->nextNode != nullptr){
            if(iter->key == (ToRemove)){
                iter->prevNode->nextNode=iter->nextNode;
                iter->nextNode->prevNode=iter->prevNode;
                delete iter;
                return true;
            }
            iter=iter->nextNode;
        }

        // means that the deleted node if the last noed in the list
        if((iter->key)==(ToRemove)){
            iter->prevNode->nextNode = NULL;
            delete iter;
            return true;
        }
        return false;
    }

    bool ListContains(int playerid){ // return if the list contains the specific player
        if(this->head->nextNode == nullptr){
            return false;
        }
        auto iter=this->head->nextNode;
        while(iter->nextNode != nullptr){
            if(iter->key.playerId ==(playerid)){
                return true;
            }
            iter=iter->nextNode;
        }

        // means that the deleted node if the last noed in the list
        if(iter->key.playerId ==(playerid)){
            return true;
        }
        return false;
    }

};

template<class T >
Node<T>* LinkedList<T>::find(int playerid){
    if(this->head->nextNode == nullptr){
        return nullptr;
    }
    auto iter=this->head->nextNode;

    while(iter->nextNode != nullptr){
        if(iter->key.playerId ==(playerid)){
            return iter;
        }
        iter=iter->nextNode;
    }

    // means that the deleted node if the last noed in the list
    if(iter->key.playerId ==(playerid)){
        return iter;
    }
    return nullptr;
}



#endif //DATASTRUCTURES_HW2_LIST_H
