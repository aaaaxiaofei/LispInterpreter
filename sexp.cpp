#include "sexp.hpp"
#include <string>
#include <iostream>
using namespace std;

SExp::SExp() {
	this->left = NULL;
	this->right = NULL;
}

SExp::SExp(int i) {
	type = 4;
	val = i;
	this->left = NULL;
	this->right = NULL;
}

SExp::SExp(string s) {
	type = 5;
	name = s;
	val = -1;
	this->left = NULL;
	this->right = NULL;
}
