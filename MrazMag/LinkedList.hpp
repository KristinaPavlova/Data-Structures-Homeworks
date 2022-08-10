
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<iostream>

#include"node.inl"

template<typename Type>
class LinkedList
{
    private:
    node<Type>* first{nullptr};
    node<Type>* last{nullptr};
    size_t size{0};


    public:
    LinkedList() = default;
    LinkedList(const LinkedList<Type>& );
    LinkedList<Type>& operator=(const LinkedList<Type>& );
    ~LinkedList();

    const Type& front()const;
    const Type& back()const;
    void push_front(const Type&);
    void push_back(const Type&);
    void pop_front();
    void pop_back();
    void insertElemOnPosition(const Type& ,  size_t);
    void deleteElemOnPosition(  size_t);

    
    size_t getElemOnPosition(  size_t);

    bool empty()const;
    size_t getSize();

    void print()const;

    class Iterator
    {
        

        private:
        node<Type>* ptr;
        Iterator(node<Type>* ptr_):ptr(ptr_){}
        
        friend LinkedList<Type>;
        public:

        Iterator():ptr(nullptr){}

        node<Type>* operator->()
        {
            return this->ptr;
        }
        Type& operator*() const
        {
            return ptr->data;
        }
        Iterator& operator++()
        {
            this->ptr= ptr->next;
            return *this;
        }
        bool operator!=(const Iterator& other)
        {
            return (this->ptr != other.ptr);
        }

    };

    Iterator begin()
    {
        return  Iterator(this->first);
    }
    Iterator end()
    {
        return  Iterator(nullptr);
    }

    void deleteElemByPointer(LinkedList<Type>::Iterator&);


};

#include"LinkedList.inl"
#endif