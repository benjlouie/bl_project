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
    unsigned start=0, end;
    unsigned length = input.size();
    while(start < length) {
        string tmp = "";
        if(isgraph(input[start])) { // letter
            //TODO: check for bad characters
            
            bool endFlag = false;
            if(input[start] == '(') { // get parentheses chunk
                tmp = paren_chunk(input.substr(start), &start);
            } else {
                end = start;
                while(end < length) {
                    char c = input[end];
                    if(c == '(' || isspace(c)) {
                        break;
                    }
                    end++;
                }
                tmp = input.substr(start, end - start);
                start = end;
                if(start == length) {
                    endFlag = true;
                }
                
            }
            
            if(endFlag) {
                finalVar.parse(tmp);
                cout << tmp << endl;
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
        cout << "ERROR: paren_chunk(): no opening parenthesis in \"" << input << "\"\n";
        return NULL;
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
    
    cout << "ERROR: paren_chunk(): no closing parenthesis in \"" << input << "\"\n";
    return NULL;
}

//grabs that variable chunk, increments index
string expression::var_chunk(string input, unsigned *index)
{
    
}

variable expression::evaluate(void)
{
    cout << "evaluate\n";
    return finalVar;
}
