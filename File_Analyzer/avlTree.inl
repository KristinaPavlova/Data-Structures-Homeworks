#ifndef AVLTREE_INL
#define AVLTREE_INL
#include"avlTree.h"
#include<exception>



template<typename Type>
avlTree<Type>::~avlTree()
{
    clear(this->root);
}

template<typename Type>
avlTree<Type>::avlTree(avlTree<Type>& other)
{
    if (!other.empty())
    {
        std::vector<Type> otherInorder = other.inorder();
        for (auto it = otherInorder.begin(); it != otherInorder.end(); ++it)
        {
            this->insert((*it));
        }
    }
}

template<typename Type>
avlTree<Type>& avlTree<Type>::operator=(avlTree<Type>& other)
{
    if(this != &other)
    {
        avlTree<Type> otherCpy(other);
        std::swap(this->root , otherCpy.root);
        this->size = otherCpy.size;
    }
    return this;
}

template<typename Type>
int avlTree<Type>::getHeigh(node<Type>* root)const
{
    if(root == nullptr)
    {
        return 0;
    }
    return root->heigh;
}

template<typename Type>
void avlTree<Type>::clear(node<Type>* root)
{
    if(root)
    {
        delete root->left;
        delete root->right;
    }
}

template<typename Type>
node<Type>* avlTree<Type>::rightRotation(node<Type>* root)
{
    if(root == nullptr)
    {
        throw std::invalid_argument(" root is nullptr");
    }

    node<Type> *leftChild = root->left;
    node<Type> *rightSubtr = leftChild->right;

    leftChild->right = root;
    root->left = rightSubtr;

    leftChild->heigh = std::max(getHeigh(leftChild->left), getHeigh(leftChild->right)) + 1;
    root->heigh = std::max(getHeigh(root->left) , getHeigh(root->right))+1;

    return leftChild;
}


template<typename Type>
node<Type>* avlTree<Type>::leftRotation(node<Type>* root)
{
    if(root == nullptr)
    {
        throw std::invalid_argument(" root is nullptr");
    }

    node<Type>* rightChild = root->right;

    node<Type>* leftSubtr = rightChild->left;

    rightChild->left = root;
    root->right = leftSubtr;

    rightChild->heigh = std::max(getHeigh(rightChild->left), getHeigh(rightChild->right)) + 1;
    root->heigh = std::max(getHeigh(root->left) , getHeigh(root->right))+1;

    return rightChild;
    
}

template<typename Type>
int avlTree<Type>::balanceFactor(node<Type>* root)
{
    if(root == nullptr)
    {
        return 0;
    }
    return ( getHeigh(root->left) - getHeigh(root->right) );
}

template<typename Type>
node<Type>* avlTree<Type>::minValue(node<Type>* root)
{
    node<Type>* current = root;
    while (current->left != nullptr)
    {
        current = current->left;
    }
    return current;
    
}

template<typename Type>
node<Type>* avlTree<Type>::recursiveFind(node<Type>* root , const Type& value)const
{
    if(root == nullptr)
    {
        return nullptr;
    }
    else if(root->value > value)
    {
        return recursiveFind(root->left , value);
    }
    else if(root->value < value)
    {
        return recursiveFind(root->right , value);
    }
    return root;
}

template<typename Type>
node<Type>* avlTree<Type>::recursiveInsert(node<Type>* root , const Type& value)
{
    if(root == nullptr)
    {
        this->size++;
        node<Type>* newNode = nullptr;
        try
        {
            newNode = new node<Type>(value , nullptr , nullptr , 1);
        }
        catch(...)
        {
            throw std::bad_alloc();
        }
        
        return newNode;
    }
    if(root->value > value)
    {
        root->left = recursiveInsert(root->left , value);
    }
    else if(root->value < value)
    {
        root->right = recursiveInsert(root->right , value);
    }
    else
    {
        return root;
    }

    root->heigh = std::max(getHeigh(root->left) , getHeigh(root->right))+1;

    int balance = balanceFactor(root);

    if(balance > 1)
    {
        int balanceLeft = balanceFactor(root->left);
        if(balanceLeft >= 0)
        {
            return rightRotation(root);
        }
        else
        {
            root->left = leftRotation(root->left);
            return rightRotation(root);
        }

    }
    else if(balance < -1)
    {
        int balanceRight = balanceFactor(root->right);
        if(balance > 0)
        {
            root->right = rightRotation(root->right);
            return leftRotation(root->right);
        }
        else
        {
            return leftRotation(root);
        }

    }
   
    return root;
}

template<typename Type>
node<Type>* avlTree<Type>::recursiveRemove(node<Type>* root, const Type& value)
{
    if(root == nullptr)
    {
        return root;
    }
    else if(root->value > value)
    {
        root->left = recursiveRemove(root->left , value);
    }
    else if(root->value < value)
    {
        root->right = recursiveRemove(root->right , value);
    }
    else
    {   //only one child or no children
        if(root->left == nullptr || root->right == nullptr)
        {
            node<Type>* current = root->left ? root->left : root->right;
            if(current == nullptr) //no children
            {
                current = root;
                root = nullptr;
            }
            else //only one child
            {
                *root = *current;
            }
            delete current;
        }
        //two children
        else
        {
            node<Type>* current = minValue(root->right);
            root->value = current->value;
            root->right = recursiveRemove(root->right , current->value);

        }
    }
    size--;
    if(root == nullptr)
    {
        return root;
    }

    root->heigh = std::max(getHeigh(root->left) , getHeigh(root->right))+1;

    int balance = balanceFactor(root);

    if(balance > 1 && value < root->left->value)
    {
        return rightRotation(root);
    }
    else if(balance < -1 && value > root->right->value)
    {
        return leftRotation(root);
    }
    else if(balance > 1 && value > root->left->value)
    {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }
    else if(balance < -1 && value < root->right->value)
    {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }
   
    return root;

}

template<typename Type>
std::vector<Type> avlTree<Type>::recursiveInorder(node<Type>* root)const
{
    std::vector<Type> inord;
    if(root->left != nullptr)
    {
        inord = recursiveInorder(root->left);
    }
    inord.push_back(root->value);
    if(root->right != nullptr)
    {
        std::vector<Type> rightInord = recursiveInorder(root->right);
        inord.insert(inord.end() , rightInord.begin() , rightInord.end());
    }
    return inord;

}

template<typename Type>
node<Type>* avlTree<Type>::find(const Type& value)const
{
    return recursiveFind(root , value);
}

template<typename Type>
bool avlTree<Type>::empty()const
{
    return this->root == nullptr;
}

template<typename Type>
int avlTree<Type>::getSize()const
{
    return size;
}

template<typename Type>
void avlTree<Type>::insert(const Type& value)
{
    root = recursiveInsert(root , value);
}

template<typename Type>
void avlTree<Type>::remove(const Type& value)
{
    recursiveRemove(root , value);
}

template<typename Type>
std::vector<Type> avlTree<Type>::inorder()const
{
    if(root == nullptr)
    {
        std::vector<Type> inord;
        return inord;
    }
    return recursiveInorder(root);
}

#endif