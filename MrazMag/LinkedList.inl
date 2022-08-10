#include"LinkedList.hpp"
#include<exception>

template<typename Type>
LinkedList<Type>::LinkedList(const LinkedList<Type>& other)
{
    if(!other.empty())
    {
        node<Type>* currentOther = other.first;
        node<Type>* currentThis = nullptr;
        try
        {
            currentThis = new node<Type>(currentOther->data , nullptr);
        }
        catch(...)
        {
            throw std::invalid_argument("cant allocate memory");
        }
        this->first = currentThis;
        
        currentOther = currentOther->next;
        while(currentOther != nullptr)
        {
            try
            {
               currentThis->next = new node<Type>(currentOther->data , nullptr);
            }
            catch(...)
            {
                node<Type>* toBeDeleted = nullptr;
                while (currentThis != nullptr)
                {
                    toBeDeleted = currentThis;
                    currentThis = currentThis->next;
                    delete toBeDeleted;
                }
                
                this->first = nullptr;
                throw std::bad_alloc();
            }
            currentThis = currentThis->next;
            currentOther = currentOther->next;
            
        }
        this->last = currentThis;
        this->size = other.size;
    }

}

template<typename Type>
LinkedList<Type>& LinkedList<Type>::operator=(const LinkedList<Type>& other)
{
    if(this != &other)
    {
        LinkedList<Type> cpy(other);
        std::swap(this->first , cpy.first);
        std::swap(this->last , cpy.last);
        std::swap(this->size , cpy.size);

    }
    return *this;
}

template<typename Type>
LinkedList<Type>::~LinkedList()
{
    node<Type>* cpy = nullptr;
    while (this->first != nullptr)
    {
        
       cpy = this->first;
       this->first = first->next;
       delete cpy;
       
    }
    
}

template<typename Type>
const Type& LinkedList<Type>::front()const
{
    if(!this->empty())
    {
        return this->first->data;
    }
    throw std::out_of_range("empty list");

}

template<typename Type>
const Type& LinkedList<Type>::back()const
{
    if(!this->empty())
    {
        return this->last->data;
    }
    throw std::out_of_range("empty list");

}

template<typename Type>
void LinkedList<Type>::push_front(const Type& value)
{
    node<Type>* newElem = nullptr;
    try
    {
        newElem = new node<Type>(value , this->first);
    }
    catch(...)
    {
        throw std::invalid_argument("cant allocate memory");
    }
    if(!empty())
    {
        this->first = newElem;
        
    }
    else
    {
        this->first = newElem;
        this->last = this->first;
    }
    this->size++;
}

template<typename Type>
void LinkedList<Type>::push_back(const Type& value)
{
    node<Type>* newElem = nullptr;
    try
    {
        newElem = new node<Type>(value , nullptr);
    }
    catch(...)
    {
        throw std::invalid_argument("cant allocate memory");
    }
    if(!empty())
    {
        this->last->next = newElem;
        this->last = newElem;
        if(this->size == 1) // if we have only one element
        {
            this->first->next = this->last;
        }
        
    }
    else // if list is empty
    {
        this->first = newElem;
        this->last = this->first;
    }
    this->size++;
}

template<typename Type>
void LinkedList<Type>::pop_front()
{
    if(!empty())
    {
        if( first != last)
        {
            node<Type> *toBeDeleted = this->first;
            this->first = this->first->next;
            delete toBeDeleted;
        }
        else
        {
            delete this->first;
            this->first = nullptr;
            this->last = nullptr;
        }
       this->size --;
    }
}

template<typename Type>
void LinkedList<Type>::pop_back()
{
    if(!empty())
    {
        if( first != last)
        {
            node<Type> *current = this->first;
            while(current->next->next != nullptr)
            {
                current = current->next;
            }
            this->last = current;
            this->last->next = nullptr;
            current = current->next;
            delete current;
        }
        else
        {
            delete this->last;
            this->first = nullptr;
            this->last = nullptr;
        }
       this->size --;
    }
}

template<typename Type>
void LinkedList<Type>::insertElemOnPosition(const Type &value,  size_t position)
{
    if(position > this->size )
    {
        throw std::invalid_argument("too big index");
    }
    if(position == this->size)
    {
        this->push_back(value);
    }
    else if(position == 0)
    {
        this->push_front(value);
    }
    else
    {
        node<Type>* current = this->first;
        position--; // we wanna reаch the previous elem
        while(position)
        {
            current = current->next;
            position--;
        }
        node<Type>* toBeAdded = nullptr;
        try
        {
           toBeAdded = new node<Type>(value , current->next);
        }
        catch(...)
        {
            throw std::bad_alloc();
        }
        std::cout<<toBeAdded->data<<std::endl;
        current->next = toBeAdded;
        this->size++;

    }
}

template<typename Type>
void LinkedList<Type>::deleteElemOnPosition( size_t position)
{
    if(position > this->size )
    {
        throw std::invalid_argument("too big index");
    }
    else if(position == 0)
    {
        this->pop_front();
    }
    else if(position == this->size - 1)
    {
        this->pop_back();
    }
    else
    {
        node<Type>* current = this->first;
        position--; //we wanna reаch the previous elem
        while(position)
        {
            current = current->next;
            position--;
        }
        
        node<Type>* toBeDeleted = current->next;
        current->next = current->next->next;
        delete toBeDeleted;
        this->size--;

    }

}

template<typename Type>
void LinkedList<Type>::deleteElemByPointer(LinkedList<Type>::Iterator& it)
{
    if(this->empty())
    {
        throw std::out_of_range("empty list");
    }
    node<Type>* toBeDeleted = it.ptr;
    if(toBeDeleted == this->first)
    {
        this->pop_front();
        it.ptr = this->first;
    }
    else if(toBeDeleted == this->last)
    {
        this->pop_back();
        it.ptr = this->last;
    }
    else
    {
        node<Type>* current = this->first;
        while(current->next != nullptr &&  current->next != toBeDeleted)
        {
            current = current->next;
        }
        if(current->next == toBeDeleted)
        {
            current->next = current->next->next;
            delete toBeDeleted;
            it.ptr = current->next;
            this->size--;
        }
    }


}

template<typename Type>
size_t LinkedList<Type>::getElemOnPosition( size_t position)
{
    if(position > this->size || empty())
    {
        throw std::invalid_argument("too big index");
    }
    else if( position == 1)
    {
        return this->first->data;
    }
    else if(position == this->size)
    {
        return this->last->data;
    }
    else
    {
        node<Type>* current = this->first;
        while(position)
        {
            current = current->next;
            position--;
        }
       return current->data;
    }

}

template<typename Type>
bool LinkedList<Type>::empty()const
{
    return (this->first == nullptr);
}

template<typename Type>
size_t LinkedList<Type>::getSize()
{
    return this->size;
}

template<typename Type>
void LinkedList<Type>::print()const
{
    if(!empty())
    {
        node<Type> *current = this->first;
        while (current != nullptr)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
   
}