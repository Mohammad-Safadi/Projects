//
// Created by tamer on 5/18/2021.
//

#include "CarManager.h"

StatusType CarManager::ADDCartype(int typeID, int numOfModels) {
    if(typeID <= 0 || numOfModels <= 0 ){
        return INVALID_INPUT;
    }
    firstNode* car1 = new firstNode(typeID,numOfModels);
    try{
        treeOne->InsertNode(typeID,car1);
    }
    catch (ALREADY_EXISTS& e){
        delete car1;
        return FAILURE;
    }
    catch(std::bad_alloc& e){
        delete car1;
        return ALLOCATION_ERROR;
    }
    AvlTree<int,ZeroNode>* graph1 = new AvlTree<int,ZeroNode>();
    ToInitZero*  Init = new ToInitZero(typeID,numOfModels);
    graph1->head->leftSon = new AvlNode<int , ZeroNode>(-1 , graph1->head);
    graph1->head->leftSon->father = graph1->head;
    BuildAvl(graph1->head->leftSon,numOfModels-1);
    graph1->InOrder(graph1->head->leftSon,Init);
    delete Init;
    thirdNode* node3 = new thirdNode(typeID,graph1);
    try {
        treeThird->InsertNode(typeID,node3);
    }
    catch (ALREADY_EXISTS& e){
        return FAILURE;
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    sellKey sellkey(typeID,0,0);
    sellnode* car2 = new sellnode(0,typeID,0);
    try{

        treeFour->InsertNode(sellkey,car2);
    }

    catch (ALREADY_EXISTS& e){
        delete car1;
        delete  car2;
        return FAILURE;
    }
    catch(std::bad_alloc& e){
        delete car1;
        delete car2;
        return ALLOCATION_ERROR;
    }

   if(maxsell->sellcounter == -1) {
       if (maxsell->typeId == -1) {
           maxsell->modelId = 0;
           maxsell->sellcounter = 0;
           maxsell->typeId = typeID;
           maxsell->flag = true;
       }
       else{
       if (typeID < maxsell->typeId) {
           maxsell->modelId = 0;
           maxsell->sellcounter = 0;
           maxsell->typeId = typeID;
           maxsell->flag = true;
       } else {
           if (typeID == maxsell->typeId) {
               if (0 < maxsell->modelId) {
                   maxsell->modelId = 0;
                   maxsell->sellcounter = 0;
                   maxsell->typeId = typeID;
                   maxsell->flag = true;
               }
           }
         }
       }
   }

   AllModulesSize+=numOfModels;
    return SUCCESS;
}
StatusType CarManager::RemoveCarType(int typeID) {
    if (typeID <= 0) {
        return INVALID_INPUT;
    }
    AvlNode<int, firstNode> *node = treeOne->PointerToTheNode(treeOne->head->getleft(), typeID);
    if (node == NULL) {
        return FAILURE;
    }
    auto car2 = treeOne->PointerToTheNode(treeOne->head->getleft(), typeID);
    if (car2 == NULL) {
        return FAILURE;
    }
    for (int i = 0; i < car2->data->getModelNum(); i++) {
        secondKey *ptr = car2->data->getFromArray(i);
        if (ptr != NULL) {
            try {
                treeSecond->remove(*ptr);
                delete ptr;
                car2->data->setInArray(i, NULL);
            }
            catch (DOESNT_EXISTS &e) {
                return FAILURE;
            }
            catch (std::bad_alloc &e) {
                return ALLOCATION_ERROR;
            }
        }
    }
    int numOfModules = node->data->modulesNum;
    int Maxid = node->data->Maxmoduletype;
    int maxSales = node->data->maxsales;
    AvlNode<int, thirdNode> *node3 = treeThird->PointerToTheNode(treeThird->head->getleft(),
                                                                 typeID); // searchfornode 2
    if (node3 != NULL) {
        try {
            treeThird->remove(typeID);
        }
        catch (DOESNT_EXISTS &e) {
            return FAILURE;
        }
        catch (std::bad_alloc &e) {
            return ALLOCATION_ERROR;
        }
    }
    try {
        treeOne->remove(typeID);
    }
    catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (DOESNT_EXISTS &e) {

        return FAILURE;
    }
    sellKey sellKey(typeID, Maxid, maxSales);
    try {
        treeFour->remove(sellKey);
    }
    catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (DOESNT_EXISTS &e) {
        return FAILURE;
    }
    if (maxsell->typeId == typeID) {
        sellnode *tmp = getmax(treeFour);
        if(tmp != NULL) {
            maxsell->typeId = tmp->typeId;
            maxsell->modelId = tmp->modelId;
            maxsell->sellcounter = tmp->sellcounter;
        }
        else {
            maxsell->typeId = -1;
            maxsell->modelId = -1;
            maxsell->sellcounter = -1;
        }
    }
    AllModulesSize -= numOfModules;
    return SUCCESS;
}

StatusType CarManager::SellCar(int typeID, int modelID) {
    int tmpSales = 0;
    if(typeID<= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }

    AvlNode<int,firstNode>* tmpnode = treeOne->PointerToTheNode(treeOne->head->getleft(),typeID);
    if(tmpnode == NULL)
    {
        return FAILURE;
    }

    if(modelID >= tmpnode->data->getModelNum()) {
        return FAILURE;
    }
    secondKey* key2 = tmpnode->data->getFromArray(modelID);
    if(key2 != NULL){
        AvlNode<secondKey,secondNode>* tree2Node = treeSecond->PointerToTheNode(treeSecond->head->leftSon,*key2);
        secondNode *newdata = new secondNode();
        *newdata = *tree2Node->data;
        secondKey* tmpkey = new secondKey() ;
        *tmpkey = tree2Node->key;
        tmpSales = tree2Node->data->getSales();
        try {
            treeSecond->remove(*tmpkey);
        }
        catch(std::bad_alloc& e) {
            delete newdata;
            delete tmpkey;
            return ALLOCATION_ERROR;
        }
        catch(DOESNT_EXISTS& e){
            delete newdata;
             delete tmpkey;
            return FAILURE;
        }
        delete key2;
        tmpnode->data->setInArray(modelID,NULL);
        newdata->increas();
        tmpkey->rating += 10;
        if(tmpkey->rating == 0)
        {
            int sellCount = newdata->sellCounter;
            ZeroNode* toAddNode = new ZeroNode(typeID,modelID,sellCount);
            AvlNode<int,thirdNode>* toAdd = treeThird->PointerToTheNode(treeThird->head->leftSon,typeID);
            AvlTree<int,ZeroNode>* tmpgraph =  toAdd->data->getTree();
            try {
                if(tmpgraph->PointerToTheNode(tmpgraph->head->leftSon,modelID) != NULL) {
                    tmpgraph->remove(modelID);
                }
                tmpgraph->InsertNode(modelID, toAddNode);
            }    catch (ALREADY_EXISTS& e){
                delete toAddNode;
                delete newdata;
                delete tmpkey;
                return FAILURE;
            }
            catch(std::bad_alloc& e){
                delete toAddNode;
                delete newdata;
                delete tmpkey;
                return ALLOCATION_ERROR;
            }
            delete newdata;
            delete tmpkey;
            delete tmpnode->data->arrayptr[modelID];
            tmpnode->data->setInArray(modelID,NULL);
        } else {
            try {
                treeSecond->InsertNode(*tmpkey, newdata);
               if(tmpnode->data->arrayptr[modelID] != NULL){
                    delete tmpnode->data->arrayptr[modelID];
                    tmpnode->data->arrayptr[modelID] = NULL;
                }
                tmpnode->data->setInArray(modelID, tmpkey);
            }
            catch (ALREADY_EXISTS& e){
                delete newdata;
                delete tmpkey;
                return FAILURE;
            }
            catch(std::bad_alloc& e){
                delete newdata;
                delete tmpkey;
                return ALLOCATION_ERROR;
            }
        }
    }
    else {
        AvlNode<int,thirdNode> *tree3Node = treeThird->PointerToTheNode(treeThird->head->getleft(), typeID);
        AvlTree<int,ZeroNode> *treeN = tree3Node->data->getTree();
        AvlNode<int,ZeroNode> *found1 = treeN->PointerToTheNode(treeN->head->getleft(), modelID);
        tmpSales = found1->data->getcount();
        int sellCount = found1->data->getcount();
        try {
            treeN->remove(modelID);

        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(DOESNT_EXISTS& e){

            return FAILURE;
        }
        secondKey* addkey = new secondKey(typeID,modelID,10);
        secondNode*  newNode = new secondNode(typeID, modelID, 10, sellCount + 1);
        try {
            treeSecond->InsertNode(*addkey, newNode);
        }
        catch (ALREADY_EXISTS& e){
            return FAILURE;
        }
        catch(std::bad_alloc& e){
            return ALLOCATION_ERROR;
        }
       if(tmpnode->data->arrayptr[modelID] != NULL){
            delete tmpnode->data->arrayptr[modelID];
            tmpnode->data->arrayptr[modelID] = NULL;
        }
        tmpnode->data->setInArray(modelID, addkey);

    }

    tmpSales+=1;
    if(tmpnode->data->checkmaxid(modelID,tmpSales))
    {
        int oldsale = tmpnode->data->getSale();
        int oldId = tmpnode->data->getId();
        tmpnode->data->setMax(modelID,tmpSales);
        sellKey sellkey(typeID,oldId,oldsale);
        try {
            treeFour->remove(sellkey);
        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(DOESNT_EXISTS& e){

            return FAILURE;
        }
        sellnode* tmpnew = new sellnode(tmpSales,typeID,modelID);
        sellKey sellkey1(typeID,modelID,tmpSales);
        try {
            treeFour->InsertNode(sellkey1, tmpnew);
        }
        catch (ALREADY_EXISTS& e){
            return FAILURE;
        }
        catch(std::bad_alloc& e){
            return ALLOCATION_ERROR;
        }
    }
    if(tmpSales > maxsell->sellcounter)
    {
        maxsell->modelId= modelID;
        maxsell->sellcounter = tmpSales;
        maxsell->typeId = typeID;
        maxsell->flag = true;
        return SUCCESS;
    }
    if(tmpSales == maxsell->sellcounter) {
        if (typeID < maxsell->typeId) {
            maxsell->modelId = modelID;
            maxsell->sellcounter = tmpSales;
            maxsell->typeId = typeID;
            maxsell->flag = true;
            return SUCCESS;
        }
        if (typeID == maxsell->typeId) {
            if (modelID < maxsell->modelId) {
                maxsell->modelId = modelID;
                maxsell->sellcounter = tmpSales;
                maxsell->typeId = typeID;
                maxsell->flag = true;
                return SUCCESS;
            }
        }

    }
    return SUCCESS;
}

StatusType CarManager::MakeComplaint(int typeID, int modelID, int t) {

    if(typeID<= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }

    AvlNode<int,firstNode>* node = treeOne->PointerToTheNode( treeOne->head->getleft() ,typeID); // kman searchfornode
    if(node == NULL)
    {
        return FAILURE;
    }
    if(modelID >= node->data->getModelNum()) {
        return FAILURE;
    }
    secondKey* tree2Key = node->data->getFromArray(modelID);
    if(tree2Key != NULL ){
        AvlNode<secondKey,secondNode>* tree2Node = treeSecond->PointerToTheNode(treeSecond->head->leftSon,*tree2Key);
        secondNode* newdata = new secondNode();
        *newdata = *tree2Node->data;
        secondKey* tmpkey = new secondKey();
        *tmpkey  = tree2Node->key;
        try {
            treeSecond->remove(*tmpkey);
          }
        catch(std::bad_alloc& e) {
            delete newdata;
            delete tmpkey;
            return ALLOCATION_ERROR;
        }
        catch(DOESNT_EXISTS& e){
            delete newdata;
            delete tmpkey;
            return FAILURE;
        }
        newdata->decressRating(t);
        tmpkey->rating -= 100/t;
        if(tmpkey->rating == 0)
        {
            int sellCount = newdata->sellCounter;
            ZeroNode* toAddNode = new ZeroNode(typeID,modelID,sellCount);
            AvlNode<int,thirdNode>* toAdd = treeThird->PointerToTheNode(treeThird->head->leftSon,typeID);
            AvlTree<int,ZeroNode>* tmpgraph =  toAdd->data->getTree();
            try {
                if(tmpgraph->PointerToTheNode(tmpgraph->head->leftSon,modelID) != NULL) {
                    tmpgraph->remove(modelID);
                }
                tmpgraph->InsertNode(modelID, toAddNode);
            }
            catch (ALREADY_EXISTS& e){
                delete newdata;
                delete toAddNode;
                delete tmpkey;
                return FAILURE;
            }
            catch(std::bad_alloc& e){
                delete newdata;
                delete toAddNode;
                delete tmpkey;
                return ALLOCATION_ERROR;
            }
            delete newdata;
            delete tmpkey;
            delete tree2Key;
            node->data->setInArray(modelID,NULL);
        } else {
            try {
                treeSecond->InsertNode(*tmpkey, newdata);
            }
            catch (ALREADY_EXISTS& e){
                delete newdata;
                delete tmpkey;
                return FAILURE;
            }
            catch(std::bad_alloc& e){
                delete newdata;
                delete tmpkey;
                return ALLOCATION_ERROR;
            }
            if(node->data->arrayptr[modelID] != NULL)
            {
                delete node->data->arrayptr[modelID];
                node->data->arrayptr[modelID] = NULL;
            }
            node->data->setInArray(modelID, tmpkey);
        }
    } else {
        AvlNode<int,thirdNode> *tree3Node = treeThird->PointerToTheNode(treeThird->head->getleft(), typeID); // search 2
        AvlTree<int,ZeroNode> *treeN = tree3Node->data->getTree();
        AvlNode<int,ZeroNode> *found1 = treeN->PointerToTheNode(treeN->head->getleft(), modelID); // search 2
        int sellCount = found1->data->getcount();
        try {
            treeN->remove(modelID);
        }
        catch(std::bad_alloc& e) {
            return ALLOCATION_ERROR;
        }
        catch(DOESNT_EXISTS& e){
            return FAILURE;
        }
        int complain = 100 / t;
        secondKey* ToADD = new secondKey(typeID,modelID,-complain);
        secondNode* newNode = new secondNode(typeID, modelID, -complain, sellCount);
        try {
            treeSecond->InsertNode(*ToADD, newNode);
        }
        catch (ALREADY_EXISTS& e){
            delete ToADD;
            delete newNode;
            return FAILURE;
        }
        catch(std::bad_alloc& e){
            delete ToADD;
            delete newNode;
            return ALLOCATION_ERROR;
        }
       if(node->data->arrayptr[modelID] != NULL)
        {
            delete node->data->arrayptr[modelID];
            node->data->arrayptr[modelID] = NULL;
        }
        node->data->setInArray(modelID, ToADD);
    }
    return SUCCESS;
}


StatusType CarManager::GetBestSellerModelByType(int typeID, int *modelID) {

    if(this->treeOne->size == 0 && typeID==0){
        return FAILURE;
    }
    if(typeID == 0)
    {
        *modelID = maxsell->modelId;
        return SUCCESS;
    }
    if(treeOne->head->getleft() == NULL){
        return FAILURE;
    }
    AvlNode<int,firstNode>* tmp1 = treeOne->PointerToTheNode(treeOne->head->getleft(),typeID);
    if(tmp1 == NULL)
    {
        return  FAILURE;
    }
    *modelID = tmp1->data->getId();
    return SUCCESS;
}

StatusType CarManager::GetWorstModels(int numOfModels, int *types, int *models) {
    if(numOfModels <= 0)
    {
        return  INVALID_INPUT;
    }
    if(numOfModels > AllModulesSize){
        return FAILURE;
    }
    int originalCount=numOfModels;
    auto saveW = new ToSaveWorst(numOfModels);
    saveW->sizeZeroToCmp = treeThird->size;
    saveW->counterpassed = 1;
    saveW->CountedInOrder(treeSecond->head->leftSon,saveW,saveW->flag);
    saveW->flag = true;
    saveW->CountedInOrder(treeThird->head->leftSon,saveW,saveW->flagTheReturn);
    saveW->flag = true;
    saveW->flagTheReturn = false;
    if(saveW->getcounter() < originalCount) {
        saveW->CountedInOrder(saveW->ptr, saveW,true);
    }
    saveW->done = true;
    saveW->counterpassed--;
    saveW->flag = true;
    saveW->counterS = 1;
    saveW->CountedInOrder(treeSecond->head->leftSon,saveW ,true);
    saveW->transport(types,models);
    delete saveW;
    return SUCCESS;
}

sellnode *CarManager::getmax(AvlTree<sellKey,sellnode>* tree4) {
    AvlNode<sellKey,sellnode>* iterator = tree4->head->getleft();
    if(iterator == NULL){
        return NULL;
    }
    while (iterator ->getright() != NULL)
    {
        iterator = iterator->getright();
    }

    return iterator->data;
}

void CarManager::BuildAvl( AvlNode<int,ZeroNode>* node , int m ) {
    if(m==0){
        return;
    }
    if(m == 1){
        node->leftSon= new AvlNode<int,ZeroNode>(-1, node);
        node->leftSon->father=node;
        return;
    }
    if(m==2){
        node->leftSon= new AvlNode<int,ZeroNode>(-1, node);
        node->leftSon->father=node;
        node->rightSon = new AvlNode<int,ZeroNode>(-1, node);
        node->rightSon->father=node;
        return;
    }
    node->leftSon = new AvlNode<int,ZeroNode>(-1, node);
    node->leftSon->father=node;
    if(m%2==1) {
        BuildAvl(node->leftSon, (m/2));
    }
    else{
        BuildAvl(node->leftSon, (m/2)-1);
    }
    node->rightSon = new AvlNode<int,ZeroNode>(-1, node);
    node->rightSon->father=node;
    BuildAvl(node->rightSon,(m/2)-1);
}
