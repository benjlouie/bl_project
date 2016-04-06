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
#include <cmath>

using namespace std;

class variable {
    enum VarType {CHAR, INT, FLOAT, STRING, NIL, UNKNOWN};
    enum OpType {OPERAND, OPERATOR, FUNCTION};
    private:
        VarType type = UNKNOWN;
        OpType op = OPERAND;
        variable (variable::*operation)(variable *var1, variable *var2) = NULL;
        union var {
            char c;
            int i;
            float f;
            string *str;
        } data;
        
        variable add(variable *var1, variable *var2);
        variable subtract(variable *var1, variable *var2);
        variable multiply(variable *var1, variable *var2);
        variable divide(variable *var1, variable *var2);
        variable modulus(variable *var1, variable *var2);
    public:
        variable(void);
        variable(string input);
        void parse(string input);
        variable operate(variable var1, variable var2);
        bool isOperator(void);
        string getType(void);
        string toString(void);
};