#include <iostream>
#include <cstring>
#include <stack>
#include "./parser.h"

using namespace std;

Poliz::Poliz (int max_size) {
    p = new Lex[siz = max_size];
    sfree = 0;
}
Poliz::~Poliz() {
    delete []p; 
}
void Poliz::put_lex(const Lex & l) { 
    p[sfree] = l; 
    sfree++; 
}
void Poliz::put_lex(const Lex & l, int place) { 
    p[place] = l; 
}
void Poliz::blank() { 
    sfree++; 
}
int Poliz::size() const { 
    return sfree; 
}
Lex & Poliz::operator[] (int index) const {
    if (index > siz) {
        throw "POLIZ: out of array";
    } else {
        if (index > sfree) {
            throw "POLIZ: indefinite element of array";
        } else {
            return p[index];
        }
    }
}
/*void Poliz::print() const {
    for (int i = 0; i < sfree; i++) {
        cout << p[i];
    }
}
*/
bool Function::is_here (const string &s) const {
    for (vector<pair<string, Type>>::const_iterator i = v_table.begin(); 
        i != v_table.end(); i++) {
        if (i->first == s) {
            return true;
        }
    }
    return false;
}


template <class T, int max_size> 
Stck <T, max_size>::Stck() {
    top = 0;
}
template <class T, int max_size> 
void Stck <T, max_size>::reset() { 
    top = 0; 
}
template <class T, int max_size> 
bool Stck <T, max_size>::is_empty() const { 
    return top == 0; 
}
template <class T, int max_size> 
bool Stck <T, max_size>::is_full() const { 
    return top == max_size; 
}

template <class T, int max_size>
void Stck <T, max_size>::push(const T & i) {
    if (!is_full()) {
        s[top] = i;
        top++;
    } else {
        throw "Stack_is_full";
    }
}
template <class T, int max_size >
T Stck <T, max_size>::pop() {
    if (!is_empty()) {
        top--;
        return s[top];
    } else {
        throw "Stack_is_empty";
    }
}
template <class T, int max_size >
T Stck <T, max_size>::get_top() const {
    return s[top];
}

Parser::Parser(): p(256), st(), lex(), is_file(false) {}
Parser::Parser(const char * file): Scanner(file), is_file(true) {}

void Parser::priority(type_of_lex t, Poliz * pp, Stack * s) {
    while (!s->empty() && Priority.at(s->top()) < Priority.at(t)) {
        pp->put_lex(s->top());
        s->pop();
    }
}
void Parser::empty(Poliz * pp, Stack * s) {
    while (!s->empty()) {
        pp->put_lex(s->top());
        s->pop();
    }
}
void Parser::till_sym(type_of_lex t, Poliz * pp, Stack * s) {
    while (!s->empty() && s->top() != t) {
        pp->put_lex(s->top());
        s->pop();
    }
}
void Parser::get() {
    lex = Scanner::get_lex();
}
type_of_lex Parser::sym() const { 
    return lex.sym(); 
}
const string & Parser::var() const { 
    return lex.var(); 
}
const Type & Parser::type() const { 
    return lex.type(); 
}
bool Parser::is_l(lident_t t) const { 
    return lex.is_l(t);     
}
bool Parser::is_s(type_of_lex t) const { 
    if (lex.is_l(L_SYM)) {
        return lex.is_s(t); 
    }
}
void Parser::plus() const {
    cout << "+\t";
}
void Parser::not_end() {
    while (is_s(LEX_NEWLINE)) {
        plus();
        get();
    }
}
void Parser::move_to_stack(type_of_lex t, Poliz * pp, Stack * s) {
    priority(t, pp, s);
    s->push(t);
}
void Parser::Program(Poliz * pp, Stack *s) {
    //'\0' was reached
    if (is_s(LEX_EOF)) {
        return;
    }
    //'\n' was reached
    while (is_s(LEX_NEWLINE)) { //TODO: add function of checking
        get();
    }
    //else - continue recursive descent
    Expression(pp, s);
    //after '\0', '\n' or ':' was reached
    if (is_s(LEX_NEWLINE) || is_s(LEX_SEMICOLON) || is_s(LEX_EOF) || is_s(LEX_NULL)) {
        empty(pp, s);
        pp->put_lex(LEX_SEMICOLON);
        return;
    } else {
        throw "Semantic error";
    }
}
void Parser::Expression(Poliz * pp, Stack * s, bool bp) {
    //repeat
    if (is_s(LEX_REPEAT)) {
        get();
        //Repeat();
        return;
    }
    //break
    if (bp && is_s(LEX_BREAK)) {
        pp->put_lex(LEX_BREAK);
        get();
        return;
    }
    //not variable
    if (!is_l(L_VAR)) {
        Expr1(pp, s, 0, bp);
        return;
    }
    Var(pp, s);
    if (r_type == R_FUNC) {
        Expr1(pp, s, 1, bp);
        return;
    }
    //if <-
    if (is_s(LEX_IN)) {
        s->push(sym());
        get();
        Expression(pp, s, bp);
        return;
    }
    Expr1(pp, s, 1, bp);
    return;
}
void Parser::Expr1(Poliz * pp, Stack * s, int f, bool bp) {
    Expr2(pp, s, f, bp);
    if (is_s(LEX_AND) || is_s(LEX_OR)) {
        move_to_stack(sym(), pp, s);
        get();
        Expr1(pp, s, 0, bp);
    }
}
void Parser::Expr2(Poliz * pp, Stack * s, int f, bool bp) {
    if (!f) {
        if (is_s(LEX_NOT)) {
            move_to_stack(sym(), pp, s);
            get();
        }
        Expr3(pp, s, 0, bp);
    } else {
        Expr3(pp, s, f, bp);
    }
    
}
void Parser::Expr3(Poliz * pp, Stack * s, int f, bool bp) {
    Expr4(pp, s, f, bp);
    if (is_s(LEX_LESS) || is_s(LEX_MORE) || 
        is_s(LEX_LESS_OR_EQUAL) || is_s(LEX_MORE_OR_EQUAL) || 
        is_s(LEX_EQUAL) || is_s(LEX_NOT_EQUAL)) {
        move_to_stack(sym(), pp, s);
        get();
        Expr4(pp, s, 0, bp);
    }
}
void Parser::Expr4(Poliz * pp, Stack * s, int f, bool bp) {
    Expr5(pp, s, f, bp);
    if (is_s(LEX_PLUS) || is_s(LEX_MINUS)) {
        move_to_stack(sym(), pp, s);
        get();
        Expr4(pp, s, 0, bp);
    }
}
void Parser::Expr5(Poliz * pp, Stack * s, int f, bool bp) {
    Expr6(pp, s, f, bp);
    if (is_s(LEX_MULT) || is_s(LEX_DIV)) {
        move_to_stack(sym(), pp, s);
        get();
        Expr5(pp, s, 0, bp);
    }
}
void Parser::Expr6(Poliz * pp, Stack * s, int f, bool bp) {
    Expr7(pp, s, f, bp);
    if (is_s(LEX_COLON)) {
        move_to_stack(sym(), pp, s);
        get();
        Expr6(pp, s, 0, bp);
    }
}
void Parser::Expr7(Poliz * pp, Stack * s, int f, bool bp) {
    if (f) {
        return;
    }
    not_end();
    if (is_s(LEX_LPARENTH)) {
        s->push(sym());
        get();
        Expression(pp, s);
        not_end();
        if (!is_s(LEX_RPARENTH)) {
            throw "Invalid input: no ')'";
        } else {
            till_sym(LEX_LPARENTH, pp, s);
            get();
            return;
        }
    }
    if (is_s(LEX_LBRACE)) {
        s->push(sym());
        get();
        Block(pp, s, bp);
        return;
    }
    if (is_l(L_CONST)) {
        pp->put_lex(type());
        get();
        return;
    }
    if (is_l(L_VAR)) {
        Var(pp, s);
        return;
    } 
        throw "No variable or constant met";
    
}
void Parser::FunctCall(Poliz * pp, Stack * s) {
    not_end();
    Stack tmp;  
    if (is_l(L_VAR)) {
        Var(pp, s);
        switch (r_type) {
        case R_VAR:
            if (is_s(LEX_IS)) {
                get();
                not_end();
                Expression(pp, & tmp);
                empty(pp, & tmp);
                break;
            }
        case R_VEC:
            if (is_s(LEX_IN)) {
                tmp.push(sym());
                get();
                Expression(pp, & tmp);
            } else {
                Expr1(pp, & tmp, 1);
            }
            empty(pp, & tmp);
            pp->put_lex(Lex());
                break;
        case R_FUNC:
            Expr1(pp, & tmp, 1);
            empty(pp, & tmp);
            pp->put_lex(Lex());
            break;
        default:
            break;
        }
    } else {
        Expression(pp, & tmp);
        empty(pp, & tmp);
        pp->put_lex(Lex());
    }

    not_end();

    if (is_s(LEX_COMMA)) {
        get();
        FunctCall(pp, s);
        args++;
        return;
    } else if (is_s(LEX_RPARENTH)) {
        get();
        args = 1;
        return;
    } else {
        throw "Invalid input: no ')'";
    }
}
void Parser::Var(Poliz * pp, Stack * s) {
    string v = var();
    get();
    if (is_s(LEX_LSQBRACKET)) {
        pp->put_lex(v);
        s->push(sym());
        get();
        Expression(pp, s);
        not_end();
        if (!is_s(LEX_RSQBRACKET)) {
            throw "Invalid input: no ']'";
        } else {
            till_sym(LEX_LSQBRACKET, pp, s);
            pp->put_lex(LEX_LSQBRACKET);
            get();
            r_type = R_VEC;
            return;
        }
    } 
    if (is_s(LEX_LPARENTH)) {
        get();
        if (is_s(LEX_RPARENTH)) {
            pp->put_lex(Lex(0, true)); //?
            get();
        } else {
            FunctCall(pp, s);
            pp->put_lex(Lex(args, true));
            //add labels implementation?
        }
        pp->put_lex(v);
        pp->put_lex(LEX_FUNC);
        r_type = R_FUNC;
        return;
    }
    pp->put_lex(v);
    r_type = R_VAR;
}
// void Parser::Const() {

// }
// void Parser::ArgList() {

// }
// void Parser::ArgListElem() {

// }
void Parser::Block(Poliz * pp, Stack * s, bool bp) {
    not_end();
    Expression(pp, s, bp);
    if (is_s(LEX_SEMICOLON) || is_s(LEX_NEWLINE)) {
        if (is_s(LEX_NEWLINE)) {
            plus();
        }
        get();
        not_end();
        till_sym(LEX_LBRACE, pp, s);
        if (is_s(LEX_RBRACE)) {       
            get();
            return;
        }
        s->push(LEX_LBRACE);
        pp->put_lex(LEX_SEMICOLON);
        Block(pp, s, bp);
        return;
    }
    if (is_s(LEX_RBRACE)) {
        till_sym(LEX_LBRACE, pp, s);
        get();
        return;
    }
    throw "Semantic mistake in block operator";
}
void Poliz::print() {
    FILE *f = fopen("file.txt", "w");
    for (int i = 0; i < sfree; i++) {
        if (p[i].get_type() == L_SYM) {
             for (int j = 0; j < 29; j++) {
                if (Scanner::dlms[j] == p[i].Lex::sym()) {
                    fprintf(f, "%s ", Scanner::TD[j]);
                    //cout << Scanner::TD[j] << ' ';
                    break;
                }
            }
        } else if (p[i].Lex::get_type() == L_VAR) {
            fprintf(f, "%s ", p[i].var());
        } else if (p[i].get_type() == L_CONST) {
            // if (p[i].type().elem(0).is_char()) {
            //     cout << p[i].type().elem(0).get_char_val() << ' ';
            // } else if (p[i].type().elem(0).is_num()) {
            //     cout << p[i].type().elem(0).get_num_val() << ' ';
            // } else if (p[i].type().elem(0).is_bool()) {
            //     cout << p[i].type().elem(0).get_bool_val() << ' ';
            // }
            fprintf(f, "const ");
        } else {
            fprintf(f, "something else ");
        }
    }
    fclose(f);
}
Poliz Parser::get_line() {
    try {
        get();
        Poliz p;
        Stack s;
        Program(& p, & s);
        return p;
    } catch (const char * s) {
        throw s;
    } catch(...) {
        throw "Parser Error";
    }
}

void pol(Parser &parser) {
    while (true) {
        
            auto poliz = parser.get_line();
            poliz.print();
            return;
      
    }
}

int
main (int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        try {
            Parser parser(argv[i]);
            pol(parser);
            
        } catch (const char *s) {
            cout << s << endl;
        } catch(...) {
            throw "Error";
        }
    }

    try {
        Parser parser;
        pol(parser);
        //p.print();
    } catch (const char *s) {
        cout << s << endl;
    } catch(...) {
        cout << "Error" << endl;
    }

    return 0;
}