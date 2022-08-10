#ifndef HIERARCHYOBJECT_H
#define HIERARCHYOBJECT_H
#include<exception>
#include"interface.h"


struct HierarchyObject
{
    string name{}; // name of Hierarchy object
    Hierarchy* hierarchy{};
    bool isSaved{false}; // flag ->  if Hierarchy is saved 
    HierarchyObject(const string& name_):name(name_){}
    HierarchyObject(const string& name_ , const string& hierarchy_):name(name_) 
    {
       try
       {
          hierarchy = new Hierarchy(hierarchy_);
       }
       catch(...)
       {
            throw std::bad_alloc();
       }
       
    }
    HierarchyObject(const string& name_ , Hierarchy& hierarchy_):name(name_) 
    {
        
        hierarchy = new Hierarchy(hierarchy_);
       
    }
};

#endif