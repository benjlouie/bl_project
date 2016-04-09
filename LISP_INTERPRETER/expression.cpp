/**
 * @file expression.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */
 
#include "expression.hpp"

expression::expression(variable var)
{
    finalVar = var;
	finalVar.globalVars = globalVars;
	finalVar.localVars = &localVars;
}

expression::expression(string input, unordered_map<string, variable> *globalVars)
{
	finalVar.globalVars = globalVars;
	finalVar.localVars = &localVars;
    parse(input);
}

void expression::parse(string input)
{
    // separate into list of expressions
        // separated by spaces or parentheses
    unsigned start=0;
    unsigned length = input.size();
    while(start < length) {
        
        if(isgraph(input[start])) { // letter
            //TODO: check for bad characters
            string tmp = "";
            bool endFlag = false;
            if(input[start] == '(') { // get parentheses chunk
                tmp = paren_chunk(input.substr(start), &start);
            } else {
                tmp = var_chunk(input.substr(start), &start);
                //TODO: check for special arguments here
                
                if(start == length) {
                    endFlag = true;
                }
            }
            
            if(endFlag && exps.size() == 0) {
                finalVar.parse(tmp);
                debugString = tmp;
            } else { // add expression to list
                exps.push_back(new expression(tmp, globalVars));
            }
        } else {
            start++;
        }
    }
}

//grabs that parentheses chunk, increments index
string expression::paren_chunk(string input, unsigned *index)
{
    unsigned lparen = 1;
    
    if(input[0] != '(') {
        throw invalid_argument("paren_chunk(): no opening parenthesis in \"" + input + "\"\n");
    }
    
    for(unsigned i = 1; i < input.size(); i++) {
        if(input[i] == '(') {
            lparen++;
        } else if (input[i] == ')') {
            lparen--;
        }
        if(lparen == 0) {
            *index += i + 1;
            return input.substr(1, i - 1);
        }
    }
    
    throw invalid_argument("paren_chunk(): no closing parenthesis in \"" + input + "\"\n");
}

//grabs that variable chunk, increments index
string expression::var_chunk(string input, unsigned *index)
{
    unsigned i;
    for(i = 0; i < input.size(); i++) {
        char c = input[i];
        if(c == '(' || isspace(c)) {
            break;
        }
    }
    *index += i;
    return input.substr(0, i);
}

variable expression::evaluate(void)
{
    if(exps.size() == 0) {
        return finalVar;
    }
    
    variable vars[exps.size()];
    
    int size = 0;
    for(list<expression *>::reverse_iterator rit = exps.rbegin(); rit != exps.rend(); rit++) {
        vars[size] = (*rit)->evaluate(); //CHECK: might need try,catch here
        size++;
    }
    //have array of variables (ex: 5 10 3 + +)
    
    int i = 0;
    while(size > 1) {
        // get to operator
        while(i < size && !vars[i].isOperator()) {
            i++;
        }
        if(i == size) {
            // not enough operators for operands
            throw invalid_argument("expression::evaluate(): not enough operators\n");
        } else if(i < 2) {
            // not enough operands for operation
            throw invalid_argument("expression::evaluate(): not enough operands\n");
        } else {
            vars[i-2] = vars[i].operate(vars[i-1], vars[i-2]); //CHECK: might need try,catch here
            memmove(&vars[i - 1], &vars[i + 1], (size - i) * sizeof(variable));
            i--;
            size -= 2;
        }
    }
    return vars[0];
}

void expression::print(void)
{
    if(exps.size() == 0) {
        cout << debugString;
        return;
    }
    for(list<expression *>::iterator it = exps.begin(); it != exps.end(); it++) {
        cout << "[";
        (*it)->print();
        cout << "]";
    }
}
