/**
 * @file variable.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */
#include "variable.hpp"

variable::variable(void)
{
    type = NIL;
    data.str = NULL;
    op = OPERAND;
}

variable::variable(string input, bool isString = false)
{
    if(isString) {
        type = STRING;
		data.str = new string;
		*data.str = input;
        op = OPERAND;
    } else {
        parse(input);
    }
}

void variable::parse(string input)
{
    //TODO: make proper function
    //cout << input << " : ";
    if(isalpha(input[0])) {
        //variable or string or character
		if ((*globalVars).count(input)) { //found variable
			*this = (*globalVars)[input];
		}
		else {
			data.str = new string;
			*data.str = input;
			type = VARIABLE;
		}
        //cout << "VARIABLE\n";
    } else if(isdigit(input[0]) || (input[0] == '-' && isdigit(input[1]))) {
        if(input.find('.') != string::npos) {
            //float value
            data.f = stof(input);
            type = FLOAT;
            //cout << "FLOAT\n";
        } else {
            //int value
            data.i = stoi(input);
            type = INT;
            //cout << "INT\n";
        }
    } else {
        type = UNKNOWN;
        switch(input[0]) {
            case '+':
                op = OPERATOR;
                operation = &variable::add;
                break;
            case '-':
                op = OPERATOR;
				operation = &variable::subtract;
                break;
            case '*':
                op = OPERATOR;
				operation = &variable::multiply;
                break;
            case '/':
                op = OPERATOR;
				operation = &variable::divide;
                break;
            case '%':
                op = OPERATOR;
				operation = &variable::modulus;
                break;
            default: //incorect input
                throw invalid_argument("variable::parse(): input \"" + input + "\" cannot be parsed\n");
        }
        //cout << "OPERATOR\n";
    }
}

variable variable::operate(variable var1, variable var2)
{
    variable var11 = var1;
    variable var22 = var2;
	//TODO: first check if they are in localVars, then check for global
    if(var1.type == VARIABLE) {
        var11 = (*globalVars)[*var1.data.str];
    }
    if(var2.type == VARIABLE) {
        var22 = (*globalVars)[*var2.data.str];
    }
    return (this->*operation)(&var11, &var22);
}

bool variable::isOperator(void)
{
    if(op == OPERATOR) {
        return true;
    }
    return false;
}

string variable::getType(void)
{
    switch(type) {
        case CHAR:
            return "CHAR";
        case INT:
            return "INT";
        case FLOAT:
            return "FLOAT";
        case STRING:
            return "STRING";
        case NIL:
            return "NIL";
        case UNKNOWN:
            return "UNKNOWN";
        default:
            return "SOMETHING BORKED";
    }
}

string variable::toString(void)
{
    switch(op) {
        case OPERAND:
            switch(type) {
                case CHAR:
                    return "" + data.c;
                case INT:
                    return to_string(data.i);
                case FLOAT:
                    return to_string(data.f);
                case STRING:
                    return *data.str;
                case VARIABLE:
                    return (*globalVars)[*data.str].toString();
                case NIL:
                    return "NIL";
                case UNKNOWN:
                    return "~UNKNOWN~";
            }
        case OPERATOR:
			if (operation == &variable::add) {
                return "+";
			}
			else if (operation == &variable::subtract) {
                return "-";
			}
			else if (operation == &variable::multiply) {
                return "*";
			}
			else if (operation == &variable::divide) {
                return "/";
			}
			else if (operation == &variable::modulus) {
                return "%";
            }
        case FUNCTION:
            return "~FUNCTION~";
        default:
            return "variable::toString() SOMETHING BORKED";
    }
}

variable variable::add(variable *var1, variable *var2)
{
	variable retVal;
	retVal.op = OPERAND;
	if ((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
		if (var1->type == INT) {
			if (var2->type == INT) {
				retVal.type = INT;
				retVal.data.i = var1->data.i + var2->data.i;
			}
			else {
				retVal.type = FLOAT;
				retVal.data.f = float(var1->data.i) + var2->data.f;
			}
		}
		else {
			retVal.type = FLOAT;
			if (var2->type == INT) {
				retVal.data.f = var1->data.f + float(var2->data.i);
			}
			else {
				retVal.data.f = var1->data.f + var2->data.f;
			}
		}
	}
	else if (var1->type == STRING && var2->type == STRING) {
		retVal.type = STRING;
		retVal.data.str = new string; //CEHCK: can I find a way to not leak this?
		*retVal.data.str = *var1->data.str + *var2->data.str;
	}
	else {
		// ERROR: can't add bad types
		throw invalid_argument("variable::add(): can't add " + var1->getType() + " and " + var2->getType() + "\n");
	}

	return retVal;
}

variable variable::subtract(variable *var1, variable *var2)
{
	variable retVal;
	retVal.op = OPERAND;
	if ((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
		if (var1->type == INT) {
			if (var2->type == INT) {
				retVal.type = INT;
				retVal.data.i = var1->data.i - var2->data.i;
			}
			else {
				retVal.type = FLOAT;
				retVal.data.f = float(var1->data.i) - var2->data.f;
			}
		}
		else {
			retVal.type = FLOAT;
			if (var2->type == INT) {
				retVal.data.f = var1->data.f - float(var2->data.i);
			}
			else {
				retVal.data.f = var1->data.f - var2->data.f;
			}
		}
	}
	else {
		// ERROR: can't add bad types
		throw invalid_argument("variable::subtract(): can't subtract " + var1->getType() + " and " + var2->getType() + "\n");
	}

	return retVal;
}

variable variable::multiply(variable *var1, variable *var2)
{
	variable retVal;
	retVal.op = OPERAND;
	if ((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
		if (var1->type == INT) {
			if (var2->type == INT) {
				retVal.type = INT;
				retVal.data.i = var1->data.i * var2->data.i;
			}
			else {
				retVal.type = FLOAT;
				retVal.data.f = float(var1->data.i) * var2->data.f;
			}
		}
		else {
			retVal.type = FLOAT;
			if (var2->type == INT) {
				retVal.data.f = var1->data.f * float(var2->data.i);
			}
			else {
				retVal.data.f = var1->data.f * var2->data.f;
			}
		}
	}
	else {
		// ERROR: can't add bad types
		throw invalid_argument("variable::multiply(): can't multiply " + var1->getType() + " and " + var2->getType() + "\n");
	}

	return retVal;
}

variable variable::divide(variable *var1, variable *var2)
{
	variable retVal;
	retVal.op = OPERAND;
	if ((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
		if (var1->type == INT) {
			if (var2->type == INT) {
				retVal.type = INT;
				if (var2->data.i == 0) {
					throw invalid_argument("variable::divide(): division by zero\n");
				}
				else {
					retVal.data.i = var1->data.i / var2->data.i;
				}
			}
			else {
				retVal.type = FLOAT;
				if (var2->data.i == 0) {
					throw invalid_argument("variable::divide(): division by zero\n");
				}
				else {
					retVal.data.f = float(var1->data.i) / var2->data.f;
				}
			}
		}
		else {
			retVal.type = FLOAT;
			if (var2->type == INT) {
				if (var2->data.i == 0) {
					throw invalid_argument("variable::divide(): division by zero\n");
				}
				else {
					retVal.data.f = var1->data.f / float(var2->data.i);
				}
			}
			else {
				if (var2->data.i == 0) {
					throw invalid_argument("variable::divide(): division by zero\n");
				}
				else {
					retVal.data.f = var1->data.f / var2->data.f;
				}
			}
		}
	}
	else {
		// ERROR: can't add bad types
		throw invalid_argument("variable::divide(): can't divide " + var1->getType() + " and " + var2->getType() + "\n");
	}

	return retVal;
}

variable variable::modulus(variable *var1, variable *var2)
{
	variable retVal;
	retVal.op = OPERAND;
	if ((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
		if (var1->type == INT) {
			if (var2->type == INT) {
				retVal.type = INT;
				if (var2->data.i == 0) {
					throw invalid_argument("variable::modulus(): division by zero\n");
				}
				else {
					retVal.data.i = var1->data.i % var2->data.i;
				}
			}
			else {
				retVal.type = FLOAT;
				if (var2->data.i == 0) {
					throw invalid_argument("variable::modulus(): division by zero\n");
				}
				else {
					retVal.data.f = fmod(float(var1->data.i), var2->data.f);
				}
			}
		}
		else {
			retVal.type = FLOAT;
			if (var2->type == INT) {
				if (var2->data.i == 0) {
					throw invalid_argument("variable::modulus(): division by zero\n");
				}
				else {
					retVal.data.f = fmod(var1->data.f, float(var2->data.i));
				}
			}
			else {
				if (var2->data.i == 0) {
					throw invalid_argument("variable::modulus(): division by zero\n");
				}
				else {
					retVal.data.f = fmod(var1->data.f, var2->data.f);
				}
			}
		}
	}
	else {
		// ERROR: can't add bad types
		throw invalid_argument("variable::modulus(): can't modulus " + var1->getType() + " and " + var2->getType() + "\n");
	}

	return retVal;
}