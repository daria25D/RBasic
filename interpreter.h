#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <cstring>
#include "./spars.h"
#include "./lexer.h"
using namespace std;

//typedef Stck;

class Interpreter
{
    static map <string, Type> v_table;
    
    pair <Lex, Lex> get_param();
    vector <pair <Lex, Lex>> get_params();
    void mode();
    void length();
    void c();

    map <string, Type> &global_table;
    map <string, Type> &local_table;
    map <string, VecType> vec_table;

    bool is_func;
    bool is_for;

    Poliz poliz;
    stack<Lex> stck;

    Interpreter() = default;

    Type get_type(const Lex &);
    bool is_var_vec(const Lex &);
    Type & get_str_type(const string &);
    VecType & get_var_vec(const Lex &);
    Type & get_var_type(const Lex &);
    void set_type(const string &, const Type &);

    void function_call();
    Lex  start();

    Interpreter (map <string, Type> &gt, map <string, Type> &lt)
        : global_table(gt), local_table(lt){}

public:
    static void make_line(const Poliz &);
};

#endif