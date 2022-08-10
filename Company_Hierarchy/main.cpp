#include<iostream>
#include<exception>
#include"interface.h"
#include"hierarchyObject.h"
#include"operations.h"
#include<forward_list>

void loadOperationNames(std::vector<string>& operationsNames)
{
    std::vector<string> operations;
    operationsNames.push_back("help");
    operationsNames.push_back("load");
    operationsNames.push_back("save");
    operationsNames.push_back("find");
    operationsNames.push_back("num_subordinates");
    operationsNames.push_back("manager");
    operationsNames.push_back("num_employees");
    operationsNames.push_back("overloaded");
    operationsNames.push_back("join");
    operationsNames.push_back("fire");
    operationsNames.push_back("hire");
    operationsNames.push_back("salary");
    operationsNames.push_back("incorporate");
    operationsNames.push_back("modernize");
    operationsNames.push_back("exit");

}

// returns index of the operation if the operation is not valid returns -1
int getOperation(string& inputData) 
{
    int i =0 ;
    string operation;
    while(inputData[i] == ' ' )
    {
        ++i;
    }
    while(inputData[i] != ' ' && inputData[i] != '\0' && inputData[i] != '\n')
    {
        operation += inputData[i];
        ++i;
    }
    inputData.erase(0 , i);
    if(operation == "")
    {
        return -1;
    }
    std::vector<string> operationNames;
    loadOperationNames(operationNames);
    int size = operationNames.size();
    for(int i = 0 ; i < size ; ++i)
    {
        if(operation == operationNames[i])
        {
            return i;
        }
    }

    return -1;
}

//returns first word in a string and erase it from the string
string getName(string& inputData) 
{
    int i =0 ;
    string name;
    while(inputData[i] == ' ' )
    {
        ++i;
    }
    while(inputData[i] != ' ' && inputData[i] != '\0' && inputData[i] != '\0')
    {
        name += inputData[i];
        ++i;
    }
    inputData.erase(0 , i);
    return name;
}

//when user enter relationships in hierarchy
void enterHierarchy(string& relationships)
{
    string inputData ;
    while(std::getline(std::cin , inputData)) // enter hierarchy until "Ctrl + D"
    {
        relationships += (inputData + '\n');
        inputData = "";
    }
}

//check if fileName is correct
bool isCorrectFileName(const string& fileName)
{
    bool haveDot = false;
    for(char symbol : fileName)
    {
        if(symbol == '.')
        {
            haveDot = true;
        }
    }
    return haveDot;
}

//check if hierarchy object's name is correct
bool correctHierarchyName(const string& hierarchyName)
{
    for(char s : hierarchyName)
    {
        if( !(s >= 'a' && s <= 'z') && !(s >= 'A' && s <= 'Z') && !(s >= '0' && s <= '9') && s != '_' )
        {
            return false;
        }
    }
    return true;
}

//enter operations and process them
void start()
{
    Operations operationObj;
    std::forward_list<HierarchyObject*> firmHierarchies;
    string inputData = "";
    string operation;
    string name , managerName , subordinateName;
    string secondHierarchy , newHierarchy;
    while(true)
    {
        std::getline(std::cin , inputData);
        int operation = getOperation(inputData); // operation called
        string hierarchyName = getName(inputData); // name of the hierarchy
        if(operation != 14 && operation != 0)
        {
            if (!correctHierarchyName(hierarchyName) || hierarchyName == "")
            {
                std::cout << "not correct hierarchy name " << std::endl;
                continue;
            }
        }

        switch (operation)
        {
            case 0: // help
                operationObj.help();
                break;
            case 1: // load
                name = getName(inputData); // name of the file
                if(name == "")
                {
                    enterHierarchy(name);
                    try
                    {
                        operationObj.loadFromStream(firmHierarchies , hierarchyName , name);
                    }
                    catch(...)
                    {
                        std::cout<<"incorrect data -> no hierarchy created"<<std::endl;
                    }
                    std::cin.clear();
                }
                else if(!isCorrectFileName(name))
                {
                    std::cout<<"incorect file name"<<std::endl;
                }
                else
                {
                    try
                    {
                        operationObj.loadFromFile(firmHierarchies , hierarchyName , name);
                    }
                    catch(...)
                    {
                        std::cout<<"incorrect data -> no hierarchy created"<<std::endl;
                    }
                }
                break;
            case 2: // save
                name = getName(inputData); // name of the file
                if(name == "")
                {
                    operationObj.save(firmHierarchies , hierarchyName );
                }
                else if(!isCorrectFileName(name))
                {
                    std::cout<<"incorect file name"<<std::endl;
                }
                else
                {
                    operationObj.save(firmHierarchies , hierarchyName , name);
                }
                
                break;
            case 3: // find
                name = getName(inputData);// name of the person to find
                if(name == "")
                {
                    std::cout<<"no name entered "<<std::endl;
                    break;
                }
                operationObj.find(firmHierarchies , hierarchyName , name);
                break;
            case 4: // num_subordinates
                name = getName(inputData);// name of the person for -> subordinates
                if(name == "")
                {
                    std::cout<<"no name entered "<<std::endl;
                    break;
                }
                operationObj.num_subordinates(firmHierarchies , hierarchyName , name);
                break;
            case 5: // manager
                name = getName(inputData); // name of the person whose manager wanna search
                if(name == "")
                {
                    std::cout<<"no name entered "<<std::endl;
                    break;
                }
                operationObj.manager(firmHierarchies , hierarchyName , name);
                break;
            case 6: // num_employees
                operationObj.num_employees(firmHierarchies , hierarchyName );
                break;
            case 7: // overloaded
                operationObj.overloaded(firmHierarchies , hierarchyName );
                break;
            case 8: // join
                secondHierarchy = getName(inputData); // name of the second Hierarchy to join
                newHierarchy = getName(inputData); // name of the newHierarchy
                if(secondHierarchy == "" || newHierarchy == "")
                {
                    std::cout<<"incorrect data for this operation"<<std::endl;
                    break;
                }
                //if hierarchies can't join it will throw an exception
                try
                {
                    operationObj.join(firmHierarchies , hierarchyName , secondHierarchy , newHierarchy);
                }
                catch (...)
                {
                    std::cout << "incorrect data -> can't joine hierarchies" << std::endl;
                }
                break;
            case 9: // fire
                name = getName(inputData); // name of the person to fire
                if(name == "")
                {
                    std::cout<<"no name entered "<<std::endl;
                    break;
                }
                operationObj.fire(firmHierarchies , hierarchyName , name);
                break;
            case 10: // hire
                managerName = getName(inputData); 
                subordinateName = getName(inputData); // name of the person to hire
                if(managerName == "" || subordinateName == "")
                {
                    std::cout<<"incorrect data for this operation"<<std::endl;
                    break;
                }
                operationObj.hire(firmHierarchies , hierarchyName , managerName , subordinateName);
                break;
            case 11: // salary
                name = getName(inputData); // name of the person to take salary
                if(name == "")
                {
                    std::cout<<"no name entered "<<std::endl;
                    break;
                }
                operationObj.salary(firmHierarchies , hierarchyName , name);
                break;// done but have to check for specifics - not in right order
            case 12: // incorporate
                operationObj.incorporate(firmHierarchies , hierarchyName );
                break;
            case 13: // modernize
                operationObj.modernize(firmHierarchies , hierarchyName );
                break;
            case 14: // exit
                operationObj.exit(firmHierarchies);
               return; 
            default:
                std::cout<<"wrong operation"<<std::endl;
                break;
        }
        inputData = "";
    }
}

int main()
{

    start();

    return 0;
}