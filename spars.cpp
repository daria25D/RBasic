#include <iostream>
#include <vector>
#include <stack>
#include <cstring>
#include <map>
#include "./scanner.h"
#include "./spars.h"

using namespace std;

//priority of operations
map<type_of_lex, int> Parser::Priority =
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

Poliz::Poliz() {}

Poliz::~Poliz() {
    p.clear();
}

void Poliz::put_lex(const Lex & l) {
    p.push_back(l);
}

void Poliz::put_lex(const Lex & l, int idx) {
    vector<Lex>::iterator it = p.begin() + idx;
    p.insert(it, l);
}

int Poliz::size() const {
    return p.size();
}

Lex & Poliz::operator[] (int idx) {
    return p[idx];
}

void Poliz::print() {

}

Parser::Parser(): is_file(false), f(nullptr), args(0, true) {}

Parser::Parser(const char * filename): Scanner(filename), is_file(true), args(0, true) {
    f = fopen(filename, "r");
}

void Parser::stack_to_poliz_by_priority(type_of_lex t) {
    while (!s.empty() && Priority.at(s.top()) < Priority.at(t)) {
        p.put_lex(s.top());
        s.pop();
    }
}

void Parser::stack_to_poliz_all() {
    while (!s.empty()) {
        p.put_lex(s.top());
        s.pop();
    }
}

void Parser::stack_to_poliz_till_lex(type_of_lex t) {
    bool flag = (s.top() == t) ? true : false;
    while (!s.empty() && s.top() != t) {
        p.put_lex(s.top());
        s.pop();
        if (s.top() == t) flag = true;
    }
    if (!flag) throw "No sought-for symbol in poliz";
}

void Parser::move_to_poliz(type_of_lex t) {
    stack_to_poliz_by_priority(t);
    s.push(t);
}

void Parser::get() {
    lex = Scanner::get_lex();
}

void Parser::Program() {
    if (lex.is_s(LEX_EOF)) {
        return;
    }
    while (lex.is_s(LEX_NEWLINE)) {
        get();
    }
    Start();
    
    while (lex.is_s(LEX_NEWLINE)) {
       get();
    }
    if (lex.is_s(LEX_EOF)) {
        stack_to_poliz_all();
        if (!p[p.size() - 1].is_s(LEX_SEMICOLON) && p.size() > 0) {
            p.put_lex(LEX_SEMICOLON);
        }
        return;
    } else {
        throw "Semantic error";
    }
}

void Parser::Start() {
    Case();
    while (lex.is_s(LEX_SEMICOLON) || lex.is_s(LEX_NEWLINE)) {
        if (lex.is_s(LEX_SEMICOLON)) {
            stack_to_poliz_all();
            if (!p[p.size() - 1].is_s(LEX_SEMICOLON) && p.size() > 0) {
                p.put_lex(LEX_SEMICOLON);
            }
            get();
            if (lex.is_s(LEX_EOF)) {
                return;
            } else {
                Case();
            }
        } else if (lex.is_s(LEX_NEWLINE)) {
            stack_to_poliz_all();
            if (!p[p.size() - 1].is_s(LEX_SEMICOLON) && p.size() > 0) {
                p.put_lex(LEX_SEMICOLON);
            }
            get();
            if (lex.is_s(LEX_EOF)) {
                return;
            } else {
                Case();
            }
        }
    }
}


void Parser::Case() {
     if (lex.is_s(LEX_REPEAT)) {
         p.put_lex(LEX_REPEAT);
        get();
        Repeat();
        return;
    } else if (lex.is_s(LEX_LBRACE)) {
        s.push(lex.sym());
        p.put_lex(lex);
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Block();
        if (!lex.is_s(LEX_RBRACE)) {
            throw "Invalid input: no '}' found";
        } else {
            stack_to_poliz_till_lex(LEX_LBRACE);
            if (s.top() == LEX_LBRACE) s.pop();
            p.put_lex(lex);
            get();
            return;
        }
     } else {
         Expression();
     }
}

void Parser::Expression() {
    Exp1();
    while (lex.is_s(LEX_IN)) {
        s.push(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp1();
    }
}

void Parser::Exp1() {
    Exp2();
    while (lex.is_s(LEX_AND) || lex.is_s(LEX_OR)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp2();
    }
}

void Parser::Exp2() {
    if (lex.is_s(LEX_NOT)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp3();
    } else {
        Exp3();
    }
}

void Parser::Exp3() {
    Exp4();
    if (lex.is_s(LEX_LESS) || lex.is_s(LEX_MORE) ||
        lex.is_s(LEX_LESS_OR_EQUAL) || lex.is_s(LEX_MORE_OR_EQUAL) ||
        lex.is_s(LEX_EQUAL) || lex.is_s(LEX_NOT_EQUAL)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp4();
    }
}

void Parser::Exp4() {
    Exp5();
    while (lex.is_s(LEX_PLUS) || lex.is_s(LEX_MINUS)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp5();
    }
}

void Parser::Exp5() {
    Exp6();
    while (lex.is_s(LEX_MULT) || lex.is_s(LEX_DIV)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp6();
    }
}

void Parser::Exp6() {
    Exp7();
    while (lex.is_s(LEX_COLON)) {
        move_to_poliz(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Exp7();
    }
}

void Parser::Exp7() {
    if (lex.is_s(LEX_LPARENTH)) {
        s.push(lex.sym());
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Case();
        if (!lex.is_s(LEX_RPARENTH)) {
            throw "Invalid input: no ')' found";
        } else {
            stack_to_poliz_till_lex(LEX_LPARENTH);
            if (s.top() == LEX_LPARENTH) s.pop();
            get();
            return;
        }
    } else if (lex.is_l(L_VAR)) {
        Var();
        return;
    } else if (lex.is_l(L_CONST)) {
        Const();
        return;
    }
}

void Parser::FunctCall() {
    ArgList();
}

void Parser::Var() {
    Lex tmp = lex;
    get();
    if (lex.is_s(LEX_LPARENTH)) {
        s.push(LEX_LPARENTH);
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        args = Lex(0, true);//counter for arguments of function
        tmp.to_func();
        FunctCall();
        p.put_lex(args);
        p.put_lex(tmp);
        if (!lex.is_s(LEX_RPARENTH)) {
            throw "Invalid input: no ')' found";
        } else {
            stack_to_poliz_till_lex(LEX_LPARENTH);
            if (s.top() == LEX_LPARENTH) s.pop();
            get();
            return;
        }
    } else if (lex.is_s(LEX_LSQBRACKET)) {
        p.put_lex(tmp);
        s.push(LEX_LSQBRACKET);
        p.put_lex(lex);
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Case();
        if (!lex.is_s(LEX_RSQBRACKET)) {
            throw "Invalid input: no ']' found";
        } else {
            stack_to_poliz_till_lex(LEX_LSQBRACKET);
            if (s.top() == LEX_LSQBRACKET) s.pop();
            p.put_lex(LEX_RSQBRACKET);
            get();
            return;
        }
    } else {
        p.put_lex(tmp);//was variable
        return;
    }
}

void Parser::Const() {
    p.put_lex(lex);
    get();
    return;
}

void Parser::ArgList() {
    if (lex.is_s(LEX_RPARENTH)) {
        return;
    }
    Case();
    args.increase_ofst();
    while (lex.is_s(LEX_COMMA)) {
        try {
            stack_to_poliz_till_lex(LEX_LPARENTH);
            //
        }
        catch (const char * s) {
            throw s;
        }
        catch(...) {
            throw "Error";
        }
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();

        } 
        if (lex.is_s(LEX_EOF)) {
            return;
        }
        Case();
        args.increase_ofst();
    }
}

void Parser::Block() {
    if (lex.is_s(LEX_RBRACE)) {
        return;
    } else {
        Start();
    }
}

void Parser::Repeat() {
    if (lex.is_s(LEX_LBRACE)) {
        s.push(LEX_LBRACE);
        p.put_lex(LEX_LBRACE);
        get();
        while (lex.is_s(LEX_NEWLINE)) {
            cout << "+ "; get();
        } 
        Block();
        if (!lex.is_s(LEX_RBRACE)) {
            throw "Invalid input: no '}' found";
        } else {
            stack_to_poliz_till_lex(LEX_LBRACE);
            if (s.top() == LEX_LBRACE) s.pop();
            p.put_lex(lex);
            get();
        }
    } else {
        Start();
        if (lex.is_s(LEX_BREAK)) {
            p.put_lex(LEX_BREAK);
            get();
        } else {
            throw "No exit from repeat found";
        }
    }
}

Poliz Parser::get_line() {

    try {
        p = Poliz();
        get();
        Program();
        return p;
    }
    catch(const char * s) {
        throw s;
    } catch(...) {
        throw "Error";
    }
}