#include<iostream>
#include"ntree.h"
#include"menu.h"

void printInstructions()
{
    std::ifstream in("instruct.txt");
    if(in.is_open())
    {
        std::string line;
        while(std::getline(in , line))
        {
            std::cout<<line<<std::endl;
        }
        std::cout<<"\n";
        in.close();
    }
    else
    {
        std::cout<<"Couldn't open instructions"<<std::endl;
    }
}

void toUpperCase(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void enterOperations()
{
    Menu mn;
    printInstructions();
    std::string operation = "" , input = "";
    bool validOperation = false;
    std::string firstTree = "" , secondTree = "" , fileName = "";

    while(getline(std::cin , input))
    {
        std::stringstream ss(input);
        ss >> operation;
        toUpperCase(operation);
        if(operation == "LOAD")
        {
            ss>>firstTree;
            ss>>fileName;
            mn.load(firstTree , fileName.c_str());
        }
        else if(operation == "SAVE")
        {
            ss>>firstTree;
            ss>>fileName;
            mn.save(firstTree , fileName.c_str());
        }
        else if(operation == "CONTAINS")
        {
            ss>>firstTree;
            ss>>secondTree;
            std::cout<<std::boolalpha<<mn.contains(firstTree , secondTree)<<std::endl;

        }
        else if(operation == "REMOVE")
        {
            ss>>firstTree;
            ss>>secondTree;
            mn.remove(firstTree , secondTree);
        }
        else if(operation == "EXIT")
        {
            ss.clear();
            return;
        }
        else
        {
            std::cout<<"invalid operation"<<std::endl;
        }

    }
}


int main()
{
    enterOperations();

    return 0;
}