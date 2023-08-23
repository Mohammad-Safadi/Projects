//
// Created by m7mds on 26/12/2021.
//

#ifndef DATASTRUCTURES_HW2_AVLNODE_H
#define DATASTRUCTURES_HW2_AVLNODE_H
//
// Created by m7mds on 09/05/2021.
//

#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H
#include <iostream>
using std::exception;
//using namespace std;
class secondNode;

template<class K ,class T>
class AvlNode{
public:
    K key;
    T* data;
    AvlNode *father;
    AvlNode *leftSon;
    AvlNode *rightSon;
    int Bf;
    int height;
    void updateBalanceFactor();
    int rank; // we save the number of the sons in the tree which this node is the root of it
    int levels_sum ; // we save the sum of the levels in the tree which this node is the root of it
    int* score_histo;
    // saving a histo which explain the scores of the children in the tree which this node is the root of it
    //////////// check if we should allocate the histo by new
    void BalanceRoll();


    // kant data(NULL)
    AvlNode() : key(), data(), father(NULL), leftSon(NULL), rightSon(NULL), Bf(0), height(0),rank(1),levels_sum(0),score_histo(new int[201]) {
        for(int i=0 ; i<201 ; i++){
            score_histo[i]=0;
        }
        //score_histo[0]++;
    }
//ZDT data(node.data)// kant bl2wal bdoon el data b3deen saweet data(node.data)
////////////check if the rank should be 1 or equal to node rank
    AvlNode(AvlNode<K ,T>& node):key(node.key),data(new T(*node.data)), father(node.father), leftSon(node.leftSon), rightSon(node.rightSon), Bf(node.Bf), height(node.Bf)
            ,rank(node.rank) , levels_sum(levels_sum),score_histo(new int[201]){
        for(int i=0 ; i<201 ; i++){
            this->score_histo[i] = node.score_histo[i];
        }
    }

    int operator+(K& add){
        return (this->level)+add;
    }

    /////////check the implementation of the levels_sum and the score_histo updates
    void updateRanknode() {
        auto current = this;
        int leftRank = 0, rightRank = 0;
        int left_levels_sum = 0, right_levels_sum = 0;
        for (int i = 0; i < 201; ++i) {
            current->score_histo[i] = 0;
        }
        if (current->leftSon != nullptr) {
            leftRank = current->leftSon->rank;
            left_levels_sum = current->leftSon->levels_sum;
            for (int i = 0; i < 201; ++i) {
                current->score_histo[i] += current->leftSon->score_histo[i];
                //// something wrong in the histo updating , check it
            }
        }
        if (current->rightSon) {
            rightRank = current->rightSon->rank;
            right_levels_sum = current->rightSon->levels_sum;
            for (int i = 0; i < 201; ++i) {
                current->score_histo[i] += current->rightSon->score_histo[i];
            }
        }

//        if(!current->rightSon && !current->leftSon) {
        current->score_histo[current->key[0]] += 1;

        int sum2 = (left_levels_sum + right_levels_sum);
        current->levels_sum = current->key + sum2;
        current->rank = leftRank + rightRank + 1;
    }



    void updateRank(){
        auto current = this;
        while (current != nullptr) {
            int leftRank = 0, rightRank = 0;
            int left_levels_sum = 0 , right_levels_sum = 0;

            if (current->leftSon != nullptr) {

                leftRank = current->leftSon->rank;
                left_levels_sum = current->leftSon->levels_sum;
                for (int i = 0; i < 201; ++i) {
                    current->score_histo[i]+=current->leftSon->score_histo[i];
                    //// something wrong in the histo updating , check it
                }
            }
            if (current->rightSon) {

                rightRank = current->rightSon->rank;
                right_levels_sum = current->rightSon->levels_sum;
                for (int i = 0; i < 201; ++i) {
                    current->score_histo[i]+=current->rightSon->score_histo[i];
                }
            }
            int sum2 = (left_levels_sum+right_levels_sum);
            current->levels_sum = current->key+sum2;
            current->rank = leftRank + rightRank + 1;
            current = current->father;
        }

    }

    void LL();
    void LR();
    void RL();
    void RR();
    //kant data(NULL)
    explicit AvlNode( K key1, AvlNode *father = NULL) :key(key1), data(NULL) ,leftSon(NULL), rightSon(NULL), Bf(0), height(0),rank(1),score_histo(new int[201]) {
        this->father = father;
        for (int i = 0; i < 201; i++) {
            score_histo[i]=0;
        }
        //score_histo[key[0]]=1;

    }

    template<class func>
    void InOrderFuncByReference( func& apply) ;

    friend std::ostream& operator<<(std::ostream& os,AvlNode<K,T>* toP){
        os<<*toP->data << " " << toP->Bf << " " << toP->height <<std::endl;
        return os;
    }
    AvlNode* getleft()
    {
        return this->leftSon;
    }
    AvlNode* getright()
    {
        return this->rightSon;
    }
    AvlNode& operator=( AvlNode& node)
    {
        if( &node != this) {
            this->key=node.key;//hay zdtha
            this->data = node.data;
            this->father = node.father;
            this->leftSon = node.leftSon;
            this->rightSon = node.rightSon;
            this->Bf = node.Bf;
            this->height = node.height;
            return *this;
        }
        return *this;
    }
    int get_height() {
        return height;
    }
    void setheight(int newH)
    {
        this->height = newH;
    }
    ~AvlNode(){
        delete data;
        delete[] score_histo;
//        if(strcmp(typeid(this->key).name(),"int") != 0 ){
//            delete &key;
//        }
    }
    void updateHeight()
    {
//        if(this==nullptr){
//            return;
//        }
        if(rightSon == NULL)
        {
            if(leftSon != NULL)
            {
                height = leftSon->height + 1;
                return;
            }
            else{ // leftson && rightson ==NUll
                height = 0;
                return;
            }
        }
        if(leftSon==NULL && rightSon!=NULL){
            height=rightSon->height+1;
            return;
        }
        // leftson && rightson !=NUll
        int heightnew = max(this->rightSon->height , this->leftSon->height);
        this->height = heightnew+1;
    }
    void operator++(){
        data++;
    }
    bool operator!=(AvlNode& other) {
        return *this->data != *other.data;
    }
    bool operator==(AvlNode& other) {
        return *this->data == *other.data;
    }
    bool operator<(AvlNode &other) {
        return *this->data < *other.data;
    }

    bool operator>(AvlNode &other) {
        return *this->data > *other.data;
    }

    void setfather(AvlNode *father1) {
        * this->father = *father1;
    }

    void setleftson(AvlNode *leftson1) {
        *this->leftSon = *leftson1;
    }
    void setrightson(AvlNode *rightson1) {
        *this->rightSon = *rightson1;
    }
    int max(int leftheight, int rightheight) {
        if (leftheight > rightheight) {
            return leftheight;
        }

        return rightheight;
    }
};
template <class K,class D>
class GetNodes{
    AvlNode<K,D>** NodesTree;
    int index;
public:
    explicit GetNodes(AvlNode<K,D>** A,int a):NodesTree(A),index(a){
    }
    void operator ()(AvlNode<K,D>* ptr){
        NodesTree[index]=ptr;
        index++;
    }
};

template <class K , class T>
template <class func>
void AvlNode<K,T>::InOrderFuncByReference( func& apply) {
    if (this->leftSon) {
        leftSon->InOrderFuncByReference(apply);
    }
    apply(this);
    if (rightSon) {
        rightSon->InOrderFuncByReference(apply);
    }
}

template <class K ,class T>
void AvlNode<K,T>::updateBalanceFactor(){
//    if (this == nullptr) {
//        return;
//    }
    int leftHeight, rightHeight;
    if (leftSon == nullptr) {
        leftHeight = 0;
    } else {
        leftHeight = leftSon->height+1;
    }
    if (rightSon == nullptr) {
        rightHeight = 0;
    } else {
        rightHeight = rightSon->height+1;
    }
    this->Bf = leftHeight - rightHeight;
}
template<class K,class T>
void AvlNode<K,T>::BalanceRoll() {

    if (Bf == 2 && leftSon->Bf >= 0) {
        LL();
    }
    if (Bf == 2 && leftSon->Bf == -1) {
        LR();
    }
    if (Bf == -2 &&  rightSon->Bf <= 0) {
        RR();
    }
    if (Bf == -2 && rightSon->Bf == 1) {
        RL();
    }
}
template <class K,class T>
void AvlNode<K,T>::RR() {
    AvlNode<K,T> *oldRight = this->rightSon;
    this->rightSon = this->rightSon->leftSon;
    if (this->rightSon != NULL) {
        this->rightSon->father = this;
    }
    AvlNode<K,T> *oldfather = this->father;
    if (oldRight != NULL) {
        oldRight->leftSon = this;
    }
    this->father = oldRight;
    if (oldRight != NULL) {
        oldRight->father = oldfather;
    }
    if (oldRight != NULL) {
        if (oldRight->father->rightSon == this) {
            oldRight->father->rightSon = oldRight;
        } else {
            oldRight->father->leftSon = oldRight;
        }
    }

    this->updateRanknode();
    this->updateHeight();
    this->updateBalanceFactor();
}
template < class K,class T>
void AvlNode<K,T>::RL()
{
    this->rightSon->LL();
    RR();
}
template < class K,class T>
void AvlNode<K,T>::LR() {
    leftSon->RR();
    LL();
}
template< class K ,class T>
void AvlNode<K,T>::LL(){
    AvlNode<K,T> *oldLeft = this->leftSon;
    this->leftSon = this->leftSon->rightSon;
    if (this->leftSon != NULL) {
        this->leftSon->father = this;
    }
    AvlNode<K,T> *oldfather = this->father;
    if (oldLeft != NULL) {
        oldLeft->rightSon = this;
    }
    this->father = oldLeft;
    if (oldLeft != NULL) {
        oldLeft->father = oldfather;
    }
    if (oldLeft != NULL ) {
        if (oldLeft->father->leftSon == this) {
            oldLeft->father->leftSon = oldLeft;
        } else {
            oldLeft->father->rightSon = oldLeft;
        }
    }
    this->updateRanknode();
    this->updateHeight();
    this->updateBalanceFactor();
}




#endif //WET1_AVLTREE_H
#endif //DATASTRUCTURES_HW2_AVLNODE_H
