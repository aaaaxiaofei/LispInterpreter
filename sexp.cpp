#include "sexp.hpp"
#include <string>
#include <iostream>
using namespace std;

SExp::SExp() {
	type = 0;
	val = -123456;
	this->left = NULL;
	this->right = NULL;
}

SExp::SExp(int i) {
	type = 4;
	val = i;
	name = "";
	this->left = NULL;
	this->right = NULL;
}

SExp::SExp(string s) {
	type = 5;
	name = s;
	val = -123456;
	this->left = NULL;
	this->right = NULL;
}
