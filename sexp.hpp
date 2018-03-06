#ifndef SEXP_HPP
#define SEXP_HPP

#include <string>
using namespace std;

class SExp {

public:
	int type;
	int val;
	string name;
	SExp* left;
	SExp* right;

	SExp();
	SExp(int i);
	SExp(string s);
};

#endif