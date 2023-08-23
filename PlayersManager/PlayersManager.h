//
// Created by m7mds on 26/12/2021.
//

#ifndef DATASTRUCTURES_HW2_PLAYERSMANGER_H
#define DATASTRUCTURES_HW2_PLAYERSMANGER_H
#include "Nodetype.h"
#include "library2.h"
#include "UnionFind.h"
#include "DynamicArray.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define EXPAND_FACTOR 2
class PlayersManager {

public:

    // #1 structure
    AvlTree<RTAllPlayersKey,RTAllPlayersData>* AllPlayers;

    // #2 structure
    UnionFind* UnionGroups;

    // #3 structure
    HashTable* Players;

    // #4 struct
    int* all_players_zerosLevel_scores;

    //int numOfgroups;
    int realSize;
    const int multiply=2;
    int scale;
    int numofgroups;
    PlayersManager(int k,int scale):AllPlayers(new AvlTree<RTAllPlayersKey,RTAllPlayersData>)
            ,UnionGroups(new UnionFind(k+1)),Players(new HashTable())// k+1 because we dont want groupid=0
            ,realSize(128),scale(scale),numofgroups(k){

        this->all_players_zerosLevel_scores=new int[201];

        for(int i=1 ; i<201 ; i++){
            this->all_players_zerosLevel_scores[i] = 0;
        }
    }

    PlayersManager(const PlayersManager& other)=delete;//copy ctor

    ~PlayersManager(){
//        for(int i=0;i<numofgroups;i++){
//            delete[ UnionGroups[i];
//        }
        delete Players;
        delete UnionGroups;
        delete AllPlayers;
        delete[] all_players_zerosLevel_scores;
    }
//    void resize(){
//        int newSize=realSize*multiply;
//        UnionFind** newArray=new UnionFind*[newSize];
//        for (int i = 0; i < numOfElements; ++i) {
//            newArray[i]=array[i];
//        }
//        delete[] array;
//        this->array=newArray;
//        this->realSize=newSize;
//    }


    StatusType MergeGroups( int GroupID1, int GroupID2);

    StatusType AddPlayer( int PlayerID, int GroupID, int score);

    StatusType RemovePlayer( int PlayerID);

    StatusType IncreasePlayerIDLevel( int PlayerID, int LevelIncrease);

    StatusType ChangePlayerIDScore( int PlayerID, int NewScore);

    StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double *players);

    StatusType AverageHighestPlayerLevelByGroup( int GroupID, int m, double *level);

    StatusType GetPlayersBound( int GroupID, int score, int m,
                                int *LowerBoundPlayers, int *HigherBoundPlayers);


    int GetPercent(RTgroupKey& k1,RTAllPlayersKey &k2 , int group_id , int score , int* num_of_elements_with_score) ;
    int GetSumInGroup(RTgroupKey& k1 , int group_id );
    int GetSumInAll(RTAllPlayersKey &k2 , int group_id );

    void Quit();
};
#endif //DATASTRUCTURES_HW2_PLAYERSMANGER_H
