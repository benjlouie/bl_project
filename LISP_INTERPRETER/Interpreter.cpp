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

    l.push_front(new expression("testval(this is)something(a test)(ok(k))"));
    cout << endl << endl;
    l.push_back(new expression("(+ 3.678 -4 - 5)woops"));
    
    /*for(list<expression *>::iterator it = l.begin(); it != l.end(); it++) {
        (*it)->print();
    }*/
    
    
    return 0;
}
