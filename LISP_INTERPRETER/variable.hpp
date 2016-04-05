/**
 * @file variable.hpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */

#include <iostream>
#include <list>

using namespace std;

class variable {
    enum VarType {CHAR, INT, FLOAT, STRING, FUNCTION, NIL, SOMETHING_D};
    private:
        VarType type;
        void *data;
    public:
        variable(void);
        variable(VarType t, void *var);
        void parse(string input);
};
