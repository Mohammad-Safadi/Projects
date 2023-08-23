//
// Created by m7mds on 26/12/2021.
//
#include "library2.h"
#include "PlayersManager.h"

void *Init(int k, int scale) {
    PlayersManager *DS;
    try
    {
        DS = new PlayersManager(k,scale);
    }
    catch (const std::bad_alloc &)
    {
        DS = nullptr;
    }
    return (void *)DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2) {
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    if(GroupID1<=0 || GroupID2<=0){
        return INVALID_INPUT;
    }
    int groups_num = ((PlayersManager *)DS)->numofgroups;
    if(GroupID1 > groups_num || GroupID2 > groups_num){
        return INVALID_INPUT;
    }
    if(GroupID2 == GroupID1){
        return SUCCESS;
    }

    return (((PlayersManager *)DS)->MergeGroups(GroupID1,GroupID2));
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score) {
    if (DS==nullptr)
    {
        return INVALID_INPUT;
    }
    if (GroupID <= 0 || PlayerID <= 0)
    {
        return INVALID_INPUT;
    }
    if(GroupID > ((PlayersManager *)DS)->numofgroups){
        return INVALID_INPUT;
    }
    if(score > ((PlayersManager *)DS)->scale){
        return INVALID_INPUT;
    }
    if (score <= 0)
    {
        return INVALID_INPUT;
    }
    return(((PlayersManager *)DS)->AddPlayer(PlayerID,GroupID,score));
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if (PlayerID <= 0|| DS == nullptr)
    {
        return INVALID_INPUT;
    }
    return (((PlayersManager *)DS)->RemovePlayer(PlayerID));
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease) {
    if (DS==nullptr)
    {
        return INVALID_INPUT;
    }
    if ( PlayerID <= 0 || LevelIncrease<=0 )
    {
        return INVALID_INPUT;
    }
    return(((PlayersManager *)DS)->IncreasePlayerIDLevel(PlayerID,LevelIncrease));
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore) {
    if(DS == nullptr){
        return INVALID_INPUT;
    }
    if(PlayerID <= 0){
        return INVALID_INPUT;
    }
    if(NewScore <=0 || NewScore > ((PlayersManager *)DS)->scale){
        return INVALID_INPUT;
    }
    return (((PlayersManager *) DS)->ChangePlayerIDScore(PlayerID, NewScore));
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double *players) {
    if(DS==nullptr || players==nullptr){
        return INVALID_INPUT;
    }
    if(GroupID<0 || GroupID>((PlayersManager *)DS)->numofgroups){
        return INVALID_INPUT;
    }
    if( lowerLevel > higherLevel || higherLevel < 0 ){
        return FAILURE;
    }
    return (((PlayersManager *) DS)->GetPercentOfPlayersWithScoreInBounds(GroupID,score,lowerLevel,higherLevel,players));
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double *level) {
    if(DS==nullptr || level==nullptr){
        return INVALID_INPUT;
    }
    if(GroupID<0 || GroupID>((PlayersManager *)DS)->numofgroups){
        return INVALID_INPUT;
    }
    if(m<=0){
        return INVALID_INPUT;
    }

    return (((PlayersManager *) DS)->AverageHighestPlayerLevelByGroup(GroupID,m,level));
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers) {
    if(DS==nullptr || LowerBoundPlayers==nullptr || HigherBoundPlayers==nullptr){
        return INVALID_INPUT;
    }
    if( GroupID > ((PlayersManager *)DS)->numofgroups){
        return INVALID_INPUT;
    }
    if(m == 0 ){
        return SUCCESS;
    }

    if(m<0){
        return INVALID_INPUT;
    }
    if(score > ((PlayersManager *)DS)->scale){
        return INVALID_INPUT;
    }
    if (score <= 0)
    {
        return INVALID_INPUT;
    }
    return (((PlayersManager *) DS)->GetPlayersBound(GroupID,score,m,LowerBoundPlayers,HigherBoundPlayers));
}

void Quit(void **DS) {
    delete (PlayersManager*)(*DS);
    *DS=nullptr;
}
