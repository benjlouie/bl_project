/**
 * @file expression.hpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */

#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <stdexcept>
#include "variable.hpp"

using namespace std;

class expression {
    private:
        list<expression *> exps;
        variable finalVar;
        string debugString;
        string paren_chunk(string input, unsigned *index);
        string var_chunk(string input, unsigned *index);
    public:
        expression(void);                       //empty
        expression(variable var);               //has variable (no list)
        expression(string value);               //list of expressions (or just variable if only one)
        void parse(string input);
        variable evaluate(void);
        void print(void);
};
