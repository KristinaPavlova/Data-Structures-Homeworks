#ifndef PAIR_H
#define PAIR_H
#include<iostream>
//want to compare pairs only by the first argument (string in this case)
template <typename T1 , typename T2 >
class pair
{
    private:

    T1 first{};
    T2 second{};

    public:

    pair(const T1& first_, const T2& second_):first(first_),second(second_){}
    
    bool operator==(const pair<T1 , T2>& other)const
    {
        return first == other.first ? true : false;
    }
    bool operator<(const pair<T1 , T2>& other)const
    {
        return first < other.first ? true : false;
    }
    bool operator>(const pair<T1 , T2>& other)const
    {
        return first > other.first ? true : false;
    }
    bool operator!=(const pair<T1 , T2>& other)const
    {
        return !(this == other);
    }

    T1& getFirst()
    {
        return first;
    }

    T2& getSecond()
    {
        return second;
    }


};

#endif