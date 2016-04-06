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

variable::variable(string input)
{
    parse(input);
}

void variable::parse(string input)
{
    //TODO: make proper function
    cout << input << " : ";
    if(isalpha(input[0])) {
        //variable or string or character
        data.str = new string;
        *data.str = input;
        type = STRING;
        cout << "STRING\n";
    } else if(isdigit(input[0]) || (input[0] == '-' && isdigit(input[1]))) {
        if(input.find('.') != string::npos) {
            //float value
            data.f = stof(input);
            type = FLOAT;
            cout << "FLOAT\n";
        } else {
            //int value
            data.i = stoi(input);
            type = INT;
            cout << "INT\n";
        }
    } else {
        type = UNKNOWN;
        switch(input[0]) {
            case '+':
                op = OPERATOR;
                operation = add;
                break;
            case '-':
                op = OPERATOR;
                operation = subtract;
                break;
            case '*':
                op = OPERATOR;
                operation = multiply;
                break;
            case '/':
                op = OPERATOR;
                operation = divide;
                break;
            case '%':
                op = OPERATOR;
                operation = modulus;
                break;
            default: //incorect input
                throw invalid_argument("variable::parse(): input \"" + input + "\" cannot be parsed\n");
        }
        cout << "OPERATOR\n";
    }
}

//TODO: implement these
variable variable::add(variable *var1, variable *var2)
{
    variable retVal;
    retVal.op = OPERAND;
    if((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
        if(var1->type == INT) {
            if(var2->type == INT) {
                retVal.type = INT;
                retVal.data.i = var1->data.i + var2->data.i;
            } else {
                retVal.type = FLOAT;
                retVal.data.f = float(var1->data.i) + var2->data.f;
            }
        } else {
            retVal.type = FLOAT;
            if(var2->type == INT) {
                retVal.data.f = var1->data.f + float(var2->data.i);
            } else {
                retVal.data.f = var1->data.f + var2->data.f;
            }
        }
    } else {
        // ERROR: can't add bad types
        throw invalid_argument("variable::add(): can't add " + var1->getType() + " and " + var2->getType() + "\n");
    }
    
    return retVal;
}

variable variable::subtract(variable *var1, variable *var2)
{
    variable retVal;
    retVal.op = OPERAND;
    if((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
        if(var1->type == INT) {
            if(var2->type == INT) {
                retVal.type = INT;
                retVal.data.i = var1->data.i - var2->data.i;
            } else {
                retVal.type = FLOAT;
                retVal.data.f = float(var1->data.i) - var2->data.f;
            }
        } else {
            retVal.type = FLOAT;
            if(var2->type == INT) {
                retVal.data.f = var1->data.f - float(var2->data.i);
            } else {
                retVal.data.f = var1->data.f - var2->data.f;
            }
        }
    } else {
        // ERROR: can't add bad types
        throw invalid_argument("variable::subtract(): can't subtract " + var1->getType() + " and " + var2->getType() + "\n");
    }
    
    return retVal;
}

variable variable::multiply(variable *var1, variable *var2)
{
    variable retVal;
    retVal.op = OPERAND;
    if((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
        if(var1->type == INT) {
            if(var2->type == INT) {
                retVal.type = INT;
                retVal.data.i = var1->data.i * var2->data.i;
            } else {
                retVal.type = FLOAT;
                retVal.data.f = float(var1->data.i) * var2->data.f;
            }
        } else {
            retVal.type = FLOAT;
            if(var2->type == INT) {
                retVal.data.f = var1->data.f * float(var2->data.i);
            } else {
                retVal.data.f = var1->data.f * var2->data.f;
            }
        }
    } else {
        // ERROR: can't add bad types
        throw invalid_argument("variable::multiply(): can't multiply " + var1->getType() + " and " + var2->getType() + "\n");
    }
    
    return retVal;
}

variable variable::divide(variable *var1, variable *var2)
{
    variable retVal;
    retVal.op = OPERAND;
    if((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
        if(var1->type == INT) {
            if(var2->type == INT) {
                retVal.type = INT;
                retVal.data.i = var1->data.i / var2->data.i;
            } else {
                retVal.type = FLOAT;
                retVal.data.f = float(var1->data.i) / var2->data.f;
            }
        } else {
            retVal.type = FLOAT;
            if(var2->type == INT) {
                retVal.data.f = var1->data.f / float(var2->data.i);
            } else {
                retVal.data.f = var1->data.f / var2->data.f;
            }
        }
    } else {
        // ERROR: can't add bad types
        throw invalid_argument("variable::divide(): can't divide " + var1->getType() + " and " + var2->getType() + "\n");
    }
    
    return retVal;
}

variable variable::modulus(variable *var1, variable *var2)
{
    variable retVal;
    retVal.op = OPERAND;
    if((var1->type == INT || var1->type == FLOAT) && (var2->type == INT || var2->type == FLOAT)) {
        if(var1->type == INT) {
            if(var2->type == INT) {
                retVal.type = INT;
                retVal.data.i = var1->data.i % var2->data.i;
            } else {
                retVal.type = FLOAT;
                retVal.data.f = fmod(float(var1->data.i), var2->data.f);
            }
        } else {
            retVal.type = FLOAT;
            if(var2->type == INT) {
                retVal.data.f = fmod(var1->data.f, float(var2->data.i));
            } else {
                retVal.data.f = fmod(var1->data.f, var2->data.f);
            }
        }
    } else {
        // ERROR: can't add bad types
        throw invalid_argument("variable::modulus(): can't modulus " + var1->getType() + " and " + var2->getType() + "\n");
    }
    
    return retVal;
}

variable variable::operate(variable var1, variable var2)
{
    return (this->*operation)(&var1, &var2);
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
            return "variable::getType() SOMETHING BORKED";
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
                case NIL:
                    return "NIL";
                case UNKNOWN:
                    return "~UNKNOWN~";
            }
        case OPERATOR:
            if(operation == &add) {
                return "+";
            } else if(operation == &subtract) {
                return "-";
            } else if(operation == &multiply) {
                return "*";
            } else if(operation == &divide) {
                return "/";
            } else if(operation == &modulus) {
                return "%";
            }
        case FUNCTION:
            return "~FUNCTION~";
        default:
            return "variable::toString() SOMETHING BORKED";
    }
}
