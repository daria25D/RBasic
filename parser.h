#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <vector>
#include <stack>
#include <cstring>
#include <map>
#include "./scanner.h"
#include "./lexer.h"

using namespace std;

//priority of operations
map<type_of_lex, int> Priority =
{
    {LEX_NOT, 2},
    {LEX_DIV, 3},
    {LEX_MULT, 3},
    {LEX_PLUS, 4},
    {LEX_MINUS, 4},
    {LEX_LESS, 5},
    {LEX_MORE, 5},
    {LEX_LESS_OR_EQUAL, 5},
    {LEX_MORE_OR_EQUAL, 5},
    {LEX_EQUAL, 6},
    {LEX_NOT_EQUAL, 6},
    {LEX_AND, 7},
    {LEX_OR, 8},
    {LEX_COLON, 9},
    {LEX_IN, 10},
    {LEX_SEMICOLON, 11},
    {LEX_LPARENTH, 12}, 
    {LEX_LSQBRACKET, 12}
};

//types of returning result
enum return_t
{
    R_VAR,
    R_VEC,
    R_FUNC
};
typedef  stack<type_of_lex> Stack;
//class for poliz implementation
class Poliz
{
    Lex * p;
    int siz;
    int sfree;
public:
    Poliz(int max_size = 256);
    ~Poliz();
    void put_lex(const Lex &);
    void put_lex(const Lex &, int);
    void blank();
    int size() const;
    Lex & operator[] (int) const;
    void print();
};

template <class T, int max_size> class Stck 
{
    T * s;
    int top;
public:
    Stck();
    void reset();
    void push(const T &);
    T pop();
    bool is_empty() const;
    bool is_full() const;
    T get_top() const;
};


class Parser: Scanner
{
    Lex lex;
    Poliz p;
    Stack st;
    int args;
    return_t r_type;
    bool is_file;

    void Program(Poliz * pp, Stack * s);             //non-terminals
    void Expression(Poliz * pp, Stack * s, bool bp = false);          //...
    void Expr1(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr2(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr3(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr4(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr5(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr6(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void Expr7(Poliz * pp, Stack * s, int f = 0, bool bp = false);
    void FunctCall(Poliz * pp, Stack * s);
    void Var(Poliz * pp, Stack * s);
    void Const(Poliz * pp, Stack * s);
    void ArgList(Poliz * pp, Stack * s);
    void ArgListElem(Poliz * pp, Stack *s);
    void Block(Poliz * pp, Stack *s, bool bp = false);

    void get();
    
    type_of_lex sym() const; // no need
    const string & var() const; // no need
    const Type & type() const; // no need

    bool is_l(lident_t) const;
    bool is_s(type_of_lex) const;

    void priority(type_of_lex, Poliz * pp, Stack *s);
    void empty(Poliz * pp, Stack *s);
    void till_sym(type_of_lex, Poliz * pp, Stack *s);
    void not_end();
    void plus() const;
    void move_to_stack(type_of_lex, Poliz * pp, Stack *s);
public:
    Parser();
    explicit Parser(const char *);
    Poliz get_line();
};

#endif