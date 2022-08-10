#pragma once
#include <string>
#include <list>
#include <vector>
#include<exception>
#include<sstream>
#include<queue>
#include<cassert>
#include<stack>
#include<algorithm>
#include<forward_list>

using std::string;

struct Tree
{
    string data{};
    std::list<Tree*> children;

    Tree():data("Uspeshnia"){}

    Tree(const string& data_):data(data_){}
};

class Hierarchy
{
public:
    
    Hierarchy()=default;
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept; 
    void operator=(const Hierarchy&) = delete;

    string print()const; 

    int longest_chain() const; 
    bool find(const string& name) const; 
    int num_employees() const; 
    int num_overloaded(int level = 20) const; 

    string manager(const string& name) const; 
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who); 
    bool hire(const string& who, const string& boss); 

    void incorporate();
    void modernize();
    Hierarchy join(const Hierarchy& right) const;

   
    void addRelationship(const string& , const string&); // add one relationship in hierarchy

    Tree* getMaxPaid(Tree*); // returns pointer to the person with max salary

    int salary(const string& who)const; // signed method for salary

    bool findPerson(const string&)const; // check if person exist in current hierarchy

    string printManagerAndSubordinates(const Tree* , std::vector<Tree*>&)const; // sort lexicography subordinates

private:
   
    Tree* hierarchy{};
    
    int maxDepthRecursive(Tree*)const; // recursive max depth for longest chain

    Tree* copyRecursive(Tree*& root , Tree* cpyRoot); // recursive copy for copy constructor

    void clear(Tree*); // recursive clear for destructor

    bool isOverload(Tree* , const int& )const;  // check if current person is oerloaded

    Tree* minName(Tree* , Tree*)const; // takes two names and returns pointer to the lexicographically smaller

    Tree* getManager(const string&)const; // returns pointer to the maneger of current employee

    bool correctLine(const string&)const;

    Tree* findBiggerManager(Tree* , const string& , const string&)const; // returns pointer to hiegher in the hiearachy manager

    static bool lessName(Tree* , Tree*);// takes two names and returns lexicographically smaller

    void managers(Tree* , const string& , std::list<Tree*>&)const;//take all managers of current employee and write them in list 

    void subordinates(Tree* , const string& , std::list<Tree*>&)const;//take all subordinates of current employee and write them in list

    bool canJoin(Tree* , Tree* , Tree*)const;
};

