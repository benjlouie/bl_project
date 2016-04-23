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
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <cmath>

using namespace std;

class variable {
    enum VarType {BOOL, CHAR, INT, FLOAT, STRING, VARIABLE, NIL, UNKNOWN};
    enum OpType {OPERAND, OPERATOR, FUNCTION};
    private:
        VarType type = UNKNOWN;
        OpType op = OPERAND;
        variable (variable::*operation)(variable *var1, variable *var2) = NULL;
        union var {
			bool b;
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
		variable greater(variable *var1, variable *var2);
		variable less(variable *var1, variable *var2);
		variable equal(variable *var1, variable *var2);

    public:
    	// these two should be set when instantiated
    	unordered_map<string, variable> *localVars; // for function use
    	unordered_map<string, variable> *globalVars; // for general expressions
    	
        variable(void);
        variable(string input, bool isString, bool isFunction);
        void parse(string input);
        variable operate(variable var1, variable var2);
		bool isOperator(void);
		bool isFunction(void);
		bool isNil(void);
		string getFuncString(void);
        string getType(void);
        string toString(void);
};
