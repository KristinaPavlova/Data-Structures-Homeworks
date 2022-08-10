#ifndef OPERATIONS_CPP
#define OPERATIONS_CPP
#include"operations.h"

//load instructions from file and print them
void Operations::help()const
{
    std::ifstream in("operationsInstructions.txt");
    if(!in.good())
    {
        std::cout<<"trouble with instructions"<<std::endl;
        in.close();
        return;
    }
    string line;
    while(!in.eof())
    {   
        getline(in , line);
        std::cout<<line<<std::endl;
        line = "";
    }
    in.close();

}

//load hierarchy from given file
void Operations::loadFromFile(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& fileName)const
{
    std::ifstream in(fileName);
    if(!in.good())
    {
        std::cout << "no file with this name , no hierarchy created" << std::endl;
        in.close();
        return;
    }
    string line;
    string inputData;
    while(!in.eof())
    {
        getline(in , line);
        
        if ( line != "")
        {
            line += '\n';
            inputData += line;
            line = "";
        }
    }
    in.close();
    const string data = inputData;

    HierarchyObject* newHierarchy = nullptr;
    try
    {
       newHierarchy = new HierarchyObject(objName , data);
    }
    catch(...)
    {
       throw std::bad_alloc();
    }
    
    firmHierarchies.push_front(newHierarchy);
    std::cout<<objName<<" loaded successfully!"<<std::endl;

}

//load hierarchy from given sring -> entered by user
void Operations::loadFromStream(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& data)const
{
    HierarchyObject* newHierarchy = nullptr;
    try
    {
       newHierarchy = new HierarchyObject(objName , data);
    }
    catch(...)
    {
       throw std::bad_alloc();
    }
    
    firmHierarchies.push_front(newHierarchy);
    std::cout<<objName<<" loaded successfully!"<<std::endl;

}

// save hierarchy in given file
void Operations::save(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& fileName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            std::ofstream out(fileName);
            string outputData;
            outputData = (*it)->hierarchy->print();
            out<<outputData;
            out.close();
            std::cout<<objName<<" saved."<<std::endl;
            (*it)->isSaved = true;
            return;
        }
    }
    std::cout<<"there is no hierarchy with this name"<<std::endl;
}

// print hierarchy -> save with no given file
void Operations::save(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            std::cout<<(*it)->hierarchy->print()<<std::endl;
            return;
        }
    }
    std::cout<<"there is no hierarchy with this name"<<std::endl;
}

//search employee in given hierarchy
void Operations::find(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& subordinateName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            if((*it)->hierarchy->find(subordinateName))
            {
                std::cout<<subordinateName<<" is employed in "<<objName<<std::endl;
                return;
            }
        }
    }
    std::cout<<"there is no "<<subordinateName<<" in "<<objName<<std::endl;

}

//search number of subordinates of given name in current hierarchy
void Operations::num_subordinates(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& name)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {   if((*it)->hierarchy->find(name))
            {
                int numOfsubordinates = (*it)->hierarchy->num_subordinates(name);
                if (numOfsubordinates == 0)
                {
                    std::cout << name << " has no subordinates" << std::endl;
                }
                else
                {
                    std::cout << name << " has " << numOfsubordinates << " subordinates" << std::endl;
                }
                return;
            }
        }
    }
    std::cout<<"there is no "<<name<<" in "<<objName<<std::endl;

}

//find manager of given employee in current hierarchy
void Operations::manager(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& name)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    else if(name == "Uspeshnia")
    {
        std::cout<<"Uspeshniq has no manager"<<std::endl;
        return;
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            if((*it)->hierarchy->find(name))
            {
                std::cout << "The manager of " << name << " is " << (*it)->hierarchy->manager(name) << std::endl;
                return;
            }
        }
    }
    std::cout<<"There is no "<<name<<" in "<<objName<<std::endl;

}

// search number of all employees in current hierarchy
void Operations::num_employees(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            std::cout<<"The are "<<(*it)->hierarchy->num_employees()<<" employees in "<<objName<<std::endl;
            return;
        }
    }
    std::cout<<"There is no "<<objName<<" int the firm"<<std::endl;

}

// search number of all overloaded employees in current hierarchy
void Operations::overloaded(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            int numberOfOverloaded = (*it)->hierarchy->num_overloaded();
            if( numberOfOverloaded == 0)
            {
                std::cout<<"No overloaded employees in "<<objName<<std::endl;
            }
            else
            {
                std::cout<<"There are "<<numberOfOverloaded<<" overloaded employees in "<<objName<<std::endl;
            }

            return;
        }
    }
    std::cout<<"There is no "<<objName<<" in the firm"<<std::endl;

}

//join two hierarchies
void Operations::join(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& obj1Name , const string& obj2Name , const string& newObj)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    bool foundFirst = false , foundSecond = false;
    Hierarchy* firstHierarchy = nullptr;
    Hierarchy* secondHierarchy = nullptr;
    // search both hierarchies in all firmHierarchies
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == obj1Name)
        {
           firstHierarchy = (*it)->hierarchy;
        }
        else if((*it)->name == obj2Name)
        {
            secondHierarchy = (*it)->hierarchy;
        }
        if((*it)->name == newObj)
        {
            firmHierarchies.remove(*it);
        }
    }
    if(firstHierarchy == nullptr ||  secondHierarchy == nullptr)
    {
        std::cout<<"One of those hierarchies doesn't exist"<<std::endl;
        return;
    }
    //create new joined Hierarchy
    Hierarchy* newHierarchy = nullptr;
    try
    {
        newHierarchy = new Hierarchy((*firstHierarchy));
    }
    catch(...)
    {
        throw std::bad_alloc();
    }

    Hierarchy *joined = nullptr;
    try
    {
        joined = new Hierarchy(newHierarchy->join((*secondHierarchy)));
    }
    catch(const std::exception& e)
    {
        throw std::bad_alloc();
    }
    

    //create new Hierarchy object with the new Hierarchy
    HierarchyObject* newHierarchyObject = nullptr;
    try
    {
        newHierarchyObject = new HierarchyObject(newObj , *joined);
    }
    catch(...)
    {
        throw std::bad_alloc();
    }
    // push new Hierarchy object in all firmHierarchies
    firmHierarchies.push_front(newHierarchyObject);
    std::cout<<newObj<<" created."<<std::endl;
    

}

//fire employee in current hierarchy
void Operations::fire(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& name)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            if((*it)->hierarchy->find(name))
            {
                (*it)->hierarchy->fire(name);
                std::cout << name << " was fired." << std::endl;
                return;
            }
            
        }
    }
    std::cout<<"There is no "<<name<<" in "<<objName<<std::endl;
}

void Operations::hire(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& newSubordinate ,const string& manager )const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            (*it)->hierarchy->hire(newSubordinate , manager);
            std::cout<<newSubordinate<<" was hired."<<std::endl;
            return;
        }
    }
    std::cout<<"There is no "<<objName<<std::endl;
}

//find salary of given employee in current hierarchy
void Operations::salary(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName, const string& name)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            if((*it)->hierarchy->find(name))
            {
                std::cout << "The salary is " << (*it)->hierarchy->getSalary(name) << "BGN" << std::endl;
                return;
            }
        }
    }
    std::cout<<"There is no "<<name<<" in "<<objName<<std::endl;
}

//incorporate current hierarchy
void Operations::incorporate(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            (*it)->hierarchy->incorporate();
            std::cout<<objName<<" incorporated."<<std::endl;
            return;
        }
    }
    std::cout<<"There is no "<<objName<<std::endl;

}

//modernize current hierarchy
void Operations::modernize(std::forward_list<HierarchyObject*>& firmHierarchies ,  const string& objName)const
{
    if(firmHierarchies.empty())
    {
        throw std::invalid_argument("no firm hierarchies");
    }
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        if((*it)->name == objName)
        {
            (*it)->hierarchy->modernize();
            std::cout<<objName<<" modernized"<<std::endl;
            return;
        }
    }
    std::cout<<"There is no "<<objName<<std::endl;

}

//exit the program
void Operations::exit(std::forward_list<HierarchyObject*>& firmHierarchies)const
{
    bool allSaved = true;
    char answer;
    string fileName;
    for(std::forward_list<HierarchyObject*>::iterator it = firmHierarchies.begin() ; it != firmHierarchies.end() ; ++it)
    {
        //check for unsaved hierarchies
        if((*it)->isSaved == false) 
        {
            allSaved = false;
            std::cout<<(*it)->name<<" is modified, but not saved.";
            std::cout << "Do you wanna save it ? (y/n) " << std::endl;
            //ask user if he want to save unsaved hierarchies
            do
            {
                std::cin >> answer;
            } while (answer != 'y' && answer != 'n');
            //if user want to save hierarchies -> user enter file name
            if (answer == 'y')
            {
                std::cout<< "Enter file name to save it"<<std::endl;
                std::cin>>fileName;
                //save current hierarchy in given file
                save(firmHierarchies , (*it)->name , fileName);
            }
            
        }
    }
    std::cout<<"Goodbye!"<<std::endl;
}

#endif