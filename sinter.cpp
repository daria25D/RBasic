#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include "./spars.h"
#include "./sinter.h"

using namespace std;

//map<string, Type> Interpreter::v_table;

vector <Lex> Interpreter::get_args(int args) {
    vector<Lex> v;
    for (int i = 0; i < args; i++) {
        v.insert(v.begin(), st.top());
        st.pop();
    }
    return v;
}

void Interpreter::add_to_table(const string & var, const Type & t) {
    if (vec_table.find(var) != vec_table.end()) {
        vec_table[var] = t;
    } else {
        global_table[var] = t;
    }
}

Type Interpreter::get_type(const Lex & lex) {
    if (lex.is_l(L_VAR)) {
        if (is_var_vec(lex)) {
            return get_vec_from_table(lex);
        } else {
            return get_var_from_table(lex);
        }
    } else if (lex.is_l(L_CONST)) {
        return lex.type();
    } else if (lex.is_l(L_VEC)) {
        return Type(lex.vec());
    } 
    return Type();
}

bool Interpreter::is_var_vec(const Lex & lex) {
    if (vec_table.find(lex.var()) != vec_table.end()) {
        return true;
    } else {
        return false;
    }
}

Type & Interpreter::get_var_from_table(const Lex & lex) {
    string s = lex.var();
    auto i = global_table.find(s);
    if (i != global_table.end()) {
        return i->second;
    } else {
        throw "Variable was not defined";
    }
}

VecType & Interpreter::get_vec_from_table(const Lex & lex) {
    auto i = vec_table.find(lex.var());
    if (i != vec_table.end()) {
        return i->second;
    }
    throw "No such vector";
}

int Interpreter::frepeat(int i) {
    int j = i + 1;
    if (poliz[j].is_s(LEX_LBRACE)) {
        j++;
    }
    while (!poliz[j].is_s(LEX_BREAK) && !poliz[j].is_s(LEX_RBRACE)) {
        if (poliz[j].is_l(L_OFST)) {
            function(poliz[i], poliz[j + 1]);
            j += 2;
        } else if (!poliz[j].is_l(L_SYM)) {
            st.push(poliz[j]);
            j++;
        } else if (poliz[j].is_s(LEX_RSQBRACKET)) {
            Type t = get_type(st.top());
            st.pop();
            VecType v = VecType(get_var_from_table(st.top())[t]);
            v = Type(v);
            st.pop();
            st.push(v);
            j++;
        } else if (poliz[j].is_s(LEX_IN)) {
            Type t = get_type(st.top());
            st.pop();
            if (st.top().is_l(L_VAR)) {
                add_to_table(st.top().var(), t);
            } else if (st.top().is_l(L_VEC)) {
                VecType v = st.top().vec();
                st.pop();
                st.push(v = t);
            } else {
                throw "Invalid assignment";
            }
        } else if (poliz[j].is_s(LEX_NOT)) {
            Type t = get_type(st.top());
            st.pop();
            st.push(!t);
            j++;
        } else if (poliz[j].is_s(LEX_LSQBRACKET) || poliz[j].is_s(LEX_LBRACE)) {
            j++;
        } else {
            Type t1 = get_type(st.top());
            st.pop();
            Type t2 = get_type(st.top());
            st.pop();
            st.push(operation(t2, t1, poliz[j].sym()));
            j++;
        }
    }
    return j;
}

void Interpreter::c(int args) {
    vector<Lex> v = get_args(args);
    Type res;
    basic_t typ = T_NULL;
    for (int i = 0; i < args; i++) {
        if (typ == T_NULL) {
            typ = get_type(v[i]).type;
        }
        if (get_type(v[i]).is(T_CHAR)) {
            typ = T_CHAR;
            for (int j = 0; j < res.size(); j++) {
                res.elem(i) = res.elem(i).char_cast();
            }
        }
        if (typ != get_type(v[i]).type) {
            throw "Different types in vector";
        }
        if (v[i].is_l(L_FUNC)) {
            throw "Cannot cast to a function";
        }
        switch (typ) {
        case T_NUM:
            res.elem(i) = get_type(v[i]).elem(0).num_cast();
            break;
        case T_CHAR:
            res.elem(i) = get_type(v[i]).elem(0).char_cast();
            break;
        case T_BOOL:
            res.elem(i) = get_type(v[i]).elem(0).bool_cast();
            break;
        default:
            res.elem(i) = get_type(v[i]).elem(i);
        }
    }
    res.type = typ;
    st.push(res);
}

void Interpreter::mode(int args) {
    if (args != 1) {
        throw "Expected 1 argument in function mode()";
    }
    vector<Lex> v = get_args(args);
    if (v[0].is_l(L_NULL)) {
        throw "No suitable arguments found";
    }
    switch (get_type(v[0]).type) {
    case T_NULL:
        st.push(Type(string("NULL")));
        break;
    case T_BOOL:
        st.push(Type(string("BOOL")));
        break;
    case T_NUM:
        st.push(Type(string("NUMERIC")));
        break;
    case T_CHAR:
        st.push(Type(string("CHARACTER")));
        break;
    default:
        break;
    }
    if (v[0].is_l(L_FUNC)) {
        st.push(Type(string("FUNCTION")));
    }
    //
}

void Interpreter::length(int args) {
    if (args != 1) {
        throw "Expected 1 argument in function length()";
    }
    vector<Lex> v = get_args(args);
    if (v[0].is_l(L_NULL)) {
        throw "No suitable arguments found";
    }
    Type res((double)get_type(v[0]).size());
    st.push(res);
    //
}

void Interpreter::function(const Lex & offset, const Lex & funcname) {
    if (!offset.is_l(L_OFST) || !funcname.is_l(L_FUNC)) {
        throw "Invalid line: expected function, none found";
    }
    int i = offset.ofst();
    string name = funcname.var();
    if (name == "c") {
        Interpreter::c(i);
    } else if (name == "mode") {
        Interpreter::mode(i);
    } else if (name == "length") {
        Interpreter::length(i);
    } else {
        throw "Unexpected function name";
    }
}

Lex Interpreter::start() {
    int i = 0;
    int size = poliz.size();
    while (i < size) {
        if (poliz[i].is_l(L_OFST)) {
            function(poliz[i], poliz[i + 1]);
            i += 2;
        } else if (!poliz[i].is_l(L_SYM)) {
            st.push(poliz[i]);
            i++;
        } else if (poliz[i].is_s(LEX_RSQBRACKET)) {
            Type t = get_type(st.top());
            st.pop();
            VecType v = VecType(get_var_from_table(st.top())[t]);
            v = Type(v);
            st.pop();
            st.push(v);
            i++;
        } else if (poliz[i].is_s(LEX_IN)) {
            Type t = get_type(st.top());
            st.pop();
            if (st.top().is_l(L_VAR)) {
                add_to_table(st.top().var(), t);
            } else if (st.top().is_l(L_VEC)) {
                VecType v = st.top().vec();
                st.pop();
                st.push(v = t);
            } else {
                throw "Invalid assignment";
            }
            if (poliz[i + 1].is_s(LEX_SEMICOLON)) {
                i += 2;
                st.pop();
            } else {
                i++;
            }
        } else if (poliz[i].is_s(LEX_SEMICOLON)) {
            if (!st.empty()) {
                cout << get_type(st.top()).Type::to_string() << endl;
                st.pop();
            }
            i++;
        } else if (poliz[i].is_s(LEX_MORE) && st.size() == 1) {
            if (st.top().is_l(L_VAR)){
                auto j = global_table.find(st.top().var());
                if (j != global_table.end()) {
                    cout << get_type(j->second).to_string() << endl;
                    st.pop();
                } else {
                    throw "Variable was not defined";
                }
                i++;
            } else {
                i++;
            }
        } else if (poliz[i].is_s(LEX_NOT)) {
            Type t = get_type(st.top());
            st.pop();
            st.push(!t);
            i++;
        } else if (poliz[i].is_s(LEX_REPEAT)) {
            i = frepeat(i);
            if (poliz[i].is_s(LEX_RBRACE) || poliz[i].is_s(LEX_BREAK)) {
                i++;
            }
        } else if (poliz[i].is_s(LEX_BREAK)) {
            return Lex();
        } else if (poliz[i].is_s(LEX_LSQBRACKET) || poliz[i].is_s(LEX_LBRACE)) {
            i++;
        } else {
            Type t1 = get_type(st.top());
            st.pop();
            Type t2 = get_type(st.top());
            st.pop();
            st.push(operation(t2, t1, poliz[i].sym()));
            i++;
        }
    }
    if (!st.empty()) {
        return get_type(st.top());
    }
    return Lex();
}

void Interpreter::interpret_line(const Poliz & p) {
    poliz = p;
    is_repeat = false;
    try {
        start();
    }
    catch(const char * s) {
        throw s;
    } 
    catch(...) {
        throw "Error";
    }
}


