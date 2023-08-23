//
// Created by m7mds on 05/01/2022.
//

#ifndef HW2_2ND_TRY_UNIONNODE_H
#define HW2_2ND_TRY_UNIONNODE_H
//
// Created by m7mds on 27/12/2021.
//

#ifndef DATASTRUCTURES_HW2_UNIONFIND_H
#define DATASTRUCTURES_HW2_UNIONFIND_H

#include "AvlTree.h"
#include "AvlNode.h"



class UnionNode{
public:
    int GroupID;
    int size;///////////////////////size of united groups and not the size of the players in the group
    AvlTree<RTgroupKey,RTgroupData>* GroupPlayers;//rank tree that contains the players, sorted by number of level
    UnionNode* father;
    int* zerosLevel_scores; //////////cehck if we should allocate it

    UnionNode(int groupid):GroupID(groupid),GroupPlayers(new AvlTree<RTgroupKey,RTgroupData>)
    {
        this->zerosLevel_scores=new int[201];
        size=1;///////////////////////size of united groupss and not the size of the players in the group
        father= nullptr;
        for (int i = 1; i < 201; i++) {
            zerosLevel_scores[i]=0;
        }
    }
    ~UnionNode() {
        delete GroupPlayers;
        delete[] zerosLevel_scores;
    };




    /*
    friend statusUnion unionFindUnion( UnionFind *first,  UnionFind *second) {
        if (first == nullptr || second == nullptr) return UNION_NULL_ARGS;
        auto newGroup = first->GroupPlayers->AvlMerge(first->GroupPlayers,
                                                      second->GroupPlayers);// ??first->GroupPlayers->AvlMerge?
        if (newGroup == nullptr) {
            delete newGroup;
            return UNION_MEM_FAIL;
        }
        if (first->size > second->size) {
            first->size += second->size;
            second->father = first;

            delete second->GroupPlayers; // check whats happen with new group
            second->GroupPlayers = nullptr;
            delete first->GroupPlayers;
            first->GroupPlayers = nullptr;
            first->GroupPlayers = newGroup;

            return UNION_SUCCESS;
        }

        second->size += first->size;
        first->father = second;

        delete second->GroupPlayers;
        second->GroupPlayers = nullptr;
        delete first->GroupPlayers;
        first->GroupPlayers = nullptr;
        first->GroupPlayers = newGroup;
        return UNION_SUCCESS;


    }
    //UnionFind()=default;
    UnionFind* find() {
        auto tmp = this;
        while (tmp->father != nullptr) { //continue until reaching the head
            tmp = tmp->father;
        }
        auto it = this;
        //if (it== nullptr) return tmp;
        while (it->father != nullptr) {
            auto x = it->father;
            it->father = tmp;
            it = x;
        }
        return tmp;
    }*/


};






#endif //DATASTRUCTURES_HW2_UNIONFIND_H

#endif //HW2_2ND_TRY_UNIONNODE_H
