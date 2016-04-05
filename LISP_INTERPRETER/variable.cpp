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
    data = NULL;
}

variable::variable(VarType t, void *var)
{
    type = t;
    data = var;
}

void variable::parse(string input)
{
    //TODO: make proper function
    type = SOMETHING_D;
}
