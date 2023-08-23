//
// Created by tamer on 5/18/2021.
//

#ifndef UNTITLED_CARMANAGER_H
#define UNTITLED_CARMANAGER_H

#include "Nodetype.h"
#include "library.h"
class CarManager;
class ToSaveWorst{
public:
    int* type;
    int* models;
    int size;
    bool flag;
    int counter;
    int sizeZero;
    int sizeZeroToCmp;
    bool flagTheReturn;
    bool done;
    bool doneZero;
    int counterS;
    int counterpassed;
    AvlNode<secondKey,secondNode>* ptr;

    ~ToSaveWorst(){
        delete[] type;
        delete[] models;
    }

    ToSaveWorst(int size) :  size(size) ,flag(true) ,counter(0)  ,sizeZero(0),sizeZeroToCmp(0) ,flagTheReturn(true) ,done(false),doneZero(
            false),counterS(size)  ,counterpassed(0) ,ptr(NULL)
    {
        if(size > 0) {
            type = new int[size];
            models = new int[size];
        }
    }

    void InOrderM(AvlNode<int,ZeroNode>* node ){
        if (!node)
        {
            return;
        }
        if(counter == size)
        {
            doneZero = true;
            return;
        }
        if(!doneZero) {
            if(node->leftSon != NULL) {
            InOrderM(node->getleft() );
            }
            if(!doneZero) {
                type[counter] = node->data->getId();
                models[counter] = node->data->getmodel();
                counter++;
                counterS--;
                if(counter == size)
                {
                    doneZero = true;
                    return;
                }
            }
            if(!doneZero) {
                if (node->rightSon != NULL) {
                    InOrderM(node->getright());
                }
            }
        }
    }

    void transport(int* types1 , int* models1){
        for(int i=0; i<size; i++)
        {
            types1[i] = type[i];
            models1[i] = models[i];
        }
    }


    int getcounter()
    {
        return counter;
    }

    template<class K,class D,class func>
    void CountedInOrder(AvlNode<K,D> *node, func* apply,bool flagP) {
        if (!node  || counterS <= 0 || counterpassed <= 0 || !flag ) {
            return;
        }
        if (node->leftSon != NULL) {
            CountedInOrder(node->getleft(), apply, flagP);
        }
        (*apply)(node);

        if (node->rightSon != NULL) {
            CountedInOrder(node->getright(), apply, flagP);
        }
    }
    void operator()(AvlNode<secondKey,secondNode>* node){
        if( node->data != NULL && node->data->getrating() > 0 && flagTheReturn && !done){
            ptr = node;
            flag = false;
        }
        if(node->data != NULL && flag && !node->data->passed && !done)
        {
            if(counter < size) {
                type[counter] = node->data->getId();
                models[counter] = node->data->getmodel();
                node->data->setflag();
                counter++;
                counterS--;
                counterpassed++;
            }
        }
        if(done){
            counterpassed--;
            node->data->passed = false;
        }
    }

    void operator()(AvlNode<int,thirdNode>* node){
        AvlTree<int,ZeroNode>* graph1 = node->data->getTree();
        if(sizeZero > sizeZeroToCmp) return;
        sizeZero++;
        if(graph1 != NULL)
        {
            AvlNode<int,ZeroNode>* tmphead = graph1->head->leftSon;
            InOrderM(graph1->head->getleft());
            graph1->head->leftSon = tmphead;
        }
    }

    int getsizeZero(){
        return sizeZero;
    }
    void setflag(){
        flag = true;
    }
    bool getflag(){
        return flag;
    }
};

class CarManager {
private:
    AvlTree<int,firstNode>* treeOne;
    AvlTree<secondKey,secondNode>* treeSecond;
    AvlTree<int,thirdNode>* treeThird;
    AvlTree< sellKey,sellnode>* treeFour;
    sellnode* maxsell;
    int AllModulesSize;
public:
    CarManager(): treeOne(new AvlTree<int,firstNode>()) , treeSecond(new AvlTree<secondKey,secondNode>())
            , treeThird(new AvlTree<int,thirdNode>()),
                  treeFour(new AvlTree<sellKey,sellnode>()) , maxsell(new sellnode(-1,-1,-1)),AllModulesSize(0){}
    ~CarManager(){
        delete treeOne;
        delete treeSecond;
        delete treeThird;
        delete treeFour;
       // treeOne->ClearTree();
        //treeSecond->ClearTree();
        //treeThird->ClearTree();
        //treeFour->ClearTree();
        delete maxsell;
    }
    static sellnode* getmax(AvlTree<sellKey,sellnode>* tree4);
    static void BuildAvl(AvlNode<int,ZeroNode>* node ,int m );
    StatusType ADDCartype(int typeID, int numOfModels);
    StatusType RemoveCarType(int typeID);
    StatusType SellCar(int typeID , int modelID);
    StatusType MakeComplaint(int typeID , int modelID , int t);
    StatusType GetBestSellerModelByType(int typeID, int *modelID);
    StatusType GetWorstModels(int numOfModels , int *types, int *models);





};

class ToInitZero{
public:
    int ZeroCounter;
    int typeID;
    int ToStop;
    ToInitZero(int typeID , int m):ZeroCounter(0),typeID(typeID) ,ToStop(m){}
    ~ToInitZero() = default;
    void operator()(AvlNode<int , ZeroNode>* node)
    {
        if(ZeroCounter < ToStop) {
            ZeroNode *data = new ZeroNode(typeID, ZeroCounter, 0);
            node->data = data;
            node->key = ZeroCounter;
            ZeroCounter++;
        }

    }
};

#endif //UNTITLED_CARMANAGER_H
