//
// Created by m7mds on 26/12/2021.
//

#ifndef DATASTRUCTURES_HW2_NODETYPE_H
#define DATASTRUCTURES_HW2_NODETYPE_H
//
// Created by m7mds on 16/11/2021.
//

#ifndef UNTITLED5_NODETYPE_H
#define UNTITLED5_NODETYPE_H
#include <iostream>
#include <cstdlib>
#include "AvlNode.h"
#include "AvlTree.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   HW2      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class RTgroupKey{
public:
    int playerId;
    int groupId;
    int score;
    int level;
    RTgroupKey(int Pid,int Gid,int score, int level):playerId(Pid),groupId(Gid),score(score),level(level){};
    RTgroupKey():playerId(-1),groupId(-1),score(-1),level(-1){

    }
    int& operator[](int k){
        return score;
    }
    bool operator==(RTgroupKey &Player) const {
        return (this->playerId==Player.playerId);
    }
    int operator+(int& add){
        return (this->level)+add;
    }

    bool operator!=(RTgroupKey &Player) const {
        return  (this->playerId != Player.playerId);
    }

    bool operator>(RTgroupKey &Player) const
    //primary sort according to score, secondary one is according to players Player id
    {
        if (this->level == Player.level) {
            return this->playerId > Player.playerId;
        } else {
            return this->level > Player.level;
        }
    }

    bool operator<(RTgroupKey &Player) const
    {
        if (this->level == Player.level ) {
            return this->playerId < Player.playerId;
        } else {
            return this->level < Player.level ;
        }
    }

    bool operator<=(RTgroupKey &Player) const
    {
        if (this->level == Player.level ) {
            return this->playerId <= Player.playerId;
        } else {
            return this->level <= Player.level ;
        }
    }
};

class RTgroupData{
public:
    int playerId;
    int groupId;
    int score;
    RTgroupData(int Pid, int Gid, int score):playerId(Pid), groupId(Gid), score(score){};
};

class RTAllPlayersKey{
public:
    int playerId;
    int groupId;
    int score;
    int level;
    RTAllPlayersKey()=default;
    RTAllPlayersKey(int Pid,int Gid,int score,int level=0):playerId(Pid),groupId(Gid),score(score),level(level){};
    RTAllPlayersKey &operator=(const RTAllPlayersKey &other) = default;
    bool operator==(int PlayerId) const {
        return (this->playerId ==PlayerId);
    }
    int operator+(int& add){
        return (this->level)+add;
    }
    int& operator[](int key){
        return score;
    }
    bool operator==(RTAllPlayersKey &Player) const {
        return (this->playerId == Player.playerId);
    }

    bool operator!=(RTAllPlayersKey &Player) const {
        return ((this->playerId != Player.playerId));
    }
    bool operator>(RTAllPlayersKey &Player) const
    //primary sort according to score, secondary one is according to players Player id
    {
        if (this->level  == Player.level) {
            return this->playerId > Player.playerId;
        } else {
            return this->level > Player.level;
        }
    }

    bool operator<(RTAllPlayersKey &Player) const
    {
        if (this->level  == Player.level ) {
            return this->playerId < Player.playerId;
        } else {
            return this->level  < Player.level;
        }
    }

    bool operator<=(RTAllPlayersKey &Player) const
    {
        if (this->level  == Player.level ) {
            return this->playerId <= Player.playerId;
        } else {
            return this->level  <= Player.level;
        }
    }
};
class RTAllPlayersData {
public:
    int playerId;
    int groupId;
    int score;

    RTAllPlayersData(int Pid, int Gid, int score) : playerId(Pid), groupId(Gid), score(score) {};
    RTAllPlayersData():playerId(-500), groupId(-500), score(-500){}
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   HW2      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~










//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AllPlayersKey;
class AllPlayersNode;
class AllPlayersIdNodeData;
class GroupLevelsNode
{
public:
    int GroupId;
    int PlayerId;
    int Level;

    AllPlayersIdNodeData* PointerToTheData;
    //AvlNode<AllPlayersKey,AllPlayersNode>* PointerToTheLevelNode;// This will help to Get to the Node in AllplayersLevelTree in o(1) complexity time
    //AvlNode<int,AllPlayersIdNodeData>* PointerToTheIdNode;// This will help to Get to the Node in AllplayersIdTree in o(1) complexity time

    GroupLevelsNode(int PlayerId,int GroupId, int Level,AllPlayersIdNodeData* PointerToTheData1)
            :GroupId(GroupId),PlayerId(PlayerId),Level(Level){
        this->PointerToTheData=(PointerToTheData1);
    };
    GroupLevelsNode(){
        this->PointerToTheData=(nullptr);
    }
    ~GroupLevelsNode(){
        PointerToTheData= nullptr;
    }
};

class GroupLevelsKey
{
public:
    int level;
    int PlayerId;

    GroupLevelsKey() = default;

    GroupLevelsKey &operator=(const GroupLevelsKey &other) = default;

    bool operator>(GroupLevelsKey &Player) const
    {
        if (this->level == Player.level) {
            return this->PlayerId < Player.PlayerId;
        } else {
            return this->level > Player.level;
        }
    }

    bool operator==(GroupLevelsKey &Player) const {
        return ((this->PlayerId == Player.PlayerId) && (this->level == Player.level));
    }

    bool operator!=(GroupLevelsKey &Player) const {
        return ((this->PlayerId != Player.PlayerId) || (this->level != Player.level));
    }


    GroupLevelsKey(int level, int PlayerId) : level(level), PlayerId(PlayerId) {}

    ~GroupLevelsKey() = default;

    bool operator<(GroupLevelsKey &Player) const
    {
        if (this->level == Player.level) {
            return this->PlayerId > Player.PlayerId;
        } else {
            return this->level < Player.level;
        }
    }
};// sort by level then by the lower playersid get to be the node with higher value

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AllPlayersKey;
class AllPlayersNode;
class GroupsTreeNode// Tree #1 Data
{
public:
    int GroupId;
    //int NumOfPlayers;// group counter....optional, not in use at the moment
    int HighestLevelInGroup;// מידע נוסף מתעדכן בכל הוצאה והכנסה לעץ
    int PlayerIdWithHighestLevel;
    //AvlTree<int,GroupPlayersNode>* GroupPlayersTree;// A Tree sorted by PlayersId's
    AvlTree<GroupLevelsKey,GroupLevelsNode>* GroupLevelsTree;// a Tree Sorted by players Levels in the first place then by the playerid whereas
    //player with lower id is th more "valued" according to search tree
    explicit GroupsTreeNode(int GroupId):GroupId(GroupId),HighestLevelInGroup(1),PlayerIdWithHighestLevel(1) {
        //this->GroupPlayersTree=new AvlTree<int,GroupPlayersNode>;
        this->GroupLevelsTree = new AvlTree<GroupLevelsKey, GroupLevelsNode>;
    }
    GroupsTreeNode(){
        GroupLevelsTree=new AvlTree<GroupLevelsKey, GroupLevelsNode>;
    }
    ~GroupsTreeNode(){
        delete GroupLevelsTree;
    }

};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//A TREE THAT IS SORTED BY PLAYERS LEVELS THEN BY ID ###CONTAINS ALL THE PLAYERS###
class AllPlayersNode{
public:
    int level;
    int PlayerId;
    int GroupId;
    AllPlayersNode(int level,int PlayerId,int GroupId):level(level),PlayerId(PlayerId),GroupId(GroupId){}
    AllPlayersNode(){

    }
    ~AllPlayersNode()=default;
};
class AllPlayersKey {
public:
    int level;
    int PlayerId;

    AllPlayersKey() = default;

    AllPlayersKey &operator=(const AllPlayersKey &other) = default;

    bool operator>(AllPlayersKey &Player) const
    // if lvl1<lvl2 => lvl1 is "bigger"
    {
        if (this->level == Player.level) {
            return this->PlayerId < Player.PlayerId;
        } else {
            return this->level > Player.level;
        }
    }

    bool operator==(AllPlayersKey &Player) const {
        return ((this->PlayerId == Player.PlayerId) && (this->level == Player.level));
    }

    bool operator!=(AllPlayersKey &Player) const {
        return ((this->PlayerId != Player.PlayerId) || (this->level != Player.level));
    }


    AllPlayersKey(int level, int PlayerId) : level(level), PlayerId(PlayerId) {}

    ~AllPlayersKey() = default;

    bool operator<(AllPlayersKey &Player) const
    // if lvl1>lvl2 => lvl2 is "bigger"
    {
        if (this->level == Player.level) {
            return this->PlayerId > Player.PlayerId;
        } else {
            return this->level < Player.level;
        }
    }
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//A TREE THAT IS SORTED BY PLAYERS ID ###CONTAINS ALL THE PLAYERS###
class AllPlayersIdNodeData{
public:
    int level;
    int PlayerId;
    int GroupId;
    AllPlayersIdNodeData(int level,int PlayerId,int GroupId):level(level),PlayerId(PlayerId),GroupId(GroupId){}
    AllPlayersIdNodeData(){
    }
    ~AllPlayersIdNodeData()=default;
};




#endif //UNTITLED5_NODETYPE_H
#endif //DATASTRUCTURES_HW2_NODETYPE_H
