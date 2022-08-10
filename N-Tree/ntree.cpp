#ifndef NTREE_CPP
#define NTREE_CPP

#include"ntree.h"

NTree::NTree()
{
    this->root = nullptr;
}

NTree::NTree(const int& data_)
{
    try
    {
        this->root = new node(data_);
    }
    catch(const std::exception& e)
    {
        this->root = nullptr;
        std::cerr << e.what() << '\n';
    }

}

NTree::NTree(const NTree& other)
{
    this->copy(this->root , other.root);
}

NTree::NTree(const char* fileName)
{
    std::ifstream in(fileName);
    if(in.is_open())
    {
        //first check for empty file
        std::string word;
        std::string line;
        std::getline(in, line);
        if(line == "")
        {
            this->root = nullptr;
            in.close();
            return;
        }

        //if file isn't empty read root
        std::vector<node*> first;
        std::vector<node*> second;
        std::stringstream ss(line);
        ss >> word;
        while (ss >> word)
        {
            if (word != "|")
            {
                first.push_back(new node(std::stoi(word)));
            }
        }
        ss.str("");
        this->root =first.back();
        //add rest of the elements by level
        while(std::getline(in, line))
        {
            std::stringstream ss(line);
            ss>>word;
            for(auto it = first.begin() ; it != first.end() ; ++it)
            {
                ss>>word;
                while (word != "|")
                {
                    node* child = new node(std::stoi(word));
                    second.push_back(child);
                    this->add((*it) , child);
                    ss>>word;
                }
            }
            first.clear();
            for(auto it = second.begin() ; it != second.end() ; ++it)
            {
                first.push_back(*it);
            }
            second.clear();
            ss.str("");
        }

        in.close();
    }
    else
    {
        this->root = nullptr;
        throw std::invalid_argument("can't open file");
    }
}

NTree& NTree::operator=(const NTree& other)
{
    if(this != &other)
    {
        NTree cpy(other);
        std::swap(this->root , cpy.root);
    }
}

NTree::~NTree()
{
    this->destroy(this->root);
}

void NTree::add(node*& parent , node*& child)
{

    if(parent == this->root)
    {
        if (this->find(root->children, child) == nullptr)
        {
            this->root->children.push_back(child);
            return;
        }
        else
        {
            return;
        }
    }

    std::queue<node*> elems;

    node* curr = this->root;
    elems.push(curr);
    
    while(!elems.empty())
    {
        curr = elems.front();
        elems.pop();

        if(curr == parent)
        {
            if (this->find(curr->children, child) == nullptr)
            {
                curr->children.push_back(child);
                return;
            }
            else
            {
                return;
            }

        }

        for(auto it = curr->children.begin() ; it != curr->children.end() ; ++it)
        {
            elems.push(*it);
        }
    }

    throw std::invalid_argument("no such parent");
}

int NTree::heigh()
{
    return this->heighRecursive(this->root);
}

void NTree::printByLevels() const
{
    if(this->root == nullptr)
    {
        std::cout<<"\n";
        return;
    }
    std::queue<node*> elems;

    node* curr = this->root;
    elems.push(curr);
    
    while(!elems.empty())
    {
        curr = elems.front();
        elems.pop();

        std::cout<<curr->data<<" ";

        for(auto it = curr->children.begin() ; it != curr->children.end() ; ++it)
        {
            elems.push(*it);
        }
    }
    std::cout<<"\n";

}

void NTree::writeInFile(const char* fileName)
{
    std::ofstream out(fileName);
    if(out.is_open())
    {
        //if tree is empty -> create empty file and return
        if(this->root == nullptr)
        {
            out.close();
            return;
        }
        std::vector<node*> first;
        first.push_back(this->root);
        out<<"| "<<this->root->data<<" |\n";
        std::vector<node *> second;
        while (!first.empty())
        {
            out<<"|";
            for (auto it = first.begin(); it != first.end(); ++it)
            {
                out<<" ";
                for(auto iter = (*it)->children.begin() ; iter != (*it)->children.end() ; ++iter)
                {
                    out<<(*iter)->data<<" ";
                    second.push_back(*iter);
                }
                out<<"|";
            }
            out<<"\n";
            first.clear();
            for(auto it = second.begin(); it != second.end(); ++it)
            {
                first.push_back(*it);
            }
            second.clear();
        }

        out.close();
    }
    else
    {
        throw std::invalid_argument("can't open file");
    }
}

void NTree::loadFromFile(const char* fileName)
{
    if(this->root != nullptr)
    {
        this->destroy(this->root);
        this->root = nullptr;
    }
    std::ifstream in(fileName);
    if(in.is_open())
    {
        //first check for empty file
        std::string word;
        std::string line;
        std::getline(in, line);
        if(line == "")
        {
            this->root = nullptr;
            in.close();
            return;
        }

        //if file isn't empty read root
        std::vector<node*> first;
        std::vector<node*> second;
        std::stringstream ss(line);
        ss >> word;
        while (ss >> word)
        {
            if (word != "|")
            {
                first.push_back(new node(std::stoi(word)));
            }
        }
        ss.str("");
        this->root =first.back();
        //add rest of the elements by level
        while(std::getline(in, line))
        {
            std::stringstream ss(line);
            ss>>word;
            for(auto it = first.begin() ; it != first.end() ; ++it)
            {
                ss>>word;
                while (word != "|")
                {
                    node* child = new node(std::stoi(word));
                    second.push_back(child);
                    this->add((*it) , child);
                    ss>>word;
                }
            }
            first.clear();
            for(auto it = second.begin() ; it != second.end() ; ++it)
            {
                first.push_back(*it);
            }
            second.clear();
            ss.str("");
        }

        in.close();
    }
    else
    {
        this->root = nullptr;
        throw std::invalid_argument("can't open file");
    }
}

bool NTree::contains(NTree& other)
{
    return this->containsReursive(this->root , other.root);
}

void NTree::remove(NTree& subTree)
{
    if(this->root == nullptr || subTree.root == nullptr)
    {
        throw std::invalid_argument("empty tree");
    }
    if(heighRecursive(subTree.root) > heighRecursive(this->root))
    {
        throw std::invalid_argument("cant delete deeper tree");
    }

    //take elements from bottom to root in a stack
    std::queue<node*> topToBotom;
    std::stack<node*> bottomToTop;

    node* current = this->root;
    node* father= nullptr;
    int sum = 0;
    bool valueExists = false;
    topToBotom.push(current);
    node* toDel = nullptr;

    while(!topToBotom.empty())
    {
        current = topToBotom.front();
        topToBotom.pop();
        for(auto it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            topToBotom.push(*it);
        }
        bottomToTop.push(current);
    }

    //check for match and delete if needed
    while(!bottomToTop.empty())
    {
        current = bottomToTop.top();
        bottomToTop.pop();
        // if found subtree -> delete else continue
        if(containsHelper(current , subTree.root))
        {
            father = getFather(this->root , current); // take father to check if sum exists in his children

            sum = getSumOfSubtrees(current , subTree.root); //get sum of all subtrees left after deleting

            if(father != nullptr) // if currrent != root
            {
                // if nothing left to be sum
                if(!leftSubtreesToSum(current , subTree.root))
                {
                    for (auto it = father->children.begin(); it != father->children.end(); ++it)
                    {
                        if ((*it)->data == current->data)
                        {
                            father->children.erase(it);
                            this->destroy(current);
                            break;
                        }
                    }
                }
                else// if subtrees left after deleting
                {                   
                    for (auto it = father->children.begin(); it != father->children.end(); ++it)
                    {
                        if ((*it)->data == sum)
                        {
                            valueExists = true;
                        }
                    }
                    if (!valueExists)
                    {
                        current->data = sum;
                        for (auto it = current->children.begin(); it != current->children.end();)
                        {
                            toDel = *it;
                            current->children.erase(it);
                            this->destroy(toDel);
                            toDel = nullptr;
                        }
                        // current->children.clear();
                        // this->destroy(toDel);
                        // toDel = nullptr;
                    }
                    else
                    {
                        for (auto it = father->children.begin(); it != father->children.end(); ++it)
                        {
                            if ((*it)->data == current->data)
                            {
                                father->children.erase(it);
                                this->destroy(current);
                                break;
                            }
                        }
                    }
                }
            }
            else // if current is root
            {
                // if nothing left to be sum
                if(!leftSubtreesToSum(current , subTree.root))
                {
                    this->destroy(this->root);
                    this->root = nullptr;
                }
                else
                {
                    // if subtrees left after deleting
                    this->destroy(this->root);
                    this->root = nullptr;
                    this->root = new node(sum);
                }
      
            }
            //add same node* in stack (may need to be deleted again)
            bottomToTop.push(current);
            valueExists = false;
        }
    }
   

}

void NTree::copy(node*& curr , const node* other)
{
    if(other ==nullptr)
    {
        curr = nullptr;
        return;
    }
    curr = new node(other->data);
    for(auto it = other->children.begin() ; it != other->children.end() ; ++it)
    {
        node* newChild = nullptr;
        copy(newChild , *it);
        curr->children.push_back(newChild);
    }
    
}

void NTree::destroy(node*& curr)
{
    if(curr == nullptr)
    {
        return;
    }
    for(auto it = curr->children.begin() ; it != curr->children.end() ; ++it)
    {
        this->destroy(*it);
    }
    delete curr;
}

node* NTree::find(std::vector<node*>& children, node*& searchedValue)
{
    for(auto it = children.begin() ; it != children.end() ; ++it)
    {
        if((*it)->data == searchedValue->data)
        {
            node* ptr = *it;
            return ptr;
        }
    }
    return nullptr;
}

bool NTree::containsHelper(node*& curr, node*& other)
{
    if(other == nullptr)
    {
        return true;
    }
    else if(curr == nullptr)
    {
        return false;
    }
    else if(curr->data != other->data)
    {
        return false;
    }

    //in queues write only matching elements
    
    std::queue<node*> otherNodes;
    otherNodes.push(other);

    std::queue<node*> currentNodes;
    currentNodes.push(curr);

    node* currentNode = nullptr;
    node* otherNode = nullptr;

    while(!otherNodes.empty())
    {
        currentNode = currentNodes.front();
        currentNodes.pop();

        otherNode = otherNodes.front();
        otherNodes.pop();
        //traverse the children of the subtree and look to see if they are contained in the large one
        //if not -> return false
        //else continue 
        for(auto it = otherNode->children.begin() ; it != otherNode->children.end() ; ++it)
        {
            node* newChild = this->find(currentNode->children , *it);
            if(newChild == nullptr)
            {
                return false;
            }
            currentNodes.push(newChild);
            otherNodes.push(*it);
        }
    }
    return true;
}

bool NTree::containsReursive(node*& curr, node*& other)
{
    if(other == nullptr)
    {
        return true;
    }
    else if(curr == nullptr)
    {
        return false;
    }
    if(containsHelper(curr , other))
    {
        return true;
    }
    else
    {
        for (auto it = curr->children.begin(); it  != curr->children.end(); ++it)
        {
            return this->containsReursive(*it, other);
        }
    }
}

int NTree::sumOfNodes(node*& curr)
{
    if(curr == nullptr)
    {
        return 0;
    }
    else if(curr->children.empty())
    {
        return curr->data;
    }
    std::queue<node*> elems;
    elems.push(curr);
    node* element = nullptr;
    int sum = 0;
    while(!elems.empty())
    {
        element = elems.front();
        elems.pop();
        sum += element->data;
        for(auto it = element->children.begin() ; it != element->children.end() ; ++it)
        {
            elems.push(*it);
        }
    }
    return sum;
}

node* NTree::getFather(node*& currRoot, node*& child)
{
    if(currRoot == nullptr || child == nullptr)
    {
        throw std::invalid_argument("nullptr");
    }
    if(currRoot == child) // root has no father
    {
        return nullptr;
    }
    std::queue<node*> elems;
    elems.push(currRoot);
    node* current = nullptr;
    while(!elems.empty())
    {
        current = elems.front();
        elems.pop();
        for(auto it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            if(*it == child)
            {
                return current;
            }
            elems.push(*it);
        }
    }
    throw std::invalid_argument("no such child");
}

int NTree::heighRecursive(node*& curr)
{
    if(curr == nullptr)
    {
        return 0;
    }
    int maxDepth = 0;
    for(auto it = curr->children.begin() ; it != curr->children.end() ; ++it)
    {
        maxDepth = std::max(maxDepth , heighRecursive(*it));
    }
    return 1 + maxDepth;
}

bool NTree::leftSubtreesToSum(node*& currentTree, node*& otherTree)
{
    std::queue<node*> otherNodes;
    otherNodes.push(otherTree);
    std::queue<node*> currentNodes;
    currentNodes.push(currentTree);
    node* currentNode = nullptr;
    node* otherNode = nullptr;
    std::queue<node*> toSum;
    while(!currentNodes.empty())
    {
        currentNode = currentNodes.front();
        currentNodes.pop();
        otherNode = otherNodes.front();
        otherNodes.pop();
        for(auto it = currentNode->children.begin() ; it != currentNode->children.end() ; ++it)
        {
            node* exist = this->find(otherNode->children , *it);
            if(exist == nullptr)
            {
                toSum.push(*it);
                continue;
                
            }
            currentNodes.push(*it);
            otherNodes.push(exist);
        }
    }
    
    return (toSum.empty() == false);
}

int NTree::getSumOfSubtrees(node*& currentTree, node*& otherTree)
{
    std::queue<node*> otherNodes;
    otherNodes.push(otherTree);
    std::queue<node*> currentNodes;
    currentNodes.push(currentTree);
    node* currentNode = nullptr;
    node* otherNode = nullptr;
    int sum = 0;
    std::queue<node*> toSum;
    while(!currentNodes.empty())
    {
        currentNode = currentNodes.front();
        currentNodes.pop();
        otherNode = otherNodes.front();
        otherNodes.pop();
        for(auto it = currentNode->children.begin() ; it != currentNode->children.end() ; ++it)
        {
            node* exist = this->find(otherNode->children , *it);
            if(exist == nullptr)
            {
                toSum.push(*it);
                continue;
                
            }
            currentNodes.push(*it);
            otherNodes.push(exist);
        }
    }
    while(!toSum.empty())
    {
        currentNode = toSum.front();
        toSum.pop();
        sum += this->sumOfNodes(currentNode);
    }
    return sum;
}


#endif