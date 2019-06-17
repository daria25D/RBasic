#ifndef PARS_H_
#define PARS_H_

#include <iostream>
#include <cstring>
#include <stack>
#include <map>
#include <vector>
#include "./scanner.h"
#include "./lexer.h"

using namespace std;


//class for poliz implementation
class Poliz
{
    vector<Lex> p;
public:
    Poliz();
    ~Poliz();
    void put_lex(const Lex &);
    void put_lex(const Lex &, int);
    int size() const;
    Lex & operator[] (int);
    void print();
};

//class for Parser
class Parser: Scanner {
    Poliz p;
    stack<type_of_lex> s; 
    Lex lex;
    Lex args;
    bool is_file;
    FILE * f;
    static map<type_of_lex, int> Priority;
    
    //functions of recursive-descend method
    void Program();
    void Start();
    void Case();
    void Expression();
    void Exp1();
    void Exp2();
    void Exp3();
    void Exp4();
    void Exp5();
    void Exp6();
    void Exp7();
    void FunctCall();
    void Var();
    void Const();
    void ArgList();
    void Block();
    void Repeat();
    
    //get new lexem
    void get();
    //add lexem to poliz
    void move_to_poliz(type_of_lex);
    //add stack to poliz regarding priority of operations
    void stack_to_poliz_by_priority(type_of_lex);
    //add all stack to poliz
    void stack_to_poliz_all();
    //add stack to poliz before lexem <type_of_lexem> was met in stack
    void stack_to_poliz_till_lex(type_of_lex);
    
public:
    //for console input
    Parser();
    //for file input
    explicit Parser(const char *); 
    //make new line of poliz
    Poliz get_line();    
    ~Parser() = default;
};

#endif