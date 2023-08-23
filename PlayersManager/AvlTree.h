//
// Created by m7mds on 26/12/2021.
//

#ifndef DATASTRUCTURES_HW2_AVLTREE_H
#define DATASTRUCTURES_HW2_AVLTREE_H
//
// Created by m7mds on 16/05/2021.
//

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

    template<class func>
    void InOrderFuncByReference( func& func1) {
        if (size!=0) {
            head->leftSon->InOrderFuncByReference(func1);
        }
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

    void InOrderToTransferPlayers(AvlNode<K ,D> *node,AvlNode<K ,D>* FillA[],int ArrayIndex);// its actually an inorder to put tree's nodes in an array


    template<class func>
    void CountedInOrder(AvlNode<K ,D> *node, func* apply,int* count,bool flag);
    template<class func>
    void InOrderToFindPlayersByHisId(AvlNode<K ,D> *node, func* apply) ;



    AvlNode<K,D>** TreeToArray(AvlTree<K,D>* t1);

    AvlTree<K,D>* AvlMerge(AvlTree<K,D>* t1,AvlTree<K,D>*t2) ;


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


    void BuildAvl( AvlNode<K,D>* node , int m );

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

    AvlNode<K,D>* sortedArrayToBST(AvlNode<K,D>** arr, int start, int end)
    {
        if (start > end)
            return NULL;

        int mid = (start + end)/2;
        AvlNode<K,D>*root = new AvlNode<K,D>(*arr[mid]);
        root->leftSon =  sortedArrayToBST(arr, start, mid-1);
        if(root->leftSon)  root->leftSon->father=root;
        root->rightSon = sortedArrayToBST(arr, mid+1, end);
        if(root->rightSon)  root->rightSon->father=root;
        return root;
    }

    /* int getTheIndexOfTheElement(K& key , int histo_index , int* num_in_histo_index){
         int num_of_elements=0;
         *num_in_histo_index = 0;
         auto* ptr = this->head;
         while (ptr != NULL){
             if(ptr->key <= key){
                 if(ptr->leftSon){
                     num_of_elements+=ptr->leftSon->rank;
                     num_in_histo_index+=ptr->leftSon->score_histo[histo_index];
                 }
                 num_of_elements++;
                 //////////////////////////should check the if the scor of the current node == histo inedx => add 1
                 ptr=ptr->rightSon;
             }else{
                 ptr=ptr->leftSon;
             }
         }
         return num_of_elements;
     }*/
    AvlNode<K,D>* Select( int rank) {
        if (rank > size || rank <=0){
            return nullptr;
        }
        return SelectAux(head->leftSon,rank);
    }

    AvlNode<K,D>* SelectAux(AvlNode<K,D>* node , int rank){
        int w=0;
        if(node->leftSon != nullptr) {
            w = node->leftSon->rank;
        }
        if (w == rank-1){
            return node;
        }
        if(w > rank-1){
            return SelectAux(node->leftSon,rank);
        }
        if (w <rank-1){
            return SelectAux(node->rightSon,rank-w-1);
        }
        return nullptr;
    }



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
void AvlTree<K,D>::InOrderToTransferPlayers(AvlNode<K ,D> *node,AvlNode<K,D>* FillA[], int ArrayIndex) {
    if (!node )
    {
        return;
    }
    if(node->leftSon != NULL) {
        InOrderToTransferPlayers(node->getleft(),FillA,ArrayIndex+1);
    }
    FillA[ArrayIndex]=node;

    if(node->rightSon != NULL ) {
        InOrderToTransferPlayers(node->getright(),FillA,ArrayIndex+1);
    }

}
template<class K , class D>
template <class func>
void AvlTree<K,D>::InOrderToFindPlayersByHisId(AvlNode<K ,D> *node, func* apply) {
    if (!node )
    {
        return;
    }
    if(node->leftSon != NULL) {
        InOrderToFindPlayersByHisId(node->getleft(), apply);
    }
    (*apply)(node);
    if(node->rightSon != NULL ) {
        InOrderToFindPlayersByHisId(node->getright(),apply);
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

        int* tmp=next->score_histo;
        next->score_histo=node->score_histo;
        node->score_histo=tmp;

        UpdateAndBalance(next->father);

        removeByPointer(next);
    }

}

template<class K , class D>
void AvlTree<K,D>::UpdateAndBalance(AvlNode<K,D> *node) {

    AvlNode<K,D> *iterator = node;
    while (iterator != head) {
        iterator->updateHeight();
        iterator->updateBalanceFactor();
        iterator->updateRanknode();
        iterator->BalanceRoll();
        iterator->updateRanknode();
        iterator = iterator->father;
    }

}

template<class K ,class D>
AvlNode<K,D> *AvlTree<K,D>::SearchForNode(AvlNode<K,D> *Node, K& key) {//this father return the father in case we need to add a node

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
AvlNode<K,D> *AvlTree<K,D>::PointerToTheNode(AvlNode<K, D> *Node, K& key) {//this function searched for the node which has the given key
    //return NULL if not found
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


template < class K,class D>
void AvlTree<K,D>::BuildAvl( AvlNode<K,D>* node , int m ) {// m= num of nodes that we want to allocate memory for
    if(m==0){
        return;
    }
    if(m == 1){
        node->leftSon= new AvlNode<K,D>(K(), node);
        node->leftSon->father=node;
        return;
    }
    if(m==2){
        node->leftSon= new AvlNode<K,D>(K(), node);
        node->leftSon->father=node;
        node->rightSon = new AvlNode<K,D>(K(), node);
        node->rightSon->father=node;
        return;
    }
    node->leftSon = new AvlNode<K,D>(K(), node);
    node->leftSon->father=node;
    if(m%2==1) {
        BuildAvl(node->leftSon, (m/2));
    }
    else{
        BuildAvl(node->leftSon, (m/2)-1);
    }
    node->rightSon = new AvlNode<K,D>(K(), node);
    node->rightSon->father=node;
    BuildAvl(node->rightSon,(m/2)-1);
}



template <class K,class D>
class UpdateAfterMergeBf{
    int size;
public:
    explicit UpdateAfterMergeBf(int a):size(a){
    }
    void operator ()(AvlNode<K,D>* ptr){
        ptr->updateHeight();
        ptr->updateRanknode();
        ptr->updateBalanceFactor();

    }
};

template <class K,class D>
AvlNode<K,D>** AvlTree<K,D>::TreeToArray(AvlTree<K,D>* t1){
    AvlNode<K, D> **NodesTree1 = new AvlNode<K, D> *[t1->size];
    GetNodes<K,D> func1(NodesTree1, 0);
    t1->InOrderFuncByReference(func1);
    return NodesTree1;
}

template <class K,class D>
AvlTree<K,D>* AvlTree<K,D>::AvlMerge(AvlTree<K,D>* t1,AvlTree<K,D>*t2) {
    int size1 = t1->size + t2->size;
    AvlNode<K, D> **NodesTree1 = new AvlNode<K, D> *[t1->size];
    AvlNode<K, D> **NodesTree2 = new AvlNode<K, D> *[t2->size];
    GetNodes<K,D> func1(NodesTree1, 0);
    GetNodes<K,D> func2(NodesTree2, 0);
    t1->InOrderFuncByReference(func1);
    t2->InOrderFuncByReference(func2);

    AvlNode<K, D> **NodesTree = new AvlNode<K, D> *[size1];
    int index = 0, i = 0, j = 0;// index for the new array, i for the nodesTree1 j for the nodesTree2
    while (i < t1->size && j < t2->size){
        if (NodesTree1[i]->key > NodesTree2[j]->key) {
            NodesTree[index] = NodesTree2[j];
            j++;
            index++;
        } else {
            NodesTree[index] = NodesTree1[i];
            i++;
            index++;
        }
    }
    while (i < t1->size){
        NodesTree[index] = NodesTree1[i];
        i++;
        index++;
    }
    while (j < t2->size){
        NodesTree[index] = NodesTree2[j];
        j++;
        index++;
    }
    delete [] NodesTree1;
    delete [] NodesTree2;

    AvlTree<K,D>* tree = new AvlTree<K,D>();
    if(size1==0){
        delete [] NodesTree;
        return tree;
    }
    tree->head->leftSon=sortedArrayToBST(NodesTree,0,size1-1);// BINARY SEARCH TREE
    tree->head->leftSon->father=tree->head;
    delete [] NodesTree;
    tree->size=size1;
    UpdateAfterMergeBf<K,D> fun1(tree->size);
    tree->postOrder(tree->head->leftSon,fun1);
    return tree;
}




#endif //DATASTRUCTURES_HW2_AVLTREE_H
