#ifndef MENU_H
#define MENU_H
#include"ntree.h"
#include <unordered_map>
class Menu
{
    public:

    //tree name , file name
    void load(std::string& , const char*);

    //tree name , file name
    void save(std::string&  , const char*);

    bool contains(std::string&  , std::string& );

    void remove(std::string&  , std::string&);


    private:

    std::unordered_map<std::string , NTree> trees;

};

#endif