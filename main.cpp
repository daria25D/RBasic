#include <iostream>
#include "./sinter.h"
#include "./spars.h"
#include "./scanner.h"
#include "./lexer.h"

using namespace std;

void inter (Parser & parser) {
    while (true) {
        try {
            Poliz poliz = parser.Parser::get_line(); //add implementation
            if (poliz.Poliz::size() == 0) {
                return;
            }
            Interpreter interp;
            interp.interpret_line(poliz);
        } catch (const char *s) {
            cout << s << endl;
        } catch (...) {
            cout << "Error" << endl;
        }
    }
}

int main (int argc, char * argv[])
{
    for (int i = 1; i < argc; ++i) {
        try {
            Parser parser(argv[i]);//add constructor
            inter(parser);
        } catch (const char *s) {
            cout << s << endl;
        } catch (...) {
            cout << "Error" << endl;
        }
        }
    if (argc == 1) {
        try {
            Parser parser;
            inter(parser);
        } catch (const char *s) {
            cout << s << endl;
        } catch (...) {
            cout << "Error" << endl;
        }
        return 0;
    }
}