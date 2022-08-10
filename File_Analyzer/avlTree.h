#ifndef AVLTREE_H
#define AVLTREE_H
#include<vector>

template<typename Type>
struct node
{
    Type value{};
    node<Type>* left{};
    node<Type>* right{};
    int heigh{};

    node(const Type& value_, node<Type>* left_, node<Type>* right_, int heigh_):value(value_) ,left(left_) , right(right_) , heigh(heigh_){}

};

template<typename Type>
class avlTree
{
    private:

    node<Type>* root{};
    int size{}; // keeps number of elements in order to take them for O(1)

    int getHeigh(node<Type>*)const;
    
    void clear(node<Type>*);

    node<Type>* rightRotation(node<Type>*);
    node<Type>* leftRotation(node<Type>*);

    int balanceFactor(node<Type>*);

    node<Type>* minValue(node<Type>*);

    node<Type>* recursiveFind(node<Type>* , const Type&)const;
    node<Type>* recursiveInsert(node<Type>* , const Type&);
    node<Type>* recursiveRemove(node<Type>* , const Type&);

    std::vector<Type> recursiveInorder(node<Type>*)const;

    public:

    avlTree() = default;
    avlTree(avlTree<Type>& other); 
    avlTree<Type>& operator=(avlTree<Type>& other); 
    ~avlTree();

    bool empty()const;

    int getSize()const;

    node<Type>* find(const Type&)const;

    void insert(const Type&);
    void remove(const Type&);

    std::vector<Type> inorder()const; // return vector of elements inorder

};
#include"avlTree.inl"

#endif