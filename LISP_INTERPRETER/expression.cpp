/**
 * @file expression.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */
 
#include "expression.hpp"

expression::expression(void)
{
    
}

expression::expression(variable var)
{
    finalVar = var;
}

expression::expression(string input)
{
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
                if(start == length) {
                    endFlag = true;
                }
            }
            
            if(endFlag && exps.size() == 0) {
                finalVar.parse(tmp);
                debugString = tmp;
                //cout << tmp << endl;
            } else { // add expression to list
                exps.push_back(new expression(tmp));
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
    //TODO: make proper function
    cout << "evaluate\n";
    return finalVar;
}

void expression::print(void)
{
    if(exps.size() == 0) {
        cout << debugString << endl;
        return;
    }
    for(list<expression *>::iterator it = exps.begin(); it != exps.end(); it++) {
        (*it)->print();
    }
}
