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
    cout << "inside evaluate()" << endl;
    if(exps.size() == 0) {
        cout << "eval recurse 0: " + finalVar.toString() << endl;
        return finalVar;
    }
    
    if(exps.size() == 1) {
        return exps.front()->evaluate();
    }
    
    
    while(exps.size() > 1) {
        expression *opExp = exps.front();
        variable eval = opExp->evaluate();
        cout << "test1\n";
        if(eval.isOperator()) {
            cout << "test2\n";
            // needs to operate on the next two
            exps.pop_front();
            expression exp1 = *exps.front();
            exps.pop_front();
            expression exp2 = *exps.front();
            exps.pop_front();
            opExp->finalVar = eval.operate(exp1.evaluate(), exp2.evaluate());
            exps.push_front(opExp);
        } else {
            break;
        }
    }
    return exps.front()->finalVar;
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
