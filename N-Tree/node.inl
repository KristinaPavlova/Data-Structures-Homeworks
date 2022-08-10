#ifndef NODE_INL
#define NODE_INL
#include<vector>
struct node
{
    int data{};
    std::vector<node*> children;

    node(const int& data_ ):data(data_){};

    //used in std::find for checking if child already exists
    bool operator==(const node& other)
    {
        return this->data == other.data;
    }
};


#endif