#ifndef QUEUE_INL
#define QUEUE_INL

#include"Queue.h"
#include<cassert>

template <typename Type>
void Queue<Type>::clear()
{
    delete[] this->first;
    this->first = this->last = nullptr;
    this->mSize = 0;
}

template <typename Type>
Queue<Type>::Queue(const Queue<Type>& other)
{
    Node<Type>* current = new Node<Type>(other.first->value , other.first);
    while (current != nullptr)
    {
        this->push(current->value);
        current = current->next;
    }
    delete current;
}

template <typename Type>
Queue<Type> &Queue<Type>::operator=(const Queue<Type> &other)
{
    if(this != &other)
    {
        Queue<Type> cpy(other);
        std::swap(this->first , cpy.first);
        std::swap(this->last , cpy.last);
        std::swap(this->mSize , cpy.mSize);
    }
    return *this;
}

template <typename Type>
Queue<Type>::~Queue()
{
    this->clear();
}

template <typename Type>
void Queue<Type>::push(const Type &value)
{
    if(this->isEmpty())
    {
        this->first = new Node<Type>(value, nullptr);
        this->last = this->first;
    }
    else
    {
        this->last->next = new Node<Type>(value, nullptr);
        this->last = this->last->next;
    }

    this->mSize++;

}

template <typename Type>
void Queue<Type>::pop()
{
    assert(!isEmpty());
    Node<Type>* toBeDeleted = this->first;
    this->first = this->first->next;
    this->mSize--;
    delete toBeDeleted;
}

template <typename Type>
inline const size_t &Queue<Type>::size() const
{
    return this->mSize;
}

template <typename Type>
const Type &Queue<Type>::top() const
{
    assert(!isEmpty());
    return this->first->value;
}

template <typename Type>
inline bool Queue<Type>::isEmpty() const
{
    return this->mSize == 0;
}

#endif