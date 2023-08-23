//
// Created by m7mds on 26/12/2021.
//

#include "PlayersManager.h"
#include <limits>
///function get a histo and return the number of the element in it
int getNumOfElemnts(int* histo , int k){

    int sum=0;
    for (int i = 1; i < k+1; ++i) {
            sum+=histo[i];
    }
    return sum;

}

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2) {

    auto result = UnionGroups->Union(GroupID1,GroupID2);
    if(result == UNION_MEM_FAIL){
        return ALLOCATION_ERROR;
    }else{
        if(result == UNION_NULL_ARGS){
            return INVALID_INPUT;
        }
        return SUCCESS;
    }
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    RTAllPlayersKey ToAdd(PlayerID,GroupID,score,0);
    if(!Players->AddToHashTable(ToAdd)){
        return FAILURE;
    }

    auto* group_head = this->UnionGroups->findHead(GroupID);
    group_head->zerosLevel_scores[score]+=1;
    this->all_players_zerosLevel_scores[score]+=1;
    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {// o(log(n)) + o(1) approximate
    RTAllPlayersKey ToRemove(PlayerID,0,0,0);
    int groupId=-1 ,player_level=-1 , player_score=-1;
    Node<RTAllPlayersKey>* found;
    try{
        found=Players->ListArr[Players->Hash(PlayerID)]->find(PlayerID);////check what's happen to found after we delete the player id
        if(found == nullptr){
            return FAILURE;
        }
        ToRemove.level=found->key.level;
        groupId = found->key.groupId;
        player_level = found->key.level;
        player_score = found->key.score;

         if( !Players->DeleteFromHashTable(PlayerID) )
         {
             return FAILURE;
         }
    }
    catch(...){

    }
    auto* group_head = UnionGroups->findHead(groupId);
    if(player_level == 0){
        group_head->zerosLevel_scores[player_score] = group_head->zerosLevel_scores[player_score] -1;
        this->all_players_zerosLevel_scores[player_score]=this->all_players_zerosLevel_scores[player_score] -1 ;
    } else{
        try {
            this->AllPlayers->remove(ToRemove);
            RTgroupKey ToRemovefromgroup(PlayerID, groupId, player_score, player_level);
            group_head->GroupPlayers->remove(ToRemovefromgroup);
        }
        catch(...){

        }
    }
    return SUCCESS;

}
StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    int OLD_LEVEL;
    auto res=Players->ListArr[Players->Hash(PlayerID)]->find(PlayerID);
    if( res == nullptr)
    {
        return FAILURE;
    }
    else
    {
        OLD_LEVEL = res->key.level;
        res->key.level = res->key.level + LevelIncrease;  // increase level in hash
    }
    int group_id=res->key.groupId;
    int player_score = res->key.score;
    int player_new_level = res->key.level;

    RTgroupKey KeyToAdd(PlayerID,group_id,player_score,player_new_level);
    auto* DataToAdd=new RTgroupData(PlayerID,group_id,player_score);

    RTAllPlayersKey KeyToAddInAllPlayersTree(PlayerID,group_id,player_score,player_new_level);
    auto* DataToAddInAllPlayersTree=new RTAllPlayersData(PlayerID,res->key.groupId,res->key.score);

    auto* group_head = UnionGroups->findHead(group_id);

    if(OLD_LEVEL == 0){
        //didnt exist in the tree ,, means that the old level was 0
        group_head->zerosLevel_scores[player_score]-=1;
        this->all_players_zerosLevel_scores[player_score]-=1;
        try {
            group_head->GroupPlayers->InsertNode(KeyToAdd, DataToAdd);// o(log(n))
            this->AllPlayers->InsertNode(KeyToAddInAllPlayersTree, DataToAddInAllPlayersTree);
        }
        catch (...){
            std::cout<<std::endl<<"NAYAAAAAAAKATNAYAAAAAAAKATNAYAAAAAAAKATNAYAAAAAAAKAT"<<std::endl;

        }
    }else{
        // have been in the tree ,, means its level was greater than 0

        RTgroupKey ToRemovefromgroup(PlayerID,group_id,player_score,OLD_LEVEL);
        RTAllPlayersKey ToRemovefromAll(PlayerID,group_id,player_score,OLD_LEVEL);
        try {
            group_head->GroupPlayers->remove(ToRemovefromgroup);
            group_head->GroupPlayers->InsertNode(KeyToAdd, DataToAdd);

            this->AllPlayers->remove(ToRemovefromAll);
            this->AllPlayers->InsertNode(KeyToAddInAllPlayersTree, DataToAddInAllPlayersTree);
        }
        catch(...){


        }
    }
    return SUCCESS;

}
StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore) {
    //RTAllPlayersKey checkExists(PlayerID,0,0,0);
    auto res=Players->ListArr[Players->Hash(PlayerID)]->find(PlayerID);
    if( res == nullptr){
        return FAILURE;
    }
    int old_score=res->key.score;
    res->key.score=NewScore;

    auto* group_head = UnionGroups->findHead(res->key.groupId);

    RTgroupKey KeyToAdd(PlayerID,res->key.groupId,NewScore,res->key.level);
    RTAllPlayersKey KeyToAddInAllPlayersTree(PlayerID,res->key.groupId,NewScore,res->key.level);

    if(res->key.level==0){
        //didnt exist in the tree ==> includeed in scores-arrray

        group_head->zerosLevel_scores[old_score] -= 1;
        group_head->zerosLevel_scores[NewScore] += 1;

        this->all_players_zerosLevel_scores[old_score] -= 1;
        this->all_players_zerosLevel_scores[NewScore] += 1;

        /* auto* DataToAdd=new RTgroupData(PlayerID,res->key->groupId,res->key->score);
         group_head->GroupPlayers->InsertNode(KeyToAdd,DataToAdd);// o(log(n))
         auto* DataToAddInAllPlayersTree=new RTAllPlayersData(PlayerID,res->key->groupId,res->key->score);
         this->AllPlayers->InsertNode(KeyToAddInAllPlayersTree,DataToAddInAllPlayersTree);*/
    }else{
        // have been in the tree

        auto ptr=group_head->GroupPlayers->PointerToTheNode(group_head->GroupPlayers->head->leftSon,KeyToAdd);
        ptr->key.score=NewScore;// updated the score field in  the groups tree
        auto ptr2=this->AllPlayers->PointerToTheNode(this->AllPlayers->head->leftSon,KeyToAddInAllPlayersTree);
        ptr2->key.score=NewScore;// updated the score field in  AllPlayersTree tree
        while(ptr != nullptr){ // update the score histo in the tree
            ptr->score_histo[old_score]-=1;
            ptr->score_histo[NewScore]+=1;
            ptr = ptr->father;
        }
        while(ptr2 != nullptr){ // update the score histo in the tree
            ptr2->score_histo[old_score]-=1;
            ptr2->score_histo[NewScore]+=1;
            ptr2 = ptr2->father;
        }
    }
    return SUCCESS;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,double *players) {


    int players_with_level_smaller_than_high = 0;
    int players_with_level_smaller_than_low = 0;
    int players_with_score_and_level_smaller_than_high = 0;
    int players_with_score_and_level_smaller_than_low = 0;
    int players_in_the_range = 0;
    int players_with_score_in_the_range = 0;

    RTgroupKey lowerKeyToAdd(std::numeric_limits<int>::min(), GroupID, 0, lowerLevel);
    auto *lowerdataToAdd = new RTgroupData(std::numeric_limits<int>::min(), GroupID, 0);

    RTgroupKey higherKeyToAdd(std::numeric_limits<int>::max(), GroupID, 0, higherLevel);
    auto *higherdataToAdd = new RTgroupData(std::numeric_limits<int>::max(), GroupID, 0);



    RTAllPlayersKey lowerKeyToAddInAllPlayersTree(std::numeric_limits<int>::min(), GroupID, 0, lowerLevel);
    auto *lowerdataToAddInAllPlayersTree = new RTAllPlayersData(std::numeric_limits<int>::min(), GroupID, 0);

    RTAllPlayersKey higherKeyToAddInAllPlayersTree(std::numeric_limits<int>::max(), GroupID, 0, higherLevel);
    auto *higherdataToAddInAllPlayersTree = new RTAllPlayersData(std::numeric_limits<int>::max(), GroupID, 0);



    if (lowerLevel > 0 && higherLevel > 0) {
        try {
            this->AllPlayers->InsertNode(lowerKeyToAddInAllPlayersTree, lowerdataToAddInAllPlayersTree);
            this->UnionGroups->findHead(GroupID)->GroupPlayers->InsertNode(lowerKeyToAdd, lowerdataToAdd);

            this->AllPlayers->InsertNode(higherKeyToAddInAllPlayersTree, higherdataToAddInAllPlayersTree);
            this->UnionGroups->findHead(GroupID)->GroupPlayers->InsertNode(higherKeyToAdd, higherdataToAdd);

        }
        catch (...) {
            

        }
        players_with_level_smaller_than_high = GetPercent(higherKeyToAdd, higherKeyToAddInAllPlayersTree, GroupID,
                                                          score, &players_with_score_and_level_smaller_than_high) - 1;
// now the variable hold the number of nodes with value < higherkey

        players_with_level_smaller_than_low = GetPercent(lowerKeyToAdd, lowerKeyToAddInAllPlayersTree, GroupID, score,
                                                         &players_with_score_and_level_smaller_than_low);

        //-1 to exclude the min node

        players_in_the_range = players_with_level_smaller_than_high - players_with_level_smaller_than_low;
        // num of players [low,high]

        players_with_score_in_the_range = players_with_score_and_level_smaller_than_high - players_with_score_and_level_smaller_than_low;

        try {

            this->AllPlayers->remove(lowerKeyToAddInAllPlayersTree);
            this->UnionGroups->findHead(GroupID)->GroupPlayers->remove(lowerKeyToAdd);

            this->UnionGroups->findHead(GroupID)->GroupPlayers->remove(higherKeyToAdd);
            this->AllPlayers->remove(higherKeyToAddInAllPlayersTree);


        }
        catch (...) {
            
        }


        if (players_in_the_range == 0) {
            return FAILURE;
        }

        *players = ((double) players_with_score_in_the_range / (double) players_in_the_range) * 100;
        return SUCCESS;

    } else {
        if (lowerLevel <= 0 && higherLevel <= 0) {

            if (GroupID > 0) {
                players_in_the_range = getNumOfElemnts(this->UnionGroups->findHead(GroupID)->zerosLevel_scores, 200);
                //num of all level 0  group players

                players_with_score_in_the_range = this->UnionGroups->findHead(GroupID)->zerosLevel_scores[score];
                // num of group players with the given score with zero level

            } else {
                //groupid==0
                players_in_the_range =  getNumOfElemnts(this->all_players_zerosLevel_scores, 200);
                //num of all  players
                players_with_score_in_the_range = this->all_players_zerosLevel_scores[score];
            }

            if (players_in_the_range == 0) {

                delete higherdataToAddInAllPlayersTree;
                delete lowerdataToAddInAllPlayersTree;
                delete lowerdataToAdd;
                delete higherdataToAdd;

                return FAILURE;
            }

            *players = ((double) players_with_score_in_the_range / (double) players_in_the_range) * 100;

            delete higherdataToAddInAllPlayersTree;
            delete lowerdataToAddInAllPlayersTree;
            delete lowerdataToAdd;
            delete higherdataToAdd;

            return SUCCESS;
        }else {
            if (lowerLevel <= 0 && higherLevel > 0) {

                try {
                    this->AllPlayers->InsertNode(lowerKeyToAddInAllPlayersTree, lowerdataToAddInAllPlayersTree);
                    this->UnionGroups->findHead(GroupID)->GroupPlayers->InsertNode(lowerKeyToAdd, lowerdataToAdd);

                    this->AllPlayers->InsertNode(higherKeyToAddInAllPlayersTree, higherdataToAddInAllPlayersTree);
                    this->UnionGroups->findHead(GroupID)->GroupPlayers->InsertNode(higherKeyToAdd, higherdataToAdd);

                }
                catch (...) {
                    std::cout<<std::endl<<"NAYAAAAAAAKATNAYAAAAAAAKATNAYAAAAAAAKATNAYAAAAAAAKAT"<<std::endl;

                }
                ///////////////////////////////// positive //////////////////////////////////////////////////////////////////
                players_with_level_smaller_than_high = GetPercent(higherKeyToAdd, higherKeyToAddInAllPlayersTree, GroupID,
                                                                      score, &players_with_score_and_level_smaller_than_high)-1 ;
                    //num of all level 0  group players
                players_in_the_range = players_with_level_smaller_than_high;
                players_with_score_in_the_range = players_with_score_and_level_smaller_than_high;
                try {

                    this->AllPlayers->remove(lowerKeyToAddInAllPlayersTree);
                    this->UnionGroups->findHead(GroupID)->GroupPlayers->remove(lowerKeyToAdd);

                    this->UnionGroups->findHead(GroupID)->GroupPlayers->remove(higherKeyToAdd);
                    this->AllPlayers->remove(higherKeyToAddInAllPlayersTree);


                }
                catch (...) {
        
                }
                ///////////////////////////////// positive //////////////////////////////////////////////////////////////////



                ///////////////////////////////// zero levels //////////////////////////////////////////////////////////////////
                if (GroupID > 0) {
                    players_in_the_range += getNumOfElemnts(this->UnionGroups->findHead(GroupID)->zerosLevel_scores,
                                                            200);
                    //num of all level 0  group players
                    players_with_score_in_the_range += this->UnionGroups->findHead(GroupID)->zerosLevel_scores[score];
                } else {
                    //num of all level 0 players
                    players_with_score_in_the_range += this->all_players_zerosLevel_scores[score] ;
                    players_in_the_range += getNumOfElemnts(this->all_players_zerosLevel_scores,200);
                }


                if (players_in_the_range == 0) {
                    return FAILURE;
                }

                *players = ((double) players_with_score_in_the_range / (double) players_in_the_range) * 100;

                return SUCCESS;

            }
        }
        return SUCCESS;
    }
}













StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double *level) {

    int num_of_players_in_the_tree ;
    int num_of_players_with_zero_level;
    if(GroupID==0){

        num_of_players_in_the_tree = this->AllPlayers->size;
        num_of_players_with_zero_level = getNumOfElemnts(this->all_players_zerosLevel_scores , 200);

    }else{

        num_of_players_in_the_tree = this->UnionGroups->findHead(GroupID)->GroupPlayers->size;
        num_of_players_with_zero_level = getNumOfElemnts(this->UnionGroups->findHead(GroupID)->zerosLevel_scores , 200);
    }

    int num_of_players = num_of_players_in_the_tree + num_of_players_with_zero_level; //Total players in the prog

    if(num_of_players < m){
        return FAILURE;
    }

    if(num_of_players_in_the_tree == 0){
        *level=0;
        return SUCCESS;
    }

    if(m >= num_of_players_in_the_tree){ // nonzeroplayers <= m <= all players
        if( GroupID ==  0){
            *level = (double) this->AllPlayers->head->leftSon->levels_sum/m;
            return SUCCESS;
        }else{
            *level= (double) this->UnionGroups->findHead(GroupID)->GroupPlayers->head->leftSon->levels_sum/m;
            return SUCCESS;
        }
    }



    AvlNode<RTAllPlayersKey,RTAllPlayersData>* lowerInAll = nullptr;
    AvlNode<RTgroupKey,RTgroupData>* lowerInGroup = nullptr;
    if( GroupID == 0) {

        lowerInAll = this->AllPlayers->Select(num_of_players_in_the_tree - m + 1);

        auto tmp = this->AllPlayers->head->leftSon;
        while(tmp != nullptr){
            tmp= tmp->leftSon;
        }

    }else {

        lowerInGroup = this->UnionGroups->findHead(GroupID)->GroupPlayers->Select(num_of_players_in_the_tree - m + 1);

        auto tmp = this->UnionGroups->findHead(GroupID)->GroupPlayers->head->leftSon;
        while(tmp != nullptr){
            tmp= tmp->leftSon;
        }

    }


    int sum=0;

    if(lowerInGroup != nullptr) {

        try{

            //this->UnionGroups->findHead(GroupID)->GroupPlayers->InsertNode(lowerKeyToAdd, lowerdataToAdd);

            sum= GetSumInGroup(lowerInGroup->key,GroupID);

            //this->UnionGroups->findHead(GroupID)->GroupPlayers->remove(lowerKeyToAdd);

        }
        catch(...) {

        }

    }
    if(lowerInAll != nullptr) {

        try {

            //this->AllPlayers->InsertNode(lowerKeyToAddInAllPlayersTree, lowerdataToAddInAllPlayersTree);

            sum= GetSumInAll(lowerInAll->key,0);

            //this->AllPlayers->remove(lowerKeyToAddInAllPlayersTree);

        }
        catch (...) {

        }
    }



    int range=0;
    if(GroupID == 0){
        range=this->AllPlayers->head->leftSon->levels_sum;
    }else{
        range=this->UnionGroups->findHead(GroupID)->GroupPlayers->head->leftSon->levels_sum;
    }

    range -= sum;
    *level = (double) range/m ;
    return SUCCESS;

}


///bonus
StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m, int *LowerBoundPlayers, int *HigherBoundPlayers) {
    return FAILURE;
}///////

void PlayersManager::Quit() {

}

int PlayersManager::GetPercent(RTgroupKey& k1,RTAllPlayersKey& k2, int group_id, int score, int *num_of_elements_with_score) {
    int num_of_elements=0;
    *num_of_elements_with_score=0;

    if(group_id !=0){
        auto* ptr = this->UnionGroups->findHead(group_id)->GroupPlayers->head->leftSon;
        while ( ptr->key!=k1){
            if(ptr->key < k1 ){
                if(ptr->leftSon){
                    num_of_elements+=ptr->leftSon->rank;
                    *num_of_elements_with_score=*num_of_elements_with_score+ptr->leftSon->score_histo[score];
                }
                num_of_elements++;
                if(ptr->key.score == score){
                    *num_of_elements_with_score=*num_of_elements_with_score+1;
                }
                ptr=ptr->rightSon;
            }else{
                ptr=ptr->leftSon;
            }
        }
        if(ptr->leftSon){
            num_of_elements+=ptr->leftSon->rank;
            *num_of_elements_with_score=*num_of_elements_with_score+ptr->leftSon->score_histo[score];
        }

    }else{
        auto* ptr = this->AllPlayers->head->leftSon;
        while (ptr->key != k2){
            if(ptr->key < k2 ){
                if(ptr->leftSon){
                    num_of_elements+=ptr->leftSon->rank;
                    *num_of_elements_with_score=*num_of_elements_with_score+ptr->leftSon->score_histo[score];
                }
                num_of_elements++;
                if(ptr->key.score == score){
                    *num_of_elements_with_score=*num_of_elements_with_score+1;
                }
                ptr=ptr->rightSon;
            }else{
                ptr=ptr->leftSon;
            }
        }
        if(ptr->leftSon){
            num_of_elements+=ptr->leftSon->rank;
            *num_of_elements_with_score=*num_of_elements_with_score+ptr->leftSon->score_histo[score];
        }
    }

    return num_of_elements;
}
//        while (ptr != nullptr){
//            if(ptr->key < k2 && ptr->key != k2){

int PlayersManager::GetSumInGroup(RTgroupKey& k1 , int group_id )
{
    int sum=0;
    if(group_id !=0){
        auto* ptr = this->UnionGroups->findHead(group_id)->GroupPlayers->head->leftSon;
        while (ptr != nullptr && ptr->key!=k1){
            if(ptr->key < k1 ){
                if(ptr->leftSon){
                    sum+= ptr->leftSon->levels_sum;
                }
                sum+= ptr->key.level;
                ptr=ptr->rightSon;
            }else{
                ptr=ptr->leftSon;
            }
        }
        if(ptr != nullptr && ptr->leftSon != nullptr){
            sum+=ptr->leftSon->levels_sum;
        }
    }
    return sum;
}

int PlayersManager::GetSumInAll(RTAllPlayersKey &k2 , int group_id ) {
    int sum = 0;
    auto *ptr = this->AllPlayers->head->leftSon;
    while (ptr != nullptr && ptr->key!=k2) {
        if (ptr->key < k2 ) {
            if (ptr->leftSon) {
                sum += ptr->leftSon->levels_sum;
            }
            sum += ptr->key.level;
            ptr = ptr->rightSon;
        } else {
            ptr = ptr->leftSon;
        }
    }
    if(ptr != nullptr && ptr->leftSon != nullptr){
        sum+=ptr->leftSon->levels_sum;
    }

    return sum;
}


