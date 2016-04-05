/**
 * @file Interpreter.cpp
 * @author Ben Louie
 *
 * CSE 324
 * Project 2
 * Due 4/28/16
 */

#include <iostream>
#include <list>
#include <string>
#include "expression.hpp"

using namespace std;


int main()
{
    list<expression*> l;

    l.push_front(new expression("(+ 3 4)"));
    
    
    return 0;
}
