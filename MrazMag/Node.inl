#ifndef NODE_H
#define NODE_H

template<typename Type>
struct Node
{
    Type value;
    Node* next;

    Node(const Type& value_ , Node<Type>* ptr):value(value_) , next(ptr)
    {}
    Node(Node<Type>& other)
    {
        this->value = other.value;
        this->next = other.next;
    }
    Node<Type>& operator=(Node<Type>& other)
    {
        this->value = other.value;
        this->next = other.next;
        return *this;
    }
    
};


#endif