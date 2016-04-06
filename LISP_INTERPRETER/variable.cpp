/**
 * @file variable.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */
#include "variable.hpp"

variable v_add(variable *var1, variable *var2);
variable v_subtract(variable *var1, variable *var2);
variable v_multiply(variable *var1, variable *var2);
variable v_divide(variable *var1, variable *var2);
variable v_modulus(variable *var1, variable *var2);

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
    cout << input << "\t\t";
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
        // TODO: + - * /
        switch(input[0]) {
            case '+':
                op = OPERATOR;
                operation = v_add;
                break;
            case '-':
                op = OPERATOR;
                operation = v_subtract;
                break;
            case '*':
                op = OPERATOR;
                operation = v_multiply;
                break;
            case '/':
                op = OPERATOR;
                operation = v_divide;
                break;
            case '%':
                op = OPERATOR;
                operation = v_modulus;
                break;
            default: //incorect input
                throw invalid_argument("variable::parse(): input \"" + input + "\" cannot be parsed\n");
        }
        cout << "OPERATOR\n";
    }
}

//TODO: implement these
variable v_add(variable *var1, variable *var2)
{
    variable retVal;
    
    return retVal;
}

variable v_subtract(variable *var1, variable *var2)
{
    variable retVal;

    return retVal;
}

variable v_multiply(variable *var1, variable *var2)
{
    variable retVal;

    return retVal;
}

variable v_divide(variable *var1, variable *var2)
{
    variable retVal;

    return retVal;
}

variable v_modulus(variable *var1, variable *var2)
{
    variable retVal;

    return retVal;
}
