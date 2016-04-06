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

    //l.push_front(new expression("testval(this(for(ever(and(ever(and(all(that)))ok)))) is)something(a test)(ok(k))"));
    cout << endl << endl;
    l.push_back(new expression("(+ + 3 10 5)"));
    
    for(list<expression *>::iterator it = l.begin(); it != l.end(); it++) {
        (*it)->print();
    }
    cout << endl;
    
    expression *exp = l.front();
    variable answer = exp->evaluate();
    
    cout << answer.toString() << endl;
    
    
    return 0;
}
