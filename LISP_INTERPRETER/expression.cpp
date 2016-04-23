/**
 * @file expression.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */
 
#include "expression.hpp"

expression::expression(unordered_map<string, variable> *global)
{
	globalVars = global;
}

expression::expression(variable var, unordered_map<string, variable> *global)
{
    finalVar = var;
	globalVars = global;
	finalVar.globalVars = global;
	finalVar.localVars = &localVars;
}

expression::expression(string input, unordered_map<string, variable> *global)
{
	globalVars = global;
	finalVar.globalVars = global;
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
                //TODO: check for special arguments here
                if(special_exp(input.substr(start), &start)) {
                    continue;
                } else {
                    tmp = var_chunk(input.substr(start), &start);
                }
                
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
		throw invalid_argument("paren_chunk(): no opening parenthesis");
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
	throw invalid_argument("paren_chunk(): no closing parenthesis");
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
		if (finalVar.isFunction()) {
			expression tmpExp(finalVar.getFuncString(), globalVars);
			variable tmpVar = tmpExp.evaluate();
			return tmpVar;
		}
        return finalVar;
    }
    
    variable *vars = new variable[exps.size()];
    
    int size = 0;
    for(list<expression *>::reverse_iterator rit = exps.rbegin(); rit != exps.rend(); rit++) {
        vars[size] = (*rit)->evaluate();
		if (vars[size].isFunction()) {
			expression tmpExp(vars[size].getFuncString(), globalVars);
			vars[size] = tmpExp.evaluate();
		}
		if (!vars[size].isNil()) { // only add to the stack if it isn't NIL
			size++;
		}
    }
    //have array of variables (ex: 5 10 3 + +)


    int i = 0;
	while (size > 1) {
		while (i < size && !vars[i].isOperator()) {
			i++;
		}
		if (i == size) {
			// not enough operators for operands
			throw invalid_argument("expression::evaluate(): not enough operators");
		}
		else if (i < 2) {
			// not enough operands for operation
			throw invalid_argument("expression::evaluate(): not enough operands");
		}
		else {
			vars[i - 2] = vars[i].operate(vars[i - 1], vars[i - 2]);
			memmove(&vars[i - 1], &vars[i + 1], (size - i) * sizeof(variable));
			i--;
			size -= 2;
		}
	}
	variable tmp = vars[0];
	delete vars;
	return tmp;
}

bool expression::special_exp(string input, unsigned *index)
{
    unsigned ind = 0;
    string command = var_chunk(input, &ind);
    if(command == "quote") {
        if((ind = input.find('(')) != string::npos) {
            string strChunk = paren_chunk(input.substr(ind), &ind);
            variable tmpVar(strChunk, true, false);
            exps.push_back(new expression(tmpVar, globalVars));
            *index += ind;
        } else {
            throw invalid_argument("expression::special_exp(): quote needs parenthesized expression");
        }
    } else if(command == "if") {
		string conditional;
		string conseq;
		string alt;
		//conditional
		if ((ind = input.find('(')) != string::npos) {
			conditional = paren_chunk(input.substr(ind), &ind);
		}
		else {
			throw invalid_argument("expression::special_exp(): if needs parenthesized conditional");
		}
		//conseq
		while (ind < input.size() && input[ind] != '(') { ind++; }
		if (ind != input.size()) {
			conseq = paren_chunk(input.substr(ind), &ind);
		} 
		else {
			throw invalid_argument("expression::special_exp(): if needs parenthesized conseq");
		}
		//alt
		while (ind < input.size() && input[ind] != '(') { ind++; }
		if (ind != input.size()) {
			alt = paren_chunk(input.substr(ind), &ind);
		}
		else {
			throw invalid_argument("expression::special_exp(): if needs parenthesized alt");
		}
		//add resulting expression to list
		expression tmpExp(conditional, globalVars);
		variable tmpVar = tmpExp.evaluate();
		if (tmpVar.getType() == "BOOL") {
			if (tmpVar.toString() == "true") {
				exps.push_back(new expression(conseq, globalVars));
			}
			else {
				exps.push_back(new expression(alt, globalVars));
			}
		}
		else {
			throw invalid_argument("expression::special_exp(): if must have valid conditional");
		}
		*index += ind;
    } else if(command == "define") {
		//name of new variable
		while (ind < input.size() && isspace(input[ind])) { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): define needs a variable name");
		}
		string varName = var_chunk(input.substr(ind), &ind);
		if (varName.size() == 0) {
			throw invalid_argument("expression::special_exp(): define needs a variable name");
		}

		//expression to assign to new variable
		while (ind < input.size() && isspace(input[ind])) { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): define needs an expression to assign");
		}
		string expChunk;
		variable value;
		if (input[ind] == '(') {
			expChunk = paren_chunk(input.substr(ind), &ind);
			expression tmpExp(expChunk, globalVars);
			value = tmpExp.evaluate();
		}
		else {
			expChunk = var_chunk(input.substr(ind), &ind);
			expression tmpExp(expChunk, globalVars);
			value = tmpExp.evaluate();
		}

		//set variable in globalList and add to current expression
		(*globalVars)[varName] = value;
		//exps.push_back(new expression(value, globalVars));
		*index += ind;
    } else if(command == "set!") {
		//name of variable
		while (ind < input.size() && isspace(input[ind])) { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): set! needs a variable name");
		}
		string varName = var_chunk(input.substr(ind), &ind);

		//expression to assign to variable
		while (ind < input.size() && isspace(input[ind])) { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): set! needs an expression to assign");
		}
		string expChunk;
		variable value;
		if (input[ind] == '(') {
			expChunk = paren_chunk(input.substr(ind), &ind);
			expression tmpExp(expChunk, globalVars);
			value = tmpExp.evaluate();
		}
		else {
			expChunk = var_chunk(input.substr(ind), &ind);
			expression tmpExp(expChunk, globalVars);
			value = tmpExp.evaluate();
		}

		if ((*globalVars).count(varName)) { //variable exists
			(*globalVars)[varName] = value;
			//exps.push_back(new expression(value, globalVars));
			*index += ind;
		}
		else {
			throw invalid_argument("expression::special_exp(): set! variable \"" + varName + "\" does not exist");
		}
    } else if(command == "lambda") {
		//name of new function
		while (ind < input.size() && isspace(input[ind])) { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): lambda needs a function name");
		}
		string funcName = var_chunk(input.substr(ind), &ind);
		if (funcName.size() == 0) {
			throw invalid_argument("expression::special_exp(): lambda needs a function name");
		}

		while (ind < input.size() && input[ind] != '(') { ind++; }
		if (ind == input.size()) {
			throw invalid_argument("expression::special_exp(): lambda needs a parenthesized function body");
		}
		string funcBody = paren_chunk(input.substr(ind), &ind);
		variable tmpVar(funcBody, false, true);

		(*globalVars)[funcName] = tmpVar;
		*index += ind;
    } else {
        return false;
    }
    return true;
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