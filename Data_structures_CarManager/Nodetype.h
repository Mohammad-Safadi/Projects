//
// Created by tamer on 5/15/2021.
//



#ifndef UNTITLED_CARSTYPE_H
#include <iostream>
#include <cstdlib>
#include "AvlNode.h"
#include "AvlTree.h"
class sellKey{
public:
    int typeId;
    int ModuleId;
    int sellCounter;
    ~sellKey() = default;
    sellKey() = default;
    sellKey(int typeId , int ModuleId , int sellCounter):typeId(typeId),ModuleId(ModuleId),sellCounter(sellCounter){}
    sellKey& operator=(const sellKey& other) {
        typeId = other.typeId;
        ModuleId = other.ModuleId;
        sellCounter = other.sellCounter;
        return *this;
    }
    bool operator==(sellKey& car) const
    {
        if( this->sellCounter == car.sellCounter)
        {
            if( this->ModuleId == car.ModuleId)
            {
                return this->typeId == car.typeId;
            }
            return false;
        }
        return false;
    }
    bool operator!=(sellKey& car) const {
        if (this->sellCounter != car.sellCounter || this->ModuleId != car.ModuleId || this->typeId != car.typeId) {
            return true;
        }
        return false;
    }

    bool operator<(sellKey& car) const
    {
        if(this->sellCounter == car.sellCounter){
            if(this->typeId == car.typeId){
                return this->ModuleId > car.ModuleId;
            }
            return this->typeId > car.typeId;
        }
        return ( this->sellCounter < car.sellCounter );
    }
    bool operator>(sellKey& car) const
    {
        if(this->sellCounter == car.sellCounter) {
            if(this->typeId == car.typeId){
                return this->ModuleId < car.ModuleId;
            }
            return this->typeId < car.typeId;
        }
        return ( this->sellCounter > car.sellCounter );
    }

};
class secondKey{
public:
    int typeId;
    int ModuleId;
    int rating;
    ~secondKey() = default;
    secondKey()= default;
    secondKey(int typeId , int ModuleId , int rating):typeId(typeId),ModuleId(ModuleId),rating(rating){}

    secondKey& operator=(const secondKey& other){
        typeId = other.typeId;
        ModuleId=other.ModuleId;
        rating = other.rating;
        return *this;
    }

    bool operator==(secondKey& car) const
    {
        if( this->rating == car.rating)
        {
            if( this->ModuleId == car.ModuleId)
            {
                return this->typeId == car.typeId;
            }
            return false;
        }
        return false;
    }
    bool operator!=(secondKey& car) const {
        if (this->rating != car.rating || this->ModuleId != car.ModuleId || this->typeId != car.typeId) {
            return true;
        }
        return false;
    }

    bool operator>(secondKey& car) const
    {
        if(this->rating == car.rating) {
            if(this->typeId == car.typeId){
                return this->ModuleId > car.ModuleId;
            }
            return this->typeId > car.typeId;
        }
        return ( this->rating > car.rating );
    }
    bool operator<(secondKey& car) const
    {
        if(this->rating == car.rating) {
            if(this->typeId == car.typeId){
                return this->ModuleId < car.ModuleId;
            }
            return this->typeId < car.typeId;
        }
        return ( this->rating < car.rating );
    }

};
class firstNode{
public:
    int typeId;
    secondKey** arrayptr;
    int Maxmoduletype;
    int maxsales;
    int modulesNum;
    bool flag;

    firstNode() = default;
    explicit firstNode(int typeId, int num): typeId(typeId),Maxmoduletype(0), maxsales(0) , modulesNum(num) , flag(true){
        if(num > 0){
            arrayptr = new secondKey*[num];
            for(int i=0 ; i<num; i++)
            {
                arrayptr[i] = NULL;
            }
        }
    }
    ~firstNode(){
        for(int i=0;i<modulesNum;i++) {
          if(arrayptr[i]!=NULL) {
              delete arrayptr[i] ;
          }
        }
        delete[] arrayptr;
        arrayptr=NULL;
    }
    firstNode(firstNode& node):  typeId(node.typeId),Maxmoduletype(node.Maxmoduletype), maxsales(node.maxsales) , modulesNum(node.modulesNum) , flag(node.flag)
    {
        if(modulesNum > 0) {
            if (node.arrayptr != NULL) {
                arrayptr = new secondKey*[modulesNum];
                for (int i = 0; i < modulesNum; i++) {
                    if (node.arrayptr[i] != NULL) {
                        arrayptr[i] = new secondKey(*node.arrayptr[i]);
                    } else {
                        arrayptr[i] = NULL;
                    }

                }
            } else {
                arrayptr = NULL;
            }
        }
    }
    void setMax(int moduleId , int sales){

        Maxmoduletype =moduleId;
        maxsales = sales;
    }
    bool checkmaxid(int moduleid, int sales){
        if(moduleid == Maxmoduletype){
            return true;
        }
        if(sales == maxsales && moduleid < Maxmoduletype)
        {
            return  true;
        }
        if(sales > maxsales)
        {
            return true;
        }
        return false;
    }
    int getSale()
    {
        return maxsales;
    }
    int getId()
    {
        return Maxmoduletype;
    }
    firstNode& operator=( firstNode& node)
    {
        if(this == &node)
        {
            return *this;
        }
        this->typeId = node.typeId;
        this->Maxmoduletype = node.Maxmoduletype;
        if(node.arrayptr != nullptr)
        {
            auto array = new secondKey*[node.modulesNum];
            for(int i = 0; i< node.modulesNum; i++)
            {
                array[i] = node.arrayptr[i];
            }
            this->arrayptr = array;
            modulesNum = node.modulesNum;
            maxsales = node.maxsales;
        }

        return *this;
    }

    secondKey* getFromArray(int i)
    {
        return arrayptr[i];
    }
    int getModelNum() const
    {
        return modulesNum;
    }

    void setInArray(int i , secondKey* node){
        arrayptr[i] = node;
    }


    friend std::ostream& operator<<(std::ostream& os , firstNode& node){
        os << node.typeId;
        return os;
    }
    bool getflag() const{
        return flag;
    }
    void changeflag(){
        flag = false;
    }
};
class secondNode{
public:
    int typeId;
    int ModuleId;
    int rating;
    int sellCounter;
    bool passed;
    secondNode() = default;
    secondNode( int typeId,int ModuleId , int rating , int sellCounter ) :typeId(typeId) ,ModuleId(ModuleId),rating(rating),sellCounter(sellCounter), passed(false){}
    secondNode( const secondNode& node) = default;


    int getId(){
        return typeId;
    }
    int getmodel(){
        return ModuleId;
    }
    int getrating(){
        return rating;
    }
    int getSales(){
        return sellCounter;
    }
    void setflag(){
        passed = true;
    }

    secondNode& operator=( secondNode& node) {
        if (this != &node){
            typeId = node.typeId;
            ModuleId = node.ModuleId;
            rating = node.rating;
            sellCounter = node.sellCounter;
            passed = node.passed;
        }
        return *this;
    }

    ~secondNode() = default ;
    friend std::ostream& operator<<(std::ostream& os , secondNode& node){
        os << node.typeId + node.ModuleId;
        return os;
    }
    void increas(){
        sellCounter++;
        rating += 10;
    }
    void operator++(){
        sellCounter++;
        rating += 10;
    }

    void decressRating(int t){
        rating-= 100/t;
    }
};

class ZeroNode;

class thirdNode{
private:
    int typeId;
    AvlTree<int,ZeroNode>* graph;
public:
    explicit thirdNode(int typeId , AvlTree<int,ZeroNode>* graph1 = NULL):typeId(typeId){
        graph=graph1;
    }

    ~thirdNode(){
        delete graph;
        graph=NULL;
    }
    friend std::ostream& operator<<(std::ostream& os , thirdNode& node){
        os << node.typeId ;
        return os;
    }

    AvlTree<int,ZeroNode>* getTree(){
        return graph;
    }

};
class ZeroNode{
private:
    int typeId;
    int moduleId;
    int sellCounter;
public:
    ZeroNode( int typeId,int moduleId , int sellCounter): typeId(typeId), moduleId(moduleId),sellCounter(sellCounter){}
    ~ZeroNode() = default;
    friend std::ostream& operator<<(std::ostream& os , ZeroNode& node){
        os <<node.moduleId;
        return os;
    }
    int getId() const{
        return typeId;
    }
    int getcount() const{
        return sellCounter;
    }
    int getmodel() const{
        return moduleId;
    }

};
class sellnode{
public:
    int sellcounter;
    int typeId;
    int modelId;
    bool flag;
    sellnode(int sellcounter , int typeId , int modelId):sellcounter(sellcounter),typeId(typeId),modelId(modelId) , flag(false){}
    ~sellnode()= default;

    int getId() const{
        return modelId;
    }
};
#define UNTITLED_CARSTYPE_H

#endif //UNTITLED_CARSTYPE_H
