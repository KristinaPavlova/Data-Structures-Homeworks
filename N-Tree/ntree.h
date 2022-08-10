#ifndef NTREE_H
#define NTREE_H
#include"node.inl"
#include<exception>
#include<queue>
#include<stack>
#include<list>
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>
#include <algorithm>
class NTree
{
    public:
    NTree();
    NTree(const int&);
    NTree(const NTree&);
    //load from file
    NTree(const char*);
    NTree& operator=(const NTree&);
    ~NTree();

    //add new node by given - parent , child
    void add(node*&  , node*&);

    int heigh();

    //for testing
    void printByLevels()const;

    void writeInFile(const char*);

    //for testing
    void loadFromFile(const char*);

    bool contains(NTree&);

    void remove(NTree&);

    
    private:
    node* root{};

    void copy(node*& , const node*);

    //delete tree recursive
    void destroy(node*&);

    //returns pointer if searched value is found else->nullptr
    node* find(std::vector<node*>& , node*&);

    //check if current tree is equal or contains subtree
    bool containsHelper(node*& , node*&);

    //go through all notes recursivaly and check for match with subtree
    bool containsReursive(node*& , node*&);

    //returns sum of all notes in a tree
    int sumOfNodes(node*& );

    //returns pointer to father of given node
    node* getFather(node*& , node*&);

    //returns heigh of a tree (recursive)
    int heighRecursive(node*&);

    //checks if subtrees remain after deletion 
    bool leftSubtreesToSum(node*& , node*&);

    //returns the sum of the remaining subtrees after deletion
    int getSumOfSubtrees(node*& , node*&);



};

#endif