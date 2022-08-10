#ifndef HEAP_INL
#define HEAP_INL
#include<exception>
#include"heap.hpp"

template<typename Type>
int MinHeap<Type>::parentIndex(int& currentIndex)
{
    return (currentIndex-1)/2;
}

template<typename Type>
int MinHeap<Type>::leftChildIndex(int& currentIndex)
{
    return (currentIndex*2)+1;
}

template<typename Type>
int MinHeap<Type>::rightChildIndex(int& currentIndex)
{
    return (currentIndex*2)+2;
}

template<typename Type>
void MinHeap<Type>::heapify(int& currentIndex)
{
    int leftCh = leftChildIndex(currentIndex);
    int rightCh= rightChildIndex(currentIndex);
    int size = heap.size();
    int currentCpy = currentIndex;
    if(leftCh < size && heap[leftCh] < heap[currentCpy])
    {
        currentCpy  = leftCh;
    }
    if(rightCh < size && heap[rightCh] < heap[currentCpy])
    {
        currentCpy  = rightCh;
    }
    if(currentCpy == currentIndex)
    {
        return;
    }
    std::swap(heap[currentIndex] , heap[currentCpy]);

    heapify(currentCpy);

}

template<typename Type>
const Type& MinHeap<Type>::min()const
{
    if(heap.empty())
    {
        throw std::length_error("empty heap");
    }
    
    return heap[0];
}

template<typename Type>
void MinHeap<Type>::push_heap(const Type& value)
{
    this->heap.push_back(value);
    int currentIndex = heap.size()-1;
    int parentId = parentIndex(currentIndex);

    while(parentId >= 0 && heap[currentIndex] < heap[parentId])
    {
        std::swap(heap[currentIndex] , heap[parentId]);
        currentIndex = parentId;
        parentId = parentIndex(parentId);
    }

}

template<typename Type>
void MinHeap<Type>::pop_heap()
{
    if(heap.empty())
    {
        throw std::invalid_argument("empty heap");
    }
    std::swap(heap[0] , heap[heap.size()-1]);
    heap.pop_back();
    int currentIndex = 0;

    heapify(currentIndex);
     
}

template<typename Type>
bool MinHeap<Type>::empty() const
{
    return heap.empty();
}

template<typename Type>
size_t MinHeap<Type>::size() const
{
    return heap.size();
}

template<typename Type>
void MinHeap<Type>::print() const
{
    for(Type value : heap)
    {
        std::cout<<value<<" ";
    }
    std::cout<<std::endl;
}
#endif