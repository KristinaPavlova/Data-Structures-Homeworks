#ifndef QUEUE_H
#define QUEUE_H

#include<cstddef> // size_t
#include"Node.inl"
template<typename Type>
class Queue
{
    private:

    size_t mSize{};
    Node<Type>* first{};
    Node<Type>* last{};
    void clear();

    public:
    Queue():mSize(0) , first(nullptr) , last(nullptr){};
    Queue(const Queue<Type>& );
    Queue<Type>& operator=(const Queue<Type>&);
    ~Queue();

    void push(const Type&);
    void pop();
    const size_t& size()const;
    const Type& top()const;
    bool isEmpty()const;
};

#include"Queue.inl"

#endif