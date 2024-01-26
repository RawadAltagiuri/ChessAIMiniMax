#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "ChessBoard.h"
#include <cmath>
#include <ctime>

template <class T>
class priority_deque{
    public:
    std::vector<std::pair<double, ChessBoard>>vec;
    
    int maxSize;
    int currLeaf;
    bool check; 
    bool isMaxHeap = false; 
    int firstLeafIndex; 
   
    public:
        priority_deque();
        priority_deque(int);
        priority_deque(bool);
        ~priority_deque();

        
        std::pair<double, ChessBoard> poll(); 
        void add(std::pair<double, ChessBoard>); 
        void clear();
        void setMaxSize(int); 


        int size();
        bool empty(); 
        T peek();


    private:
        void bubbleUp(int);
        void bubbleDown(int);
        void bubbleUpMaxHeap(int);
        void bubbleDownMaxHeap(int);
        int findLeavesIndexes();
        void addingAtMaxSize(std::pair<double, ChessBoard>);
        
};


template <class T>
priority_deque<T>::priority_deque(){
    this->maxSize = 7;
    this->check = false;
    this->isMaxHeap = false;
    this->currLeaf = 0;
    this->firstLeafIndex = 0;
}


template <class T>
priority_deque<T>::priority_deque(int maxSize){
    this->maxSize = maxSize;
    this->check = false;
    this->currLeaf = 0;
    this->firstLeafIndex = 0;
}


template <class T>
priority_deque<T>::priority_deque(bool isMaxHeap){
    this->maxSize = 7;
    this->check = false;
    this->isMaxHeap = isMaxHeap;
    this->currLeaf = 0;
    this->firstLeafIndex = 0;
}



template <class T>
priority_deque<T>::~priority_deque(){
    this->vec.clear();
}

template <class T>
T priority_deque<T>::peek(){
    if(this->vec.size()==0){
        throw std::runtime_error("Priority Queue is empty");
    }
    return this->vec[0];
}




template <class T>
void priority_deque<T>::setMaxSize(int maxSize){
    this->check = false; 
    this->maxSize = maxSize;
}


template <class T> //O(1)
bool priority_deque<T>::empty(){
    return this->vec.size() == 0;
}


template <class T> //O(1)
void priority_deque<T>::clear(){
    this->vec.clear();
}


template <class T>
int priority_deque<T>::size(){
    return this->vec.size();
}


template <class T>
std::pair<double, ChessBoard> priority_deque<T>::poll(){
    if(this->vec.size() == 0){
        throw std::runtime_error("Priority Queue is empty");
    }
    T temp = this->vec[0];
    this->vec[0] = this->vec[this->vec.size() - 1];
    this->vec.pop_back();
    if(this->isMaxHeap){
        this->bubbleDownMaxHeap(0);
    }
    else{
        this->bubbleDown(0);
    }
    return temp;
}



template <class T>
void priority_deque<T>::add(std::pair<double, ChessBoard> data){
     if(this->maxSize == 0) return;
     if(this->vec.size() < this->maxSize){ 
        this->vec.push_back(data);
        if(this->isMaxHeap){
            this->bubbleUpMaxHeap(this->vec.size()-1);
        }
        else{
            this->bubbleUp(this->vec.size()-1);
        }
        return;
     }

     if(!this->check){ 
        this->firstLeafIndex = findLeavesIndexes();
        this->check=true;
     }

    this->addingAtMaxSize(data); 
}



template <class T>
void priority_deque<T>::addingAtMaxSize(std::pair<double, ChessBoard> data){
    this->currLeaf = this->firstLeafIndex + (rand() % (this->vec.size() - this->firstLeafIndex));
    if(this->isMaxHeap){
        if(data.first < this->vec[currLeaf].first){
            return;
        }
    }else{
        if(data.first > this->vec[currLeaf].first){
            return;
        }
    }
    this->vec[currLeaf] = data;
    if(this->isMaxHeap){
        this->bubbleUpMaxHeap(this->currLeaf);
    }else{
        this->bubbleUp(this->currLeaf);
    }
}

template <class T>
void priority_deque<T>::bubbleUp(int index){
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (this->vec[index].first < this->vec[parent].first) {
            std::swap(this->vec[index], this->vec[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

template <class T>
void priority_deque<T>::bubbleDown(int index) {
    int size = this->vec.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && this->vec[left].first < this->vec[smallest].first) {
            smallest = left;
        }

        if (right < size && this->vec[right].first < this->vec[smallest].first) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(this->vec[index], this->vec[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}


template <class T>
void priority_deque<T>::bubbleUpMaxHeap(int index){
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (this->vec[index].first > this->vec[parent].first) {
            std::swap(this->vec[index], this->vec[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

template <class T>
void priority_deque<T>::bubbleDownMaxHeap(int index) {
    int size = this->vec.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && this->vec[left].first > this->vec[largest].first) {
            largest = left;
        }

        if (right < size && this->vec[right].first > this->vec[largest].first) {
            largest = right;
        }

        if (largest != index) {
            std::swap(this->vec[index], this->vec[largest]);
            index = largest;
        } else {
            break;
        }
    }
}


template<class T>
int priority_deque<T>::findLeavesIndexes(){
    int minLeave = 0;
    for(int i = this->vec.size()-1; i >= 0; i--){
        int left = (2*i)+1;
        int right = (2*i)+2;
        if(left >= this->vec.size() && right >=this->vec.size()){
            minLeave = i;
        }
    }
    return minLeave;
}
