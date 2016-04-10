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
#include <unordered_map>
#include <string>
#include <cctype>
#include <stdexcept>
#include "variable.hpp"

using namespace std;

class expression {
    private:
    	unordered_map<string, variable> localVars;
        list<expression *> exps;
        variable finalVar;
        string paren_chunk(string input, unsigned *index);
        string var_chunk(string input, unsigned *index);
        bool special_exp(string input, unsigned *index);
	public:
		string debugString;
    	unordered_map<string, variable> *globalVars;
		expression(unordered_map<string, variable> *global);
        expression(variable var, unordered_map<string, variable> *global);               //has variable (no list)
        expression(string input, unordered_map<string, variable> *global);              //list of expressions (or just variable if only one)
        void parse(string input);
        variable evaluate(void);
        void print(void);
};
