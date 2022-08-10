

template<typename Type>
struct node
{
    Type data;
    node* next{nullptr};
    node(Type data_ , node* next_):data(data_) , next(next_){}
};