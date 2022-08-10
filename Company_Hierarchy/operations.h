#ifndef OPERATIONS_H
#define OPERATIONS_H
#include"interface.h"
#include"hierarchyObject.h"
#include<cstring>
#include<iostream>
#include<fstream>
#include<exception>
using string = std::string;

//operations working with Hierarchy object
struct Operations 
{
    void help()const;

    void loadFromStream( std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;

    void loadFromFile( std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;


    void save(std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;
    
    void save(std::forward_list<HierarchyObject*>& ,  const string& )const;

    void find(std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;
    
    void num_subordinates(std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;
    
    void manager(std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;
    
    void num_employees(std::forward_list<HierarchyObject*>& ,  const string& )const;
    
    void overloaded(std::forward_list<HierarchyObject*>& ,  const string& )const;
    
    void join(std::forward_list<HierarchyObject*>& ,  const string& , const string&  , const string&  )const;

    void fire(std::forward_list<HierarchyObject *> &, const string &, const string &) const;

    void hire(std::forward_list<HierarchyObject *> &, const string &, const string & ,const string & ) const;

    void salary(std::forward_list<HierarchyObject*>& ,  const string& , const string&)const;

    void incorporate(std::forward_list<HierarchyObject*>& ,  const string& )const;

    void modernize(std::forward_list<HierarchyObject*>& ,  const string& )const;

    void exit(std::forward_list<HierarchyObject*>& )const;


};



#endif