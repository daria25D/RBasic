#ifndef TYPES_H_
#define TYPES_H_

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

//types of lexems (symbols) that are used in project
enum type_of_lex 
{
    LEX_NULL,           //0
    LEX_TRUE,           //1
    LEX_FALSE,          //2
    LEX_AND,            //3
    LEX_OR,             //4
    LEX_NOT,            //5
    LEX_LESS,           //6
    LEX_MORE,           //7
    LEX_LESS_OR_EQUAL,  //8
    LEX_MORE_OR_EQUAL,  //9
    LEX_EQUAL,          //10
    LEX_NOT_EQUAL,      //11
    LEX_PLUS,           //12
    LEX_MINUS,          //13
    LEX_MULT,           //14
    LEX_DIV,            //15
    LEX_COMMA,          //16
    LEX_COLON,          //17
    LEX_SEMICOLON,      //18
    LEX_LBRACE,         //19
    LEX_RBRACE,         //20
    LEX_LPARENTH,       //21
    LEX_RPARENTH,       //22
    LEX_LSQBRACKET,     //23
    LEX_RSQBRACKET,     //24
    LEX_IS,             //25
    LEX_IN,             //26
    LEX_NUM,            //27
    LEX_CHAR,           //28
    LEX_ID,             //29
    LEX_FIN,            //30
    LEX_DOT,            //31
    LEX_FUNC,           //32
    LEX_FLOAT,          //33
    LEX_BOOL,           //34
    LEX_IF,             //35
    LEX_ELSE,           //36
    LEX_REPEAT,         //37
    LEX_BREAK,          //38
    LEX_MATRIX,         //39
    LEX_VECTOR,         //40
    LEX_NEWLINE,        //41
    LEX_QUOTES,         //42
    LEX_EOF,            //43
    LEX_COMMENT         //44
};

//basic types that are used in project
enum basic_t 
{
    T_NULL,
    T_NUM,
    T_BOOL,
    T_FUNC,
    T_CHAR
};

//class of basic types implementation
class BasicType 
{
    basic_t type;
    void * value;

    //returns (type_of(value)*)value
    double * num_val() const;
    bool * bool_val() const;
    string * char_val() const;
    
public:
    //constructors (default, from other basic types and copy)
    BasicType();
    BasicType(double);
    BasicType(bool);
    BasicType(const string &);
    BasicType(const BasicType &);

    //cast of types
    BasicType num_cast() const;
    BasicType bool_cast() const;
    BasicType char_cast() const;

    //returns *(type_of(value *))value
    double get_num_val() const;
    bool get_bool_val() const;
    string get_char_val() const;

    //returns type of BasicType variable
    basic_t get_type() const;

    //string representation
    string to_string() const;

    //check of type
    bool is_null() const;
    bool is_num() const;
    bool is_bool() const;
    bool is_char() const;

    //overload of operations with BasicType variables
    BasicType & operator= (const BasicType &);
    BasicType operator+ (const BasicType &) const;
    BasicType operator- (const BasicType &) const;
    BasicType operator* (const BasicType &) const;
    BasicType operator/ (const BasicType &) const;
    BasicType operator< (const BasicType &) const;
    BasicType operator> (const BasicType &) const;
    BasicType operator<= (const BasicType &) const;
    BasicType operator>= (const BasicType &) const;
    BasicType operator== (const BasicType &) const;
    BasicType operator!= (const BasicType &) const;
    BasicType operator& (const BasicType &) const;
    BasicType operator| (const BasicType &) const;
    BasicType operator!() const;

    //destructor
    ~BasicType();
};
class VecType;
//clas vectors handling
class Type 
{
    friend class VecType;
public:
    basic_t type;
    vector<BasicType> vec;
    
    //function for handling operations with vector
    friend Type operation(const Type &, const Type &, type_of_lex);

    //function for vectors assignment - x:y
    friend Type colon(const Type &, const Type &);

    //methods for 
    basic_t get_type() const;

    //resizing vector
    void new_size(int);

    //get size
    int size() const;

    //check type
    bool is(basic_t) const;

    //string representation
    string to_string() const;

    //get element number idx of vector
    BasicType & elem(int);

    //get new vector of size int new_size_v
    Type new_vec(int) const;

    //constructors 
    Type();
    Type(const BasicType &);
    Type(const vector<BasicType> &);
    Type(const Type &);
    Type(const VecType &);

    //=, [] and ! overload
    Type & operator= (const Type &) = default;
    Type operator! () const;
    VecType operator[] (const Type &);
};
//class for indexing vectors
class VecType 
{
    friend class Type;
public:
    Type * val;
    vector<int> idx;
    
    VecType() = default;
    VecType(const VecType &);
    VecType & operator= (const Type &);
    VecType & operator= (const VecType &);
};
#endif