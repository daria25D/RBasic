#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "./lexer.h"
#include "./types.h"

using namespace std;

double * BasicType::num_val() const {
    return (double *)value;
}
bool * BasicType::bool_val() const {
    return (bool *) value;
}
string * BasicType::char_val() const {
    return (string *)value;
}
double BasicType::get_num_val() const {
    return * num_val();
}
bool BasicType::get_bool_val() const{
    return * bool_val();
}
string BasicType::get_char_val() const {
    return *char_val();
}
BasicType::BasicType() {
    type = T_NULL;
}
BasicType::BasicType(double v) {
    type = T_NUM;
    value = new double(v);
}
BasicType::BasicType(bool b) {
    type = T_BOOL;
    value = new bool(b); 
}
BasicType::BasicType(const string & s) {
    type = T_CHAR;
    value = new string(s); 
}
BasicType::BasicType(const BasicType & bt) {
    type = bt.type;
    switch (type) {
    case T_NUM:
        value = new double(bt.get_num_val());
        break;
    case T_BOOL:
        value = new bool(bt.get_bool_val());
        break;
    case T_CHAR:
        value = new string(bt.get_char_val());
        break;
    default:
        value = nullptr;
    }
}
BasicType::~BasicType() {
    switch(type) {
    case T_NUM:
        delete num_val();
        break;
    case T_BOOL:
        delete bool_val();
        break;
    case T_CHAR:
        delete char_val();
    default:
        break;
    }
}

BasicType BasicType::num_cast() const {
    switch(type) {
    case T_NULL:
    case T_NUM:
        return *this;
        break;
    case T_BOOL:
        return double(get_bool_val() ? 1 : 0);
        break;
    default:
        throw "Cannot cast types";
    }
}
BasicType BasicType::bool_cast() const {
    switch(type) {
    case T_NULL:
        return BasicType();
    case T_BOOL:
        return *this;
    case T_NUM:
        return bool(get_num_val() != 0);
    default:
        throw "Cannot cast types";
    }
}
BasicType BasicType::char_cast() const {
    ostringstream oss;
    switch (type) {
    case T_NUM:
        oss << get_num_val();
        return oss.str();
    case T_BOOL:
        return get_bool_val() ? "TRUE" : "FALSE";
    case T_CHAR:
        return get_char_val();
    default:
        return "NULL";
    }
}
basic_t BasicType::get_type() const {
    return type;
}
string BasicType::to_string() const {
    ostringstream oss;
    switch (type) {
    case T_NUM:
        oss << get_num_val();
        return oss.str();
    case T_BOOL:
        return (get_bool_val()) ? "TRUE" : "FALSE";
    case T_CHAR:
        return get_char_val();
    default:
        return "NULL";
    }
}
bool BasicType::is_null() const {
    return type == T_NULL;
}
bool BasicType::is_num() const {
    return type == T_NUM;
}
bool BasicType::is_bool() const {
    return type == T_BOOL;
}
bool BasicType::is_char() const {
    return type == T_CHAR;
}
BasicType & BasicType::operator= (const BasicType & bt) {
    this->BasicType::~BasicType();
    type = bt.type;
    switch (type) {
    case T_NUM:
        value = new double(bt.get_num_val());
        break;
    case T_BOOL:
        value = new bool(bt.get_bool_val());
        break;
    case T_CHAR:
        value = new string(bt.get_char_val());
        break;
    default:
        value = nullptr;
    }
    return *this;
}
BasicType BasicType::operator+ (const BasicType & bt) const {
    switch(type) {
        case T_NULL:
            if (bt.is_null() || bt.is_num()) {
                return BasicType(); //NULL
            }
            break;
        case T_BOOL:
            if (bt.is_num()) {
                return (this->num_cast()).get_num_val() + bt.get_num_val();
            } else if (bt.is_bool()) {
                return (this->num_cast()).get_num_val() + bt.num_cast().get_num_val();
            }
            break;
        case T_NUM:
            if (bt.is_num()) {
                return this->get_num_val() + bt.get_num_val();
            } else if (bt.is_bool() || bt.is_char()) {
                return this->get_num_val() + (bt.num_cast()).get_num_val();
            } else if (bt.is_null()) {
                return BasicType();
            }
            break;
        case T_CHAR:
            if (bt.is_char()) {
                return this->get_char_val() + bt.get_char_val();
            } else if (bt.is_num()) {
                return this->get_char_val() + (bt.char_cast()).get_char_val();
            }
            break;
        default:
            throw "Wrong types operands in '+', cannot assign";
            break;
    }
    return BasicType();
}
BasicType BasicType::operator- (const BasicType & bt) const {
    switch(type) {
    case T_NULL:
        if (bt.is_null() || bt.is_num()) {
            return BasicType();
        }
        break;
    case T_BOOL:
        if (bt.is_num()) {
            return (this->num_cast()).get_num_val() - bt.get_num_val();
        }else if (bt.is_bool()) {
            return (this->num_cast()).get_num_val() - bt.num_cast().get_num_val();
        }
        break;
    case T_NUM:
        if (bt.is_num()) {
            return this->get_num_val() - bt.get_num_val();
        } else if (bt.is_bool()) {
            return this->get_num_val() - (bt.num_cast()).get_num_val();
        } else if (bt.is_null()) {
            return BasicType();
        }
        break;
    default:
        throw "Wrong types operands in '-', cannot assign";
        break;
    }
    return BasicType();
}
BasicType BasicType::operator* (const BasicType & bt) const {
    switch(type) {
    case T_NULL:
        if (bt.is_null() || bt.is_num()) {
            return BasicType();
        }
        break;
    case T_BOOL:
        if (bt.is_num()) {
            return (this->num_cast()).get_num_val()  * bt.get_num_val();
        } else if (bt.is_bool()) {
            return (this->num_cast()).get_num_val() * bt.num_cast().get_num_val();
        }
        break;
    case T_NUM:
        if (bt.is_num()) {
            return this->get_num_val() * bt.get_num_val();
        } else if (bt.is_bool()) {
            return this->get_num_val() * (bt.num_cast()).get_num_val();
        } else if (bt.is_null()) {
            return BasicType();
        }
        break;
    default:
        throw "Wrong types operands in '*', cannot assign";
        break;
    }
    return BasicType();    
}
BasicType BasicType::operator/ (const BasicType & bt) const {
    switch(type) {
    case T_NULL:
        if (bt.is_null() || bt.is_num()) {
            return BasicType();
        }
        break;
    case T_BOOL:
        if (bt.is_num()) {
            return (this->num_cast()).get_num_val() / bt.get_num_val();
        } else if (bt.is_bool()) {
            if (bt.get_bool_val()) {
            return (this->num_cast()).get_num_val() / bt.num_cast().get_num_val();
            } else {
                throw "Division by zero";
            }
        }
        break;
    case T_NUM:
        if (bt.is_null()) {
            return BasicType();
        } else if (bt.is_num()) {
            return this->get_num_val() / bt.get_num_val();
        } else if (bt.is_bool()) {
            return this->get_num_val() / (bt.num_cast()).get_num_val();
        }
        break;
    default:
        throw "Wrong types operands in '/', cannot assign";
        break;
    }
    return BasicType();
}
BasicType BasicType::operator< (const BasicType & bt) const {
    if (this->is_null() && bt.is_null()) {
        return BasicType();
    } else if (this->is_num() && bt.is_num()) {
        return this->get_num_val() < bt.get_num_val();
    } else if (this->is_bool() && bt.is_num()) {
        return this->get_bool_val() < (bt.bool_cast()).get_bool_val();
    } else if (this->is_num() && bt.is_bool()) {
        return (this->bool_cast()).get_bool_val() < bt.get_bool_val();
    } else if (this->is_char() && bt.is_char()) {
        return this->get_char_val() < bt.get_char_val();
    } else if (this->is_null() || bt.is_null()) {
        return BasicType();
    } else if (this->is_char() || bt.is_char()) {
        try {
            return (this->char_cast()).get_char_val() < (bt.char_cast()).get_char_val();
        }
        catch(...) {
            throw "Wrong types operands in '<', cannot assign";
        }
    } else {
        throw "Wrong types operands in '<', cannot assign";
    }
}
BasicType BasicType::operator> (const BasicType & bt) const {
    try {
        return ! operator<=(bt);
    }
    catch(...) {
        throw "Wrong types operands in '>=', cannot assign";
    }
}
BasicType BasicType::operator<= (const BasicType & bt) const {
        if (this->is_null() && bt.is_null()) {
        return BasicType();
    } else if (this->is_num() && bt.is_num()) {
        return this->get_num_val() <= bt.get_num_val();
    } else if (this->is_char() && bt.is_char()) {
        return this->get_char_val() <= bt.get_char_val();
    } else if (this->is_char() || bt.is_char()) {
        try {
            return (this->char_cast()).get_char_val() <= (bt.char_cast()).get_char_val();
        }
        catch(...) {
            throw "Wrong types operands in '<=', cannot assign";
        }
    } else {
        throw "Wrong types operands in '<=', cannot assign";
    }
}
BasicType BasicType::operator>= (const BasicType & bt) const {
    try {
        return ! operator<(bt);
    }
    catch(...) {
        throw "Wrong types operands in '>=', cannot assign";
    }
}
BasicType BasicType::operator== (const BasicType & bt) const {
    if (this->is_null() && bt.is_null()) {
        return BasicType();
    } else if (this->is_num() && bt.is_num()) {
        return this->get_num_val() == bt.get_num_val();
    } else if (this->is_bool() && bt.is_num()) {
        return this->get_bool_val() == (bt.bool_cast()).get_bool_val();
    } else if (this->is_num() && bt.is_bool()) {
        return (this->bool_cast()).get_bool_val() == bt.get_bool_val();
    } else if (this->is_char() && bt.is_char()) {
        return this->get_char_val() == bt.get_char_val();
    } else if (this->is_null() || bt.is_null()) {
        return false;
    } else if (this->is_char() || bt.is_char()) {
        try {
            return (this->char_cast()).get_char_val() == (bt.char_cast()).get_char_val();
        }
        catch(...) {
            throw "Wrong types operands in '==', cannot assign";
        }
    } else {
        throw "Wrong types operands in '==', cannot assign";
    }
}
BasicType BasicType::operator!= (const BasicType & bt) const {
    return !operator==(bt);
}
BasicType BasicType::operator& (const BasicType & bt) const {
    switch (type) {
    case T_NULL:
        if (bt.is_bool()) {
            return BasicType();
        }
        break;
    case T_BOOL:
        if (bt.is_bool()) {
            return bool(this->get_bool_val() & bt.get_bool_val());
        } else if (bt.is_null()) {
            return BasicType();
        } else if (bt.is_num()) {
            return bool(this->get_bool_val() & (bt.bool_cast()).get_bool_val());
        }
        break;
    case T_NUM:
        if (bt.is_bool()) {
            return bool((this->bool_cast()).get_bool_val() & bt.get_bool_val());
        }
        break;
    default:
        throw "Wrong types oprands in '&', cannot assign";
        break;
    }
    return BasicType();
}
BasicType BasicType::operator| (const BasicType & bt) const {
    switch (type) {
    case T_NULL:
        if (bt.is_bool()) {
            return BasicType();
        }
        break;
    case T_BOOL:
        if (bt.is_bool()) {
            return bool(this->get_bool_val() | bt.get_bool_val());
        } else if (bt.is_null()) {
            return BasicType();
        } else if (bt.is_num()) {
            return bool(this->get_bool_val() | (bt.bool_cast()).get_bool_val());
        }
        break;
    case T_NUM:
        if (bt.is_bool()) {
            return bool((this->bool_cast()).get_bool_val() | bt.get_bool_val());
        }
        break;
    default:
        throw "Wrong types oprands in '|', cannot assign";
        break;
    }
    return BasicType();
}
BasicType BasicType::operator! () const {
    if (is_bool()) {
        return bool(!(this->get_bool_val()));
    } else if (is_null()) {
        return BasicType();
    } else if (is_num()) {
        return bool(!((this->bool_cast()).get_bool_val()));
    } else {
        throw "Wrong type operand in '!', cannot assign";
    }
}


Type::Type() {
    type = T_NULL;
}
Type::Type(const BasicType & bt) {
    type = bt.get_type();
    vec.push_back(bt);
}
Type::Type(const vector<BasicType> & vb) {
    vector<BasicType>::const_iterator it = vb.begin();
    if (it != vb.end()) {
        type = it->get_type();
    } else {
        type = T_NULL;
    }
    for (; it != vb.end(); it++) {
        if (type != it->get_type()) {
            throw "Incorrect vector, has different types arguments";
        }
    }
    vec = vb;
}
Type::Type(const Type & t) {
    type = t.type;
    vec = t.vec;
}
Type::Type(const VecType & v) {
    type = v.val->type;
    for (auto i = v.idx.begin(); i != v.idx.end(); ++i) {
        vec.push_back(v.val->elem(*i));
}
}
basic_t Type:: get_type() const {
    return type;
}
void Type::new_size(int size) {
    for (int i = vec.size(); i < size; i++) {
        vec.push_back(BasicType());
    }
    if (size <= 0) {
        throw "Cannot make size less then actual";
    }
}
int Type::size() const {
    return vec.size();
}
bool Type::is(basic_t t) const {
    return t == type;
}
BasicType & Type::elem(int idx) { //if index > vec.size(), vec will be build up to a new size
    new_size(idx + 1);
    return vec[idx];
}
Type Type::new_vec(int new_size_v) const {
    Type new_v = *this;
    for (int i = size(); i < new_size_v; i++) {
        new_v.vec.push_back(new_v.elem(i % size()));
    }
    return new_v;
}
string Type::to_string () const
{
    string result;
    if (vec.empty()) {
        result = "NULL(0)";
        return result;
    }
    for (int i = 0; i < vec.size(); i++) {
        result += vec[i].to_string() + ' ';
    }

    return result;
}
Type Type::operator! () const {
    Type res = *this;
    for (int i = 0; i < size(); i++) {
        res.elem(i) = !res.elem(i);
    }
    return res;
}
VecType Type::operator[] (const Type & t) {
    if (t.is(T_NUM)) {
        VecType res;
        res.val = this;
        for (int i = 0; i < t.size(); i++) {
            if (!t.vec[i].is_null()) {
                new_size(t.vec[i].get_num_val());
                res.idx.push_back(t.vec[i].get_num_val() - 1);
            }
        }
        return res;
    } else if (t.is(T_BOOL)) {
        VecType res;
        res.val = this;
        Type tmp = t.new_vec(size());
        for (int i = 0; i < t.size(); i++) {
            if (tmp.vec[i].get_bool_val()) {
                new_size(i + 1);
                res.idx.push_back(i);
            }
        }
        return res;
    }
    throw "Wrong index";
}
Type colon(const Type & x, const Type & y) { //x:y - вектор с шагом 1
    if (x.size() == 0 || y.size() == 0 ||
        x.get_type() == T_NULL || y.get_type() == T_NULL) {
        throw "Assignment to NULL vectors";
    } else if (x.get_type() == T_CHAR || y.get_type() == T_CHAR) {
        throw "Wrong types of vectors, cannot cast to char";
    }
    Type res;
    res.type = T_NUM;
    BasicType a = x.vec[0], b = y.vec[0];
    if ((a < b).get_bool_val()) {
        for (BasicType bt = a; (bt <= b).get_bool_val(); bt = bt + 1.) {
            res.vec.push_back(bt);
        }
    } else {
        for (BasicType bt = a; (bt >= b).get_bool_val(); bt = bt - 1.) {
            res.vec.push_back(bt);
        }
    }
    return res;
}
//operations with vectors: +, -, *, /, &, |, ==, !=, <, >, <=, >=, :
Type operation(const Type & x, const Type & y, type_of_lex op) {
    Type res;
    int msize = (x.size() > y.size()) ? x.size() : y.size();
    Type a = x.new_vec(msize), b = y.new_vec(msize);
    for (int i = 0; i < msize; i++) {
        switch (op) {
        case LEX_PLUS:
            res.elem(i) = a.elem(i) + b.elem(i);
            break;
        case LEX_MINUS:
            res.elem(i) = a.elem(i) - b.elem(i);
            break;
        case LEX_MULT:
            res.elem(i) = a.elem(i) * b.elem(i);
            break;
        case LEX_DIV:
            res.elem(i) = a.elem(i) / b.elem(i);
            break;
        case LEX_AND:
            res.elem(i) = a.elem(i) & b.elem(i);
            break;
        case LEX_OR:
            res.elem(i) = a.elem(i) | b.elem(i);
            break;
        case LEX_EQUAL:
            res.elem(i) = a.elem(i) == b.elem(i);
            break;
        case LEX_NOT_EQUAL:
            res.elem(i) = a.elem(i) != b.elem(i);
            break;
        case LEX_LESS:
            res.elem(i) = a.elem(i) < b.elem(i);
            break;
        case LEX_MORE:
            res.elem(i) = a.elem(i) > b.elem(i);
            break;
        case LEX_LESS_OR_EQUAL:
            res.elem(i) = a.elem(i) <= b.elem(i);
            break;
        case LEX_MORE_OR_EQUAL:
            res.elem(i) = a.elem(i) >= b.elem(i);
            break;
        case LEX_COLON:
            return colon(x, y);
        default:
            break;
        }
    }
    vector<BasicType>::const_iterator it = res.vec.begin();
    if (it != res.vec.end()) {
        res.type = it->get_type();
    } else {
        res.type = T_NULL;
    }
    return res;
}

VecType & VecType::operator= (const Type & t) {
    if (t.size() > idx.size()) {
        throw "Cannot assign two vectors";
    }
    Type res = t.new_vec(idx.size());
    if (t.type == T_CHAR && val->type != T_FUNC) {
        val->type = T_CHAR;
        for (int i = 0; i < val->size(); i++) {
            val->elem(i) = val->elem(i).char_cast();
        }
    }
    for (int i = 0; i < idx.size(); i++) {
        switch (val->type) {
        case T_NUM:
            val->vec[idx[i]] = t.vec[i].num_cast();
            break;
        case T_BOOL:
            val->vec[idx[i]] = t.vec[i].bool_cast();
            break;
        case T_CHAR:
            val->vec[idx[i]] = t.vec[i].char_cast();
            break;
        case T_FUNC:
            if (!(t.type == T_FUNC || t.type == T_NULL)) {
                throw "Wrong assignment";
            }
            val->vec[idx[i]] = t.vec[i];
            break;
        default:
            val->type = t.type;
            val->vec[idx[i]] = t.vec[i];
        }
    }
    return *this;
}
VecType & VecType::operator= (const VecType & v) {
    val = v.val;
    idx = v.idx;
    return *this;
}
VecType::VecType(const VecType & v) {
    val = v.val;
    idx = v.idx;
}