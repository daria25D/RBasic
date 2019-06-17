#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <cstring>
#include "./lexer.h"

using namespace std;

//states of lexical analyzator
enum state 
{ 
    H, 
    NUMB, 
    FLOAT,
    IDENT, 
    NOT, 
    COMMENT, 
    LESS, 
    MORE, 
    EQUAL,
    DELIM, 
    CHAR, 
    SLASH 
}; 


//class Scanner for implementation of lexical analysis
class Scanner 
{
    state ST;
    int c;
    string buf = string();
    FILE * file;
    int EOF_reached;
    //clear buffer
    void clear();

    //add char-symbol to buffer
    void add();

    //find lexem in TD or TW (see lexer.h)
    int look(const string &, const char **) const;

    //read next char-symbol
    void gc();

    //if reached '\n', get next char-symbol
    void new_line();
public:
    //table of key words
    static const char * TW[];

    //table of types of key words from TW
    static type_of_lex words[];

    //table of symbols
    static const char * TD[];

    //tables of typef of symbols from TD
    static type_of_lex dlms[];
    //constructor
    Scanner();
    explicit Scanner(const char *);
    //lexical analyzator
    Lex get_lex();
    
};

#endif
