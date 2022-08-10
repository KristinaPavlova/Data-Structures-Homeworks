#ifndef HEAP_HPP
#define HEAP_HPP
#include<vector>


template<typename Type>
class MinHeap
{
    private:
    std::vector<Type> heap{};

    int parentIndex(int&);
    int leftChildIndex(int&);
    int rightChildIndex(int&);

    void heapify(int&);

    public:
    MinHeap() = default;

    const Type& min()const; // return the min elem of the heap
    void  push_heap(const Type&);
    void pop_heap();
    bool empty()const;
    size_t size()const;
    void print()const;


};

#include"heap.inl"

#endif