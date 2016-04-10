/**
 * @file Interpreter.cpp
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
#include "expression.hpp"

using namespace std;

unordered_map<string, variable> globalVars;

int main()
{
    while(1) {
    	cout << "Prompt> ";
    	string input;
    	getline(cin, input);
    	if(input.size() == 0) {
    		continue;
		}
    	
		if (input == "quit" || input == "(quit)") {
			break;
		}

    	expression *exp = NULL;
		try{
			exp = new expression(input, &globalVars);
			cout << exp->evaluate().toString() << endl;
		}
		catch (exception &e) {
			cout << "ERROR: " << e.what() << endl;
		}
		if (exp) {
			delete exp; //TODO: find way to not delete functions
		}
	}
    
    return 0;
}
