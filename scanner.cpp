#include <iostream>
#include <cstring>
#include "./scanner.h"
#include "./lexer.h"

using namespace std;



//table of key words
const char * Scanner::TW[] = 
{
    "",         //0
    "TRUE",     //1
    "FALSE",    //2
    "NULL",     //3
    "if",       //4
    "else",     //5
    "repeat",   //6
    "break",    //7
    "matrix",   //8
    "vector",   //9
    NULL
};

//table of types of key words from TW
type_of_lex Scanner::words[] = 
{
    LEX_NULL,
    LEX_TRUE,
    LEX_FALSE,
    LEX_NULL,
    LEX_IF,
    LEX_ELSE,
    LEX_REPEAT,
    LEX_BREAK,
    LEX_MATRIX,
    LEX_VECTOR,
    LEX_NULL
};

//table of symbols
const char * Scanner::TD[] = 
{
    "",         //0
    ",",        //1
    ":",        //2
    ";",        //3
    "&",        //4
    "|",        //5
    "!",        //6
    "(",        //7
    ")",        //8
    "{",        //9
    "}",        //10
    "[",        //11
    "]",        //12
    "+",        //13
    "-",        //14
    "*",        //15
    "/",        //16
    "=",        //17
    ">",        //18        
    "<",        //19
    ">=",       //20
    "<=",       //21
    "!=",       //22
    "==",       //23
    "<-",       //24
    ".",        //25 
    "\n",       //26
    "\""        //27
    "#",        //28
    NULL
};

//tables of typef of symbols from TD
type_of_lex Scanner::dlms[] = 
{
    LEX_NULL,
    LEX_COMMA,
    LEX_COLON,
    LEX_SEMICOLON,
    LEX_AND,
    LEX_OR,
    LEX_NOT,
    LEX_LPARENTH,
    LEX_RPARENTH,
    LEX_LBRACE,
    LEX_RBRACE,
    LEX_LSQBRACKET,
    LEX_RSQBRACKET,
    LEX_PLUS,
    LEX_MINUS,
    LEX_MULT,
    LEX_DIV,
    LEX_IS,
    LEX_MORE,
    LEX_LESS,
    LEX_MORE_OR_EQUAL,
    LEX_LESS_OR_EQUAL,
    LEX_NOT_EQUAL,
    LEX_EQUAL,
    LEX_IN,
    LEX_DOT,
    LEX_NEWLINE,
    LEX_QUOTES,
    LEX_COMMENT,
    LEX_NULL
};


Scanner::Scanner(): file(nullptr), EOF_reached(0){
    ST = H;
    clear();
    gc();
}
Scanner::Scanner(const char * fileName): EOF_reached(0) {
    file = fopen(fileName, "r");
    ST = H;
    clear();
    gc();
}
void Scanner::clear() {
   buf = "\0";
}
void Scanner::add() {
    buf += c;
}
int Scanner::look(const string & buff, const char ** lst) const {
    int i = 1;
    while (lst[i] != NULL) {
        if (buff == lst[i]) {
            return i;
        }
        i++;
    }
    return 0;
}
void Scanner::gc() {
    if (file) {
        if ((c = fgetc(file)) == EOF) {
            EOF_reached = 1;
        }
    } else {
        if ((c = cin.get()) == EOF) {
            EOF_reached = 1;
            //cout << 1 << endl;
        }
    }
}
void Scanner::new_line() {
    while (!(cin.eof() || c == '\n')) {
        gc();
    }
}
Lex Scanner::get_lex() {
    int j;
    ST = H;
    do {
        switch(ST) {
        case H:
            if (c == EOF || cin.eof() || (EOF_reached == 1)) {
                //cout << 2 << endl;
                return Lex(LEX_EOF);
                
            } else if (c == '\n') {
                gc();
                //cout << 3 << endl;
                //return Lex(LEX_EOF);
                //c == ' ';
                //j = look(buf, TD);
                return Lex(LEX_NEWLINE);  
            } else if (isspace(c)) {
                gc();
                break;
            } else if (isalpha(c) || c == '.') {
                clear();
                add();
                gc();
                ST = IDENT;
                break;
            } else if (isdigit(c)) {
                clear();
                add();
                gc();
                ST = NUMB;
                break;
            } else if (c == '#') {
                clear();
                add();
                gc();
                ST = COMMENT;
                break;
            } else if (c == '\"') {
                clear();
                gc();
                ST = CHAR;
                break;
            } else if (c == '<') { 
                clear();
                add();
                gc();
                ST = LESS;
                break;
            } else if (c == '>') {
                clear();
                add();
                gc();
                ST = MORE;
                break;
            } else if (c == '=') {
                clear();
                add();
                gc();
                ST = EQUAL;
                break;
            }
             else if (c == '!') {
                clear();
                add();
                gc();
                ST = NOT;
                break;
            } else {
                ST = DELIM;
                break;
            }
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c) || c == '.') {
                add();
                gc();
                break;
            } else {
                if (j = look(buf, TW)) {
                    if (words[j] == LEX_TRUE) return Lex(Type(true));
                    if (words[j] == LEX_FALSE) return Lex(Type(false));
                    if (words[j] == LEX_NULL) return Lex(Type(BasicType()));
                    else return Lex(words[j]);
                } else {
                    return Lex(buf);
                }
            }
            break;
        case NUMB:
            if (isdigit(c)) {
                add();
                gc();
                break;
            } else if (c == '.') {
                add();
                gc();
                ST = FLOAT;
                break;
            } else {
                return Lex(Type(stod(buf)));
            }
            break;
        case FLOAT:
            if (isdigit(c)) {
                add();
                gc();
            } else {
                return Lex(Type(stod(buf))); 
            }
            break;
        case COMMENT: 
            if (!c || c == '\n') {
                ST = H;
                break;
            } else {
                gc();
                break;
            }
            break;
        case LESS:
            if (c == '=' || c == '-') { // <=, <-
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j]);
            } else { //<
                j = look(buf, TD);
                return Lex(dlms[j]);
            }
            break;
        case MORE:
            if (c == '=') { //>=
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j]);
            } else { //>
                j = look(buf, TD);
                return Lex(dlms[j]);
            }
            break;
        case EQUAL:
            if (c == '=') {
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j]);
            } else {
                j = look(buf, TD);
                return Lex(dlms[j]);
            }
        case NOT:
            if (c == '=') {
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j]);
            } else {
                j = look(buf, TD);
                return Lex(dlms[j]);
            }
            break;
        case CHAR:
            if (c == '\\') {
                gc();
                ST = SLASH;
                break;
            } else if (c == '\"') {
                gc();
                return Lex(Type(buf));
            } else if (c == '\n' || !c || c == EOF) { 
                new_line();
                throw "Not end of line yet";
            } else {
                add();
                gc();
                break;
            }
            break;
        case SLASH:
            if (c == 'n') {
                buf += '\n';
                gc();
                ST = CHAR;
                break;
            } else if (c == 't') {
                buf += '\t';
                gc();
                ST = CHAR;
                break;
            } else if (c == '"') {
                buf += '"';
                gc();
                ST = CHAR;
                break;
            } else {
                buf += '/';
                ST = CHAR;
                break;
            }
        case DELIM:
            clear();
            add();
            if (j = look(buf,TD)) {
                gc();
                return Lex(dlms[j]);
            } else {
                throw "Unexpected symbol";
            }
            break;
        default:
            break;
        }
    } while (true);
}

// int main()
// {
//     Scanner s;
//     Lex l = s.get_lex();
//     if (l.get_type() == L_VAR) {
//         cout << "VAR: " << l.var() << endl;
//     } else if (l.get_type() == L_SYM) {
//         for (int j = 0; j < 29; j++) {
//             if (Scanner::dlms[j] == l.sym()) {
//                 cout << "SYM: " << Scanner::TD[j] << endl;
//                 break;
//             }
//         }
//     } 
//     return 0;
// }