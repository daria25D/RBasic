#include <iostream>
//#include <stack>
#include "./interpreter.h"
#include "./spars.h"

using namespace std;

map <string, Type> Interpreter::v_table;

vector <pair <Lex, Lex>> Interpreter::get_params(){
    int count = stck.top().ofst();
    stck.pop();
    vector <pair <Lex, Lex>> result(count);
    for (int i = 0; i < count; i++) {
        result[count - i - 1] = get_param();
    }
    return result;
}

pair <Lex, Lex> Interpreter::get_param() {
    pair <Lex, Lex> result;
    if(stck.top().is_l(L_NULL)) {
        result.first = stck.top();
        stck.pop();
        result.second = stck.top();
        stck.pop();
        return result;
    } else {
        result.second = stck.top();
        stck.pop();
        result.first = stck.top();
        stck.pop();
        return result;
    }
}

void  Interpreter::c() {
    Type result;
    vector <pair <Lex, Lex>> params = get_params();
    basic_t type = T_NULL;
    for (int i = 0; i < params.size(); i++) {
        if (!params[i].first.is_l(L_NULL)) {
            throw "No such variable in c()";
        }
        if (type == T_NULL) {
            type = get_type(params[i].second).type;
        }
        if (get_type(params[i].second).is(T_CHAR) && type != T_FUNC) {
            type = T_CHAR;
            for (int i = 0; i < result.size(); i++) {
                    result.elem(i) = result.elem(i).char_cast();
            }   
        }
        switch (type) {
        case T_NUM:
            result.elem(i) = get_type(params[i].second).vec[0].num_cast();
            break;
        case T_BOOL:
            result.elem(i) = get_type(params[i].second).vec[0].bool_cast();
            break;
        case T_CHAR:
            result.elem(i) = get_type(params[i].second).vec[0].char_cast();
            break;
        case T_FUNC:
            if(!get_type(params[i].second).is(T_FUNC)) {
                throw "Cannot cast to function";
            }
        default:
            result.elem(i) = get_type(params[i].second).vec[0];
            break;
        }
    }
    result.type = type;
    stck.push(result);
}

void Interpreter::length() {
    vector <pair <Lex, Lex>> params = get_params();
    if (params.size() != 1) {
        throw "Too many arguments in length()";
    }
    if (!params[0].first.is_l(L_NULL)) {
        throw "No such field in length()";
    }
    Type result((double)get_type(params[0].second).size());
    stck.push(result);
}

void Interpreter::mode() {
    vector <pair <Lex, Lex>> params = get_params();
    if (params.size() != 1) {
        throw "Too many arguments in mode()";
    }
    if (!params[0].first.is_l(L_NULL)) {
        throw "No such field in mode()";
    }
    
    switch(get_type(params[0].second).type) {
    case T_NULL:         
        stck.push(Type(string("NULL")));        
        break;
    case T_BOOL:      
        stck.push(Type(string("bool")));    
        break;
    case T_CHAR:    
        stck.push(Type(string("character")));   
        break;
    case T_NUM:      
        stck.push(Type(string("numeral")));     
        break;
    case T_FUNC:   
        stck.push(Type(string("functional")));  
        break;
    default :               
        break;
    }       
}

Type & Interpreter::get_str_type(const string &s) {
    if (is_func) {
        auto i = local_table.find(s);
        if (i != local_table.end()) {
            return i->second;
        }
    }
    auto i = global_table.find(s);
    if (i != global_table.end()) {
        return i->second;
    }
    throw "No such variable";
}

Type & Interpreter::get_var_type(const Lex &l) {
    return get_str_type(l.var());
}
bool  Interpreter::is_var_vec(const Lex &l) {
    return (vec_table.find(l.var()) != vec_table.end()) ? true : false;
}
VecType & Interpreter::get_var_vec(const Lex & l) {
    auto i = vec_table.find(l.var());
    if (i != vec_table.end()) {
        return i->second;
    } else {
        throw "No such vector";
    }
}
Type Interpreter::get_type(const Lex &l) {
    if (l.is_l(L_VAR)) {
        if (is_var_vec(l)) {
            return get_var_vec(l);
        } else {
            return get_var_type(l); 
        }
    } else if (l.is_l(L_CONST)) {
        return l.type();
    } else if (l.is_l(L_VEC)) {
        return Type(l.vec());
    } else {
        throw "Error";
    }
}

void Interpreter::set_type (const string &variable, const Type &type)
{
    if (vec_table.find(variable) != vec_table.end()) {
        vec_table[variable] = type;
        return;
    }
    if (is_func) {
        local_table[variable] = type;
    } else {
        global_table[variable] = type;
    }
}

void Interpreter::function_call ()
{
    Lex function = stck.top();
    stck.pop(); 
    if (function.var() == "c")      { c();      return; }
    if (function.var() == "length") { length(); return; }
    if (function.var() == "mode")   { mode();   return; }
}

Lex Interpreter::start()
{
    int i = 0;
    int size = poliz.size();
    while (i < size) {
        if (!poliz[i].is_l(L_SYM)) {
            stck.push(poliz[i]);
            ++i;
        } else if (poliz[i].is_s(LEX_LSQBRACKET)) {
            Type t = get_type(stck.top());
            stck.pop();
            VecType p = get_var_type(stck.top())[t];
            stck.pop();
            stck.push(p);
            ++i;
        } else if (poliz[i].is_s(LEX_IN)) {
            Type t = get_type(stck.top());
            stck.pop();


            if (stck.top().is_l(L_VAR)) {
                set_type(stck.top().var(), t);
            } else if (stck.top().is_l(L_VEC)) {
                auto p = stck.top().vec();
                stck.pop();
                stck.push(p = t);
            } else {
                throw "Wrong assignment";
            }
                
            if (poliz[i + 1].is_s(LEX_SEMICOLON)) {
                i += 2;
                stck.pop();
            } else {
                ++i;
            }
                
        } else if (poliz[i].is_s(LEX_SEMICOLON)) {
            if (!stck.empty()) {
                cout << get_type(stck.top()).to_string() << endl;
                stck.pop();
            }
            ++i;
        } else if (poliz[i].is_s(LEX_FUNC)) {
            function_call();
            ++i;
        } else if(poliz[i].is_s(LEX_NOT)) {
            Type t = get_type(stck.top());
            stck.pop();
            stck.push(!t);
            ++i;
        } else if(poliz[i].is_s(LEX_BREAK)) {
            return Lex();
        } else {
            Type t1 = get_type(stck.top());
            stck.pop();
            Type t2 = get_type(stck.top());
            stck.pop();
            stck.push(Lex(operation(t2, t1, poliz[i].sym())));
            ++i;
        }
    }
    
    if (!is_for && is_func && stck.empty())    throw "No return value";
    if ((is_for || !is_func) && !stck.empty()) throw "Has return value?";
    if (!stck.empty()) return get_type(stck.top());
    return Lex();
}

void Interpreter::make_line(const Poliz &poliz)
{

    auto tmp = Interpreter::v_table;
    map <string, Type> tmp2;
    Interpreter inter(tmp, tmp2);
    inter.poliz   = poliz;
    inter.is_func = false;
    inter.is_for  = false;
    inter.start();
    Interpreter::v_table = inter.global_table;
}