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
    void BalanceRoll();
    AvlNode() : key(), data(NULL), father(NULL), leftSon(NULL), rightSon(NULL), Bf(0), height(0) {
    }

    AvlNode(AvlNode<K ,T>& node):key(node.key), father(node.father), leftSon(node.leftSon), rightSon(node.rightSon), Bf(node.Bf), height(node.Bf)
    {
    }

    void LL();
    void LR();
    void RL();
    void RR();
    explicit AvlNode( K key1, AvlNode *father = NULL) :key(key1), data(NULL) ,leftSon(NULL), rightSon(NULL), Bf(0), height(0)  {
        this->father = father;
    }

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
    }
    void updateHeight()
    {
        if(this==NULL){
            return;
        }
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
template <class K ,class T>
void AvlNode<K,T>::updateBalanceFactor(){
    if (this == NULL) {
        return;
    }
    int leftHeight, rightHeight;
    if (leftSon == NULL) {
        leftHeight = 0;
    } else {
        leftHeight = leftSon->height+1;
    }
    if (rightSon == NULL) {
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
    this->updateHeight();
    this->updateBalanceFactor();
}

#endif //WET1_AVLTREE_H