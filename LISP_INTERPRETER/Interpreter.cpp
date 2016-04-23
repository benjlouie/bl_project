/**
 * @file Interpreter.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */

#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>
#include <string>
#include "expression.hpp"

using namespace std;

unordered_map<string, variable> globalVars;

int main()
{
	ofstream outFile;
	outFile.open("output.txt");
	if (!outFile.is_open()) {
		cout << "Warning: \"output.txt\" could not be opened, no output file will be written" << endl;
	}

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
		string result = "None";
		try{
			exp = new expression(input, &globalVars);
			result = exp->evaluate().toString();
		}
		catch (exception &e) {
			result = "ERROR: " + (string)e.what();
		}
		if (exp) {
			delete exp;
		}
		cout << result << endl;
		if (outFile.is_open()) {
			outFile << result << endl;
		}
	}
    
	outFile.close();
    return 0;
}
