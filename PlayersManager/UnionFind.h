//
// Created by moham on 03/01/2022.
//

#ifndef UNTITLED1_UNIONFIND_H
#define UNTITLED1_UNIONFIND_H
#include <iostream>
#include "UnionNode.h"
#include "Exceptions.h"

typedef enum {
    UNION_NULL_ARGS,
    UNION_SUCCESS,
    UNION_MEM_FAIL
} statusUnion;

class UnionFind{
public:
    UnionNode** Groups;
    int groups_num;

    UnionFind(int k){// gets k+1
        this->Groups = new UnionNode*[k];
        this->groups_num = k;
        ///////////////////////////////////////check if we need to check if the allocation completed successfully
        for(int i=0 ; i<k ; i++){
            Groups[i] = new UnionNode(i);
        }
    }

    ~UnionFind(){
        for(int i=0 ; i<groups_num ; i++) {
            delete Groups[i];
        }
        delete[] Groups;
    }
    //return a pointer to the head of the group which agency belong to it
    UnionNode* findHead(int group_id){
        UnionNode* tmp1 ;
        UnionNode* tmp2;
        UnionNode* head;

        //get the head
        tmp1 = this->Groups[group_id];
        while (tmp1->father != nullptr)
        {
            tmp1 = tmp1->father;
        }
        head = tmp1;
        tmp1 = this->Groups[group_id];

        //compress the path
        while(tmp1->father != head && tmp1!=head){
            tmp2 = tmp1;
            tmp1 = tmp1->father;
            tmp2->father = head;
        }
        return head;
    }

    statusUnion Union(int first_group , int second_group){
        UnionNode* first = findHead(first_group);
        UnionNode* second = findHead(second_group);

        if(first->GroupID == second->GroupID){ // means that the two groups had been merged before
            return UNION_SUCCESS;
        }
        auto newGroup = first->GroupPlayers->AvlMerge(first->GroupPlayers,second->GroupPlayers);//merge the players
        if (newGroup == nullptr) {
            delete newGroup;
            return UNION_MEM_FAIL;
        }
        UnionNode* bigger = (first->size >= second->size) ? first : second;
        UnionNode* smaller = (first->size < second->size) ? first : second;
        for(int i=1 ; i<201 ; i++){
            bigger->zerosLevel_scores[i] += smaller->zerosLevel_scores[i];
        }

        smaller->father = bigger;
        bigger->size += smaller->size;

        delete smaller->GroupPlayers;
        smaller->GroupPlayers = nullptr;

        delete bigger->GroupPlayers;
        bigger->GroupPlayers = nullptr;

        bigger->GroupPlayers = newGroup;

        return UNION_SUCCESS;
    }


};

#endif //UNTITLED1_UNIONFIND_H
