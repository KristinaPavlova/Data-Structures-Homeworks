#ifndef INTERFACE_CPP
#define INTERFACE_CPP
#include"interface.h"
#include<iostream>
Hierarchy::Hierarchy(Hierarchy &&r) noexcept
{
    std::swap(hierarchy , r.hierarchy);
}

Hierarchy::Hierarchy(const Hierarchy &r)
{
    try
    {
        hierarchy = new Tree();
    }
    catch(...)
    {
       throw std::bad_alloc();
    }
    std::queue<Tree*> cpyTree;
    std::queue<Tree*> tree;
    cpyTree.push(r.hierarchy);
    tree.push(hierarchy);
    Tree* currentCpy = nullptr;
    Tree* current = nullptr;
    while(!cpyTree.empty())
    {
        currentCpy = cpyTree.front();
        cpyTree.pop();
        current = tree.front();
        tree.pop();
        for(std::list<Tree*>::iterator it = currentCpy->children.begin() ; it != currentCpy->children.end() ; ++it)
        {
            Tree* newTree= nullptr;
            try
            {
                newTree = new Tree((*it)->data);
            }
            catch(...)
            {
                throw std::bad_alloc();
            }
            current->children.push_back(newTree);
            cpyTree.push((*it));
            
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            tree.push((*it));
        }


    }
    
}

Hierarchy::Hierarchy(const string &data)
{
    try
    {
        hierarchy = new Tree();
    }
    catch (...)
    {
        throw std::bad_alloc();
    }
    string str, manager, subordinate;
    std::istringstream out(data);

    while (std::getline(out, str, '\n'))
    {
        if(!correctLine(str))
        {
            clear(hierarchy);
            hierarchy = nullptr;
            throw std::invalid_argument("incorect data");
        }
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        manager = "";
        subordinate = "";

        int i = 0;
        while(str[i] != '-')
        {
            ++i;
        }
        manager = str.substr(0 , i);
        subordinate = str.substr(i+1 , str.length()-1);
        if(!findPerson(manager))
        {
            clear(hierarchy);
            hierarchy = nullptr;
            throw std::invalid_argument("incorect data");
        }
        addRelationship(manager , subordinate);
        
    }
}

Hierarchy::~Hierarchy() noexcept
{
    clear(hierarchy);
}

string Hierarchy::print()const
{
    if (hierarchy->children.empty())
    {
        return "";
    }
    string relationships;
    std::queue<Tree *> employees;
    std::vector<Tree*> subordinates; // for sorting subordinates
    employees.push(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        current = employees.front();
        employees.pop();
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            subordinates.push_back((*it));
        }
        relationships += printManagerAndSubordinates(current , subordinates);
        for(int i =0 ; i< subordinates.size() ; ++i)
        {
            employees.push(subordinates[i]);
        }
        subordinates.clear();
    }
    return relationships;
}

int Hierarchy::longest_chain() const
{
    if(hierarchy->children.empty())
    {
        return 0;
    }
    return maxDepthRecursive(hierarchy);
}

bool Hierarchy::find(const string& name) const
{
    if(hierarchy->children.empty()) 
    {
        return false;
    }
    std::queue<Tree *> employees;
    employees.push(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        current = employees.front();
        if(current ->data == name)
        {
            return true;
        }
        employees.pop();
        
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            if((*it)->data == name)
            {
                return true;
            }
            employees.push((*it));
        }
    }
    return false;

}

int Hierarchy::num_employees() const
{
    if(hierarchy->children.empty())
    {
        return 0;
    }
    int counter = 0;
    std::queue<Tree *> employees;
    employees.push(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        ++counter;
        current = employees.front();
        employees.pop();
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            employees.push((*it));
        }
    }
    return counter;

}

//count all overloaded employees
int Hierarchy::num_overloaded(int level) const
{
    if(hierarchy->children.empty())
    {
        return 0;
    }
    int count = 0;
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(isOverload(current , level))
        {
            ++count;
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            employees.push((*it));
        }
    }
    return count;

}

string Hierarchy::manager(const string& name) const
{
    if(hierarchy->data == name) // if name == "Uspeshnia"
    {
        return "";
    }
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            if((*it)->data == name)
            {
                return current->data;
            }
            employees.push((*it));
        }
    }
    return "";

}

int Hierarchy::num_subordinates(const string& name) const
{
    if(hierarchy->children.empty())
    {
        return -1;
    }
    std::queue<Tree *> employees;
    employees.push(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if (current->data == name)
        {
           return current->children.size();
        }
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            employees.push((*it));
        }
    }
    return -1;
}

unsigned long Hierarchy::getSalary(const string& who) const
{
    if(hierarchy->data == who && hierarchy->children.empty())
    {
        return 0;
    }
    std::list<Tree *> employees;
    employees.push_back(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        current = employees.front();
        employees.pop_front();
        if (current->data == who) // if searched employee is found
        {
            employees.clear();
            int salary = 0;
            for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
            {
                //for every subordinate of employee +500
                salary += 500;
                // directly push employee children -> childrens
                for (std::list<Tree *>::iterator iter = (*it)->children.begin(); iter != (*it)->children.end(); ++iter)
                {
                    employees.push_back((*iter));
                }
            }
            while(!employees.empty())
            {
                current = employees.front();
                salary +=50;
                employees.pop_front();
                for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
                {
                    employees.push_back((*it));
                }
            }
            return salary;
        }
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            employees.push_back((*it));
        }
    }
    return -1;
}

bool Hierarchy::fire(const string& who)
{
    if(hierarchy->data == who || hierarchy->children.empty())
    {
        return false;
    }
    std::stack<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.top();
        employees.pop();
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            if((*it)->data == who)
            {   
                for(std::list<Tree *>::iterator iter = (*it)->children.begin(); iter != (*it)->children.end(); ++iter)
                {
                    current->children.push_back((*iter));
                }
                current->children.remove((*it));
                return true;
            }
            employees.push((*it));
        }   
    }
    return false;
}

bool Hierarchy::hire(const string& who, const string& boss)
{
    if(hierarchy->children.empty())
    {
        return false;
    }
    Tree* manager = nullptr; // keeps poiter to the new manager of who
    Tree* subordinateManager = nullptr; // keeps pointer to the old manager of who if who exist in hierarchy
    Tree* subordinate = nullptr; // keeps pointer to who if he exist in hierarchy
    Tree* current = nullptr; // for going through the tree
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(current->data == boss)
        {
            manager = current;
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            if((*it)->data == who)
            {
                subordinate = (*it);
                subordinateManager = current;
            }
            employees.push((*it));
        }
    }
    if(manager == nullptr)
    {
        return false;
    }
    else if(subordinate != nullptr) // if the subordinate exist in hierarchy
    {
        if(manager->data != subordinateManager->data) // check if new manager and previous manager are equal
        {
            manager->children.push_back(subordinate);
            subordinateManager->children.remove(subordinate);
        }
        return true;
    }
    else
    {
        try
        {
            subordinate = new Tree(who);
        }
        catch(...)
        {
            throw std::bad_alloc();
        }
        manager->children.push_back(subordinate);
        return true;
        
    }
}

void Hierarchy::incorporate()
{
    if(hierarchy->children.empty())
    {
        return;
    }
    Tree* current = nullptr;
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    std::list<Tree*> toBeRaised; // keeps pointers to all employees who should be fired -> write them in reversed order
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(num_subordinates(current->data) >= 2)
        {
            Tree* toAdd = getMaxPaid(current);
            assert(toAdd != nullptr);
            toBeRaised.push_front(toAdd); // push_front -> keep them in reversed order
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            employees.push((*it));
        }
    }
    Tree* managerToChange = nullptr;

    for(std::list<Tree*>::iterator it = toBeRaised.begin() ; it != toBeRaised.end() ; ++it)
    {
        managerToChange = getManager((*it)->data); // take manager of the subordinate who should be fired
       
        for(std::list<Tree*>::iterator iter = managerToChange->children.begin() ; iter != managerToChange->children.end() ; )
        {
            if( (*iter)->data != (*it)->data)
            {
                current = (*iter);
                ++iter;
                (*it)->children.push_back((current));
                managerToChange->children.remove((current));
            }
            else
            {
                ++iter;
            }
        }

    }
    

}

void Hierarchy::modernize()
{
    if(hierarchy->children.empty())
    {
        return;
    }
    std::forward_list<Tree*> toFire; // keeps all employees on odd levels in reversed order
    Tree* current = hierarchy;
    std::list<Tree*> level; // keeps all elements of current level
    std::list<Tree*> nextLevel; // keeps all elements of next level of first
    int levelCount = 0;

    for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
    {
        level.push_back((*it));
        for (std::list<Tree *>::iterator iter = (*it)->children.begin(); iter != (*it)->children.end(); ++iter)
        {
            nextLevel.push_back((*iter));
        }
    }
    ++levelCount;
    while(!level.empty())
    {
        if(levelCount % 2 == 1) // if level is odd add all employees on this level in toFire
        {
            for (std::list<Tree *>::iterator it = level.begin(); it != level.end(); ++it)
            {
                if (!(*it)->children.empty())
                {
                    toFire.push_front((*it));
                }
            }
        }
        level.clear();
        std::swap(level , nextLevel);
        for (std::list<Tree *>::iterator it = level.begin(); it != level.end(); ++it)
        {
            for (std::list<Tree *>::iterator iter = (*it)->children.begin(); iter != (*it)->children.end(); ++iter)
            {
                nextLevel.push_back((*iter));
            }
        }
        ++levelCount;
    }
    // fire all employees in odd levels
    for(std::forward_list<Tree*>::iterator it = toFire.begin() ; it != toFire.end() ; ++it )
    {
        fire((*it)->data);
    }
}

Hierarchy Hierarchy::join(const Hierarchy& right) const 
{
    Hierarchy newHierarchy(right);
    if(hierarchy == right.hierarchy)
    {
        return newHierarchy;
    }
    Tree* current = nullptr;
    std::queue<Tree*> elems;
    elems.push(hierarchy);
    while(!elems.empty()) // check if hierarchies can join
    {
        current = elems.front();
        elems.pop();
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {

            if(newHierarchy.find(current->data) && newHierarchy.find((*it)->data) && ( !canJoin(newHierarchy.hierarchy , (*it) , hierarchy) || !canJoin(hierarchy , (*it) , newHierarchy.hierarchy)))
            {
                throw std::invalid_argument("in one of the hierarchies the subordinate is manger");
                break;
            }
            elems.push((*it));
        }
        
    }
    current = nullptr;
    elems.push(hierarchy);
    while(!elems.empty())
    {
        current = elems.front();
        elems.pop();
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            // if the subordinate have different managers in two hieararchies
            if(newHierarchy.find((*it)->data) && newHierarchy.manager((*it)->data) != this->manager((*it)->data)) 
            {
                Tree* biggerManager = findBiggerManager(newHierarchy.hierarchy , newHierarchy.manager((*it)->data) , this->manager((*it)->data));
                newHierarchy.hire((*it)->data , biggerManager->data);
            }
            else // if the subordinate exist only in one
            {
                newHierarchy.hire((*it)->data , current->data);
            }
            elems.push((*it));
        }
        
    }
    return newHierarchy;
}

// add one pair of "manger- subordinate" in hierarchy
void Hierarchy::addRelationship(const string& manager, const string& subordinate) 
{
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(current->data == manager)
        {
            Tree* newTree = nullptr;
            try
            {
                newTree = new Tree(subordinate);
            }
            catch(...)
            {
                throw std::bad_alloc();
            }
            
            current->children.push_back(newTree);
            return;
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            employees.push((*it));
        }
    }

}

int Hierarchy::salary(const string& who)const
{
    if(hierarchy->data == who && hierarchy->children.empty())
    {
        return 0;
    }
    std::list<Tree *> employees;
    employees.push_back(hierarchy);
    Tree *current = nullptr;
    while (!employees.empty())
    {
        current = employees.front();
        employees.pop_front();
        if (current->data == who)
        {
            employees.clear();
            int salary = 0;
            for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
            {
                salary += 500;
                for (std::list<Tree *>::iterator iter = (*it)->children.begin(); iter != (*it)->children.end(); ++iter)
                {
                    employees.push_back((*iter));
                }
            }
            while(!employees.empty())
            {
                current = employees.front();
                salary +=50;
                employees.pop_front();
                for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
                {
                    employees.push_back((*it));
                }
            }
            return salary;
        }
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            employees.push_back((*it));
        }
    }
    return -1;
}

bool Hierarchy::findPerson(const string& name)const
{
    if( name == "Uspeshnia")
    {
        return true;
    }
    else if( hierarchy->children.empty())
    {
        return false;
    }
    std::queue<Tree*> employees;
    Tree* current = nullptr;
    employees.push(hierarchy);
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(current->data == name)
        {
            return true;
        }
        for(std::list<Tree*>::iterator it  = current->children.begin() ; it != current->children.end() ; ++it)
        {
            if((*it)->data == name)
            {
                return true;
            }
            employees.push((*it));
        }
    }
    return false;
    
}

string Hierarchy::printManagerAndSubordinates(const Tree* root , std::vector<Tree*>& subordinates)const
{
    string relationships;
    int start = 0;
    
    std::sort(subordinates.begin() , subordinates.end() , lessName);

    int size = subordinates.size();
    for (int i = 0; i < size; ++i)
    {
        relationships += (root->data + "-" + subordinates[i]->data + '\n');
    }
    return relationships;
}

//find max paid person in current hierarchy
Tree* Hierarchy::getMaxPaid(Tree* ptr)
{
    std::queue<Tree*> subordinates;
    Tree* maxPaidPerson = nullptr ;
    int maxSalary = -1;
    Tree *current = ptr;
    for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
    {
        subordinates.push((*it));
    }
    while(!subordinates.empty())
    {
        current = subordinates.front();
        assert(current!= nullptr);
        subordinates.pop();
        if( maxSalary < salary(current->data))
        {
            maxSalary = salary(current->data);
            maxPaidPerson = current;
        }
        else if(maxSalary == salary(current->data)) // if tho employees have equel salaries take lexicography smaller
        {
            maxPaidPerson = (minName(maxPaidPerson , current));
            assert(maxPaidPerson!=nullptr);
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            subordinates.push((*it));
        }
    }
    //returns pointer to the max paid person in hierarchy
    return maxPaidPerson;
}


int Hierarchy::maxDepthRecursive(Tree* root)const
{
    if(root == nullptr)
    {
        return 0;
    }
    int maxDepth = 0;
    for(std::list<Tree*>::iterator it = root->children.begin() ; it != root->children.end() ; ++it)
    {
        maxDepth = std::max(maxDepth , maxDepthRecursive((*it)) );
    }
    return maxDepth+1;

}


void Hierarchy::clear(Tree* root)
{
    if(root != nullptr)
    {
        for(std::list<Tree*>::iterator it = root->children.begin() ; it != root->children.end() ; ++it )
        {
            clear((*it));
        }
        delete root;
    }
}


bool Hierarchy::isOverload(Tree* root  , const int& level)const
{
    if(root == nullptr || root->children.empty())
    {
        return false;
    }
    std::queue<Tree*> subordinates;
    subordinates.push(root);
    int count = 0;
    Tree* current = nullptr;
    while(!subordinates.empty())
    {
        current = subordinates.front();
        subordinates.pop();
        for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
        {
            count++;
            subordinates.push((*it));
        }
    }
    return count > level;
}


Tree* Hierarchy::minName(Tree* firstPtr , Tree* secondPtr)const
{
    if(firstPtr == nullptr || secondPtr == nullptr)
    {
       throw std::invalid_argument("one of them is nullptr");
    }
    string first ="" , second ="";
    first +=(*firstPtr).data;
    second +=(*secondPtr).data;
    int size = first.length() > second.length() ? first.length() : second.length();
    for(int i =0 ; i < size ; ++i)
    {
        if(first[i] > second[i])
        {
            return secondPtr;
        }
        else if(first[i] < second[i])
        {
            return firstPtr;
        }
    }
    throw std::invalid_argument("same names");
}


Tree* Hierarchy::getManager(const string& subordinate)const
{
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        for(std::list<Tree*>::iterator it = current->children.begin() ; it != current->children.end() ; ++it)
        {
            if((*it)->data == subordinate)
            {
                return current;
            }
            employees.push((*it));
        }
    }
    throw std::invalid_argument("no such person in the hierarchy");
}

//check for correct input data
bool Hierarchy::correctLine(const string& line)const
{
    string manager , subordinate;
    int i =0;
    while(line[i] == ' ')
    {
        ++i;
    }
    for( ; ( line[i] != ' ' && line[i] != '-' ) && line[i] !='\0' ; ++i)
    {
        manager += line[i];
    }

    while(line[i] == ' ' || line[i] == '-')
    {
        ++i;
    }
    for( ; line[i] != ' ' && line[i] != '-' && line[i] !='\0'; ++i)
    {
        subordinate += line[i];
    }
    for( ; line[i] != '\0' ; ++i)
    {
        if(line[i] != ' ')
        {
            return false;
        }
    }
    if(manager.length() == 0 || subordinate.length() == 0)
    {
        return false;
    }
    return true;


}

//go through the tree by levels and check if one of the managers is found or both of them 
Tree* Hierarchy::findBiggerManager(Tree* root, const string& firstName, const string& secondName)const // returns pointer to hiegher in the hiearachy manager
{
    if(firstName == "Uspeshnia" || secondName == "Uspeshnia")
    {
        return root;
    }
    Tree* firstManager = nullptr; 
    Tree* secondManager = nullptr;
    std::list<Tree*> firstLevel;  // keeps all elems on current level
    std::list<Tree*> secondLevel; // keeps all elems on the next level of firstLevel
    for(std::list<Tree*>::iterator it  = root->children.begin() ; it != root->children.end() ; ++it)
    {
        if((*it)->data == firstName)
        {
            firstManager = (*it);
        }
        if((*it)->data == secondName)
        {
            secondManager = (*it);
        }
        firstLevel.push_front((*it));
    }
    while(!firstLevel.empty())
    {
        //if both managers are on the same level return lexicography smaller
        if(firstManager && secondManager)
        {
            return minName(firstManager , secondManager);
        }
        else if(firstManager)
        {
            return firstManager;
        }
        else if(secondManager)
        {
            return secondManager;
        }

        for(std::list<Tree*>::iterator it = firstLevel.begin() ; it != firstLevel.end() ; ++it)
        {
            for(std::list<Tree*>::iterator iter = (*it)->children.begin() ; iter != (*it)->children.end() ; ++iter)
            {
                if ((*iter)->data == firstName)
                {
                    firstManager = (*iter);
                }
                if ((*iter)->data == secondName)
                {
                    secondManager = (*iter);
                }
                secondLevel.push_front((*iter));
            }
        }
        firstLevel.clear();
        
        //if both managers are on the same level return lexicography smaller
        if(firstManager && secondManager)
        {
            return minName(firstManager , secondManager);
        }
        else if(firstManager)
        {
            return firstManager;
        }
        else if(secondManager)
        {
            return secondManager;
        }

        for(std::list<Tree*>::iterator it = secondLevel.begin() ; it != secondLevel.end() ; ++it)
        {
            for(std::list<Tree*>::iterator iter = (*it)->children.begin() ; iter != (*it)->children.end() ; ++iter)
            {
                if ((*iter)->data == firstName)
                {
                    firstManager = (*iter);
                }
                if ((*iter)->data == secondName)
                {
                    secondManager = (*iter);
                }
                firstLevel.push_front((*iter));
            }
        }
        secondLevel.clear();


    }


}

// if first name < second name -> return true
bool Hierarchy::lessName(Tree* firstPtr , Tree* secondPtr)
{
    if(firstPtr == nullptr || secondPtr == nullptr)
    {
       throw std::invalid_argument("one of them is nullptr");
    }
    string first ="" , second ="";
    first += firstPtr->data;
    second += secondPtr->data;
    int size = first.length() > second.length() ? first.length() : second.length();
    for(int i =0 ; i < size ; ++i)
    {
        if(first[i] > second[i])
        {
            return false;
        }
        else if(first[i] < second[i])
        {
            return true;
        }
    }
    throw std::invalid_argument("same names");
}

//take all managers of current employee and write them in list 
void Hierarchy::managers(Tree* hierarchy, const string& name, std::list<Tree*>& managersOfEmployee)const
{
    Tree* current = nullptr;
    std::string nameOfCurrentManager = name ;
    while(nameOfCurrentManager != "Uspeshnia")
    {
        current = getManager(nameOfCurrentManager);
        nameOfCurrentManager = current->data;
        managersOfEmployee.push_back(current);
        
    } 
  
}

//take all subordinates of current employee and write them in list 
void Hierarchy::subordinates(Tree* hierarchy, const string& name, std::list<Tree*>& subordinates)const
{
    std::queue<Tree*> employees;
    employees.push(hierarchy);
    Tree* current = nullptr;
    while(!employees.empty())
    {
        current = employees.front();
        employees.pop();
        if(current->data == name) // when employee found
        {
            std::queue<Tree*>subordinatesOfEmployee;
            subordinatesOfEmployee.push(current);
            while(!subordinatesOfEmployee.empty())
            {
                current = subordinatesOfEmployee.front();
                subordinatesOfEmployee.pop();
                for (std::list<Tree *>::iterator it = current->children.begin(); it != current->children.end(); ++it)
                {
                    subordinates.push_back((*it));
                    subordinatesOfEmployee.push((*it));
                }
            }
            return;
        }
        for(std::list<Tree*>::iterator it = current->children.begin() ; it!=current->children.end() ; ++it)
        {
            employees.push((*it));
        }

    }

}

//receives two hierarchies and name
//taka managers of the employee in hierarchy one
//taka subordinates of the employee in hierarchy two
//compare managers and subordinates
bool Hierarchy::canJoin(Tree* firstHierarchy, Tree* employee , Tree* secondHierarchy)const
{
    if(employee->data == "Uspeshnia")
    {
        return true;
    }
    std::list<Tree *> subordinatesInSecond;
    subordinates(secondHierarchy, employee->data, subordinatesInSecond);
    std::list<Tree *> managersInFirst;
    managers(firstHierarchy, employee->data, managersInFirst);
    if (managersInFirst.empty() || subordinatesInSecond.empty())
    {
        return true;
    }

    for (std::list<Tree *>::iterator it = managersInFirst.begin(); it != managersInFirst.end(); ++it)
    {
        for (std::list<Tree *>::iterator iter = subordinatesInSecond.begin(); iter != subordinatesInSecond.end(); ++iter)
        {
            if ((*it)->data == (*iter)->data)
            {
                return false;
            }
        }
    }
    return true;
   
}


#endif