//
// Created by m7mds on 16/05/2021.
//
#ifndef SHAJARA_AVLTREE_H
#define SHAJARA_AVLTREE_H
#include "Exceptions.h"
#include "AvlNode.h"


class removeT{
public:
    template<class K , class D>
    void operator()(AvlNode<K,D>* node)
    {
        delete node;
        node = NULL;
    }
};
template <class K , class D>
class AvlTree{
public:
    AvlNode<K,D>* head;
    // friend class AvlNode<K,D>;
    int size;

    //el copy ctor 3'ltt**************************************
    AvlTree(const AvlTree<K ,D>* copyTree):head(new AvlNode<K,D>()),size(0){
        AvlNode<K,D>* temp=copyTree->head;
        temp=temp->leftSon;
        this->CopyInsert(temp);// #+copyInsert#
    }

    void CopyInsert(AvlNode<K,D>* Node){
        if(Node == NULL)
            return;
        if (Node->leftSon) {
            CopyInsert(Node->leftSon);
        }
        this->InsertNode(*Node->key);
        if (Node->rightSon) {
            CopyInsert(Node->rightSon);
        }
    }
    AvlTree():size(0) {
        head = new AvlNode<K ,D>();
    }
    AvlNode<K,D>* PointerToTheNode(AvlNode<K,D> *Node, K& key);
    D* PointerToTheNode2(AvlNode<K,D> *Node,K& key);

    template<class func>
    void CountedInOrder(AvlNode<K ,D> *node, func* apply,int* count,bool flag);

    ~AvlTree(){
        postOrder(head->leftSon,removeT());
        head->leftSon = NULL;
        delete head;
        head=NULL;
        size=0;
    }
    void swapKeys(AvlNode<K,D> *first, AvlNode<K,D> *second);
    void ClearTree(){
        postOrder(head->leftSon,removeT());
        head->leftSon=NULL;
        size=0;
    }
    void remove(K& key);
    void removeByPointer(AvlNode<K,D>* Node);

    AvlNode<K,D>* gethead()
    {
        return this->head->leftSon;
    }
    void  sethead(AvlNode<K,D>* head1){
        *head->leftSon = *head1;
    }

    AvlNode<K,D>* InsertNode(K& k , D* data);
    AvlNode<K,D>* SearchForNode(AvlNode<K,D>* Node,K& key);

    class toPrint{
    public:
        std::ostream& os;
        toPrint(std::ostream& os) : os(os){}
        void operator()(AvlNode<K,D>* toP){
            os<<toP;
        }
    };

    friend std::ostream& operator<<(std::ostream& os , AvlTree& tree ){
        toPrint pr(os);
        tree.InOrder(tree.head->leftSon,toPrint(os));
        return pr.os;
    }
    int getBF(AvlNode<K,D>* node);
    //AvlNode<K> search(K& key);
    template<class func>
    void preOrder(AvlNode<K,D> *node, func apply);

    template<class func>
    void InOrder(AvlNode<K,D> *node, func* apply);

    template<class func>
    void postOrder(AvlNode<K,D> *node, func apply);
    void UpdateAndBalance(AvlNode<K,D>* node);

};

template <class K , class D>
int AvlTree<K,D>::getBF(AvlNode<K,D> *node) {
    return node->leftSon->height - node->rightSon->height;
}


template<class K,class D>
D* AvlTree<K,D>::PointerToTheNode2(AvlNode<K,D> *Node, K& key) {
    if (Node->key < key) {
        if (Node->rightSon == NULL) {
            return NULL;
        } else {
            return PointerToTheNode2(Node->rightSon, key);
        }
    } else {
        if (Node->key > key) {
            if (Node->leftSon == NULL) {
                return NULL;
            } else {
                return PointerToTheNode2(Node->leftSon, key);
            }
        } else { // key == iterator->key;
            return Node->data;
        }
    }
    return Node->key;
}


template<class K , class D>
template <class func>
void AvlTree<K,D>::InOrder(AvlNode<K ,D> *node, func* apply) {
    if (!node)
    {
        return;
    }
    if(node->leftSon != NULL) {
        InOrder(node->getleft(), apply);
    }
    (*apply)(node);
    if(node->rightSon != NULL) {
        InOrder(node->getright(), apply);
    }
}
template<class K , class D>
template <class func>
void AvlTree<K,D>::CountedInOrder(AvlNode<K ,D> *node, func* apply,int* count,bool flagP) {
    if (!node || *count==0 || !flagP)
    {
        return;
    }
    if(node->leftSon != NULL) {
        CountedInOrder(node->getleft(), apply, count, flagP);
    }
    (*apply)(node);

    if(node->rightSon != NULL ) {
        CountedInOrder(node->getright(),apply,count,flagP);
    }
}

template<class K , class D>
template <class func>

void AvlTree<K,D>::preOrder(AvlNode<K,D> *node, func apply) {
    if (!node) {
        return;
    }
    apply(node);
    if(node->getleft()!=NULL) {
        preOrder(node->getleft(), apply);
    }
    if(node->getleft()!=NULL) {
        preOrder(node->getright(), apply);
    }
}
template <class K , class D>
template < class func>
void AvlTree<K,D>::postOrder(AvlNode<K,D> *node, func apply) {
    if (node==NULL) {
        return;
    }
    if(node->leftSon!=NULL) {
        postOrder(node->getleft(), apply);
    }
    if(node->rightSon!=NULL) {
        postOrder(node->getright(), apply);
    }
    apply(node);
}

template <class K , class D>
void AvlTree<K,D>::remove(K& key){
    AvlNode<K,D>* node =PointerToTheNode(head->leftSon,key);
    if(node == NULL || node->key != key)  // *(node->key)!=key)
        throw DOESNT_EXISTS();
    removeByPointer(node);
    size--;
}
template<class K , class D>
void AvlTree<K,D>::removeByPointer(AvlNode<K,D>* node){
    if(node->leftSon == NULL && node->rightSon == NULL){/// the current node has no sons;
        AvlNode<K,D>* curFather=node->father;
        if(curFather->rightSon==node){
            curFather->rightSon = NULL;
        } else{
            curFather->leftSon=NULL;
        }
        delete node;
        node = NULL;
        UpdateAndBalance(curFather);
        // Size--;
    }else if(node->rightSon && !node->leftSon){ // the current node has one son (right), connect between the sons and the father;
        AvlNode<K,D>* curFather=node->father;
        if(curFather->rightSon==node){
            curFather->rightSon = node->rightSon;
        } else{
            curFather->leftSon=node->rightSon;
        }
        node->rightSon->father=curFather;
        delete node;
        node = NULL;
        UpdateAndBalance(curFather);
        //   Size--;
    } else if(node->leftSon && !node->rightSon){// the current node has one son (left), connect between the son and the father;
        AvlNode<K,D>* curFather=node->father;
        if(curFather->rightSon==node){
            curFather->rightSon = node->leftSon;
        } else{
            curFather->leftSon=node->leftSon;
        }
        node->leftSon->father=curFather;
        delete node;
        node = NULL;
        UpdateAndBalance(curFather);
        //   Size--;
    }else{                                  // the current node has two sons;
        AvlNode<K,D>* next = node->rightSon;
        while(next->leftSon){
            next = next->leftSon;
        }
        K next_key = next->key;
        D* next_data = next->data;
        next->key = node->key;
        next->data = node->data;
        node->key=next_key;
        node->data=next_data;
        removeByPointer(next);
    }

}

template<class K , class D>
void AvlTree<K,D>::UpdateAndBalance(AvlNode<K,D> *node) {

    AvlNode<K,D> *iterator = node;
    while (iterator != head) {
        iterator->updateHeight();
        iterator->updateBalanceFactor();
        iterator->BalanceRoll();
        iterator = iterator->father;
    }

}

template<class K ,class D>
AvlNode<K,D> *AvlTree<K,D>::SearchForNode(AvlNode<K,D> *Node, K& key) {

    if (Node->key < key) {
        if (Node->rightSon == NULL) {
            return Node;
        }
        else {
            return SearchForNode(Node->rightSon, key);
        }
    }
    if (Node->key > key) {
        if (Node->leftSon == NULL) {
            return Node;
        }
        else {
            return SearchForNode(Node->leftSon, key);
        }
    }

    else { // key == iterator->key;
        throw ALREADY_EXISTS();
    }
    return NULL;
}

template<class K ,class D>
AvlNode<K,D>* AvlTree<K,D>::InsertNode(K& k , D* data) {
    if(this->head->leftSon == NULL && this->head->rightSon == NULL){
        auto toAdd = new AvlNode<K,D>(k,this->head);
        toAdd->data = data;
        this->head->leftSon = toAdd;
        toAdd->father = head;
        UpdateAndBalance(toAdd);
        size++;
        return toAdd;
    }
    AvlNode<K,D> *father = SearchForNode(this->head->leftSon,k);
    AvlNode<K,D> *toAdd;
    toAdd = new AvlNode<K,D>(k, father);
    toAdd->data = data;
    if (father->key < toAdd->key) {
        father->rightSon = toAdd;
        toAdd->father=father;
    }
    if (father->key > toAdd->key) {
        father->leftSon = toAdd;
        toAdd->father=father;
    }
    if (father->key == toAdd->key) {
        delete toAdd;
        throw ALREADY_EXISTS();
    }
    UpdateAndBalance(toAdd);
    size++;
    return toAdd;
}

template<class K , class D>
AvlNode<K,D> *AvlTree<K,D>::PointerToTheNode(AvlNode<K, D> *Node, K& key) {
    if(Node == NULL)
    {
        return  NULL;
    }
    if(Node->key == key) { // key == iterator->key;
        return Node;
    }
    if (Node->key < key) {
        if (Node->rightSon == NULL) {
            return NULL;
        } else {
            return PointerToTheNode(Node->rightSon, key);
        }
    }
    if (Node->key > key) {
        if (Node->leftSon == NULL) {
            return NULL;
        } else {
            if (Node->key != key) {
                return PointerToTheNode(Node->leftSon, key);
            }
        }
    }
    return NULL;
}

template<class K , class D>
void AvlTree<K,D>::swapKeys(AvlNode<K ,D> *first, AvlNode<K,D> *second) {
    K temp=first->key;
    first->key = second->key;
    second->key = temp;
}



#endif //SHAJARA_AVLTREE_H
