#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include "./types.h"
using namespace std;

//types of lexems used in project
enum lident_t 
{
    L_SYM,
    L_VAR,
    L_CONST,
    L_OFST,
    L_VEC,
    L_FUNC,
    L_NULL
};

//class of lexems
class Lex
{
    lident_t t_lex;
    void * v_lex;
public:
    //constructors
    Lex();
    Lex(const Type &);
    Lex(const string &);
    Lex(const Lex &);
    Lex(type_of_lex);
    Lex(int, bool);
    Lex(const VecType &);
    //get type of lexem
    lident_t get_type();

    void to_vec();
    void to_func();
    void increase_ofst();
    //get *(type of symbol *)v_lex, if lexem is symbol
    type_of_lex sym() const;
    //get *(string *)v_lex, if lexem is variable/const/func etc.
    string & var() const;
    //get *(Type *)v_lex, if lexem is vector
    Type & type() const;
    //get *(int *)v-lex - for offset
    int ofst() const;
    //get *(VecType *)v_lex
    VecType vec() const;
    //check type of lexem
    bool is_l(lident_t) const;
    //check type of symbol, if lexem is symbol
    bool is_s(type_of_lex) const;

    //overload of =
    Lex & operator= (const Lex &);

    //destructor
    ~Lex() = default;
};

#endif