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
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class variable {
    enum VarType {CHAR, INT, FLOAT, STRING, NIL, UNKNOWN};
    enum OpType {OPERAND, OPERATOR, FUNCTION};
    private:
        VarType type = UNKNOWN;
        OpType op = OPERAND;
        union var {
            char c;
            int i;
            float f;
            string *str;
        } data;
        variable (*operation)(variable *var1, variable *var2);
    public:
        variable(void);
        variable(string input);
        void parse(string input);
        
};
