#include <iostream>
#include <cstring>
#include "./lexer.h"

using namespace std;

Lex::Lex(const Lex & l) {
    t_lex = l.t_lex;
    switch (t_lex) {
    case L_SYM:
        v_lex = new type_of_lex(*(type_of_lex *)l.v_lex);
        break;
    case L_FUNC:
    case L_VAR:
        v_lex = new string(*(string *)l.v_lex);
        break;
    case L_CONST:
        v_lex = new Type(*(Type *)l.v_lex);
        break;
    case L_OFST:
        v_lex = new int(*(int *)l.v_lex);
        break;
    case L_VEC:
        v_lex = new VecType(*(VecType *)l.v_lex);
        break;
    default:
        break;
    }
}
Lex::Lex() {
    t_lex = L_NULL;
}
Lex::Lex(type_of_lex t) {
    t_lex = L_SYM;
    v_lex = new type_of_lex(t);
}
Lex::Lex(const string & s) {
    t_lex = L_VAR;
    v_lex = new string(s);
}
Lex::Lex(const Type & t) {
    t_lex = L_CONST;
    v_lex = new Type(t);
}
Lex::Lex(int i, bool b) {
    t_lex = L_OFST;
    v_lex = new int(i);
}
Lex::Lex(const VecType & v) {
    t_lex = L_VEC;
    v_lex = new VecType(v);
}

lident_t Lex::get_type() {
    return t_lex;
}
void Lex::to_vec() {
    t_lex = L_VEC;
    v_lex = (Type *)v_lex;
}
void Lex::to_func() {
    t_lex = L_FUNC;
}
void Lex::increase_ofst() {
    if (t_lex == L_OFST) {
        *(int *)v_lex = ofst() + 1;
    }
}
bool Lex::is_l(lident_t t) const {
    return t == t_lex;
}
bool Lex::is_s(type_of_lex t) const {
    if (is_l(L_SYM)) return this->sym() == t;
    else return false;
}
Lex & Lex::operator= (const Lex & l) {
    this->Lex::~Lex();
    t_lex = l.t_lex;    
    switch (t_lex) {
    case L_SYM:
        v_lex = new type_of_lex(*(type_of_lex *)l.v_lex);
        break;
    case L_FUNC: //v_lex is of (string *) type - name of function
    case L_VAR:
        v_lex = new string(*(string *)l.v_lex);
        break;
    case L_CONST:
        v_lex = new Type(*(Type *)l.v_lex);
        break;
    case L_VEC:
        v_lex = new VecType(*(VecType *)l.v_lex);
        break;
    case L_OFST:
        v_lex = new int(*(int *)l.v_lex);
        break;
    default:
        throw "Error";
    }
    return *this;
}
type_of_lex Lex::sym() const {
    return *(type_of_lex *)v_lex; 
}
string & Lex::var() const {
    return *(string *)v_lex;
}
Type & Lex::type() const {
    return *(Type *)v_lex;
}
int Lex::ofst() const {
    return *(int *)v_lex;
}
VecType Lex::vec() const {
    return *(VecType *)v_lex;
}