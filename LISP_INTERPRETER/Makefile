CXX=g++ -std=c++11
CPPFLAGS=-g -Wall
OBJFLAGS=-g -Wall -c

all: variable.o expression.o Interpreter

variable.o: variable.cpp variable.hpp
	$(CXX) $(OBJFLAGS) variable.cpp

expression.o: expression.cpp expression.hpp variable.hpp
	$(CXX) $(OBJFLAGS) expression.cpp

Interpreter: variable.o expression.o
	$(CXX) $(CPPFLAGS) Interpreter.cpp variable.o expression.o -o Interpreter

clean:
	rm *.o Interpreter
