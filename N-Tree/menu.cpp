#ifndef MENU_CPP
#define MENU_CPP
#include"menu.h"

void Menu::load(std::string& treeName, const char* fileName)
{
    
    if(this->trees.find(treeName) == this->trees.end())
    {
        //not found
        try
        {
            this->trees[treeName] = NTree(fileName);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        //exists
        //rewrite tree
        try
        {
            this->trees[treeName].loadFromFile(fileName);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Menu::save(std::string& treeName, const char* fileName)
{
    if(this->trees.find(treeName) == this->trees.end())
    {
        //not found
        std::cout<<"no tree with this name"<<std::endl;
    }
    else
    {
        //exists
        //rewrite tree
        try
        {
            this->trees[treeName].writeInFile(fileName);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

bool Menu::contains(std::string& treeName, std::string& subTreeName)
{
    if(this->trees.find(treeName) == this->trees.end() || this->trees.find(subTreeName) == this->trees.end() )
    {
        //not found
        std::cout<<"wrong name of a tree"<<std::endl;
        return false;

    }
    else
    {
        //exists
        //rewrite tree
        bool containsSubtree = false;
        try
        {
            containsSubtree = this->trees[treeName].contains(this->trees[subTreeName]);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return containsSubtree;
    }
}   

void Menu::remove(std::string& treeName, std::string& subTreeName)
{
    if(this->trees.find(treeName) == this->trees.end() || this->trees.find(subTreeName) == this->trees.end() )
    {
        //not found
        std::cout<<"wrong name of a tree"<<std::endl;

    }
    else
    {
        //exists
        //rewrite tree
        try
        {
            this->trees[treeName].remove(this->trees[subTreeName]);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    
}
#endif