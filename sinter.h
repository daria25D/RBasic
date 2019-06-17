#ifndef INTER_H_
#define INTER_H_

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <utility>
#include <cstring>
#include "./spars.h"

using namespace std;

class Interpreter 
{
    Poliz poliz;
    stack<Lex> st;
    bool is_repeat;
    map<string, Type> global_table;
    map<string, VecType> vec_table;
    
    //method for getting arguments of a function
    vector <Lex> get_args(int);
    
    //gets variable from table or its representation of Type
    Type get_type(const Lex &);
    //check if is in vec_table
    bool is_var_vec(const Lex &);
    //adds variable to table
    void add_to_table(const string &, const Type &);
    //Type representation of variables in tables
    Type & get_var_from_table(const Lex &);
    VecType & get_vec_from_table(const Lex &);
    
    //repeat
    int frepeat(int);

    //for fumction calling
    void function(const Lex &, const Lex &);
    //built-in functions
    void c(int);
    void mode(int);
    void length(int);

    //begin to interpret
    Lex start();
   
public:
    Interpreter() = default;
    void interpret_line(const Poliz &); 
};



#endif
