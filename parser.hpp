#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "tokenizer.hpp"
#include "sexp.hpp"
#include <fstream>
#include <unordered_map>

using namespace std;

class Parser {
private:
	ofstream myfile;

	SExp* parseDot(Tokenizer& token);
	SExp* parseList(Tokenizer& token);
	void errorMsg(Tokenizer& token);
	void printSexp(SExp* root);
	SExp* evaluate(SExp* root);
	SExp* defun(SExp* root);
	SExp* evalFunc(SExp* root);
	SExp* substitute(SExp* root, unordered_map<string, SExp*>& mp, unordered_map<SExp*, SExp*>& mp2);
	SExp* subback(SExp* root, unordered_map<int, SExp*>& mp);
	SExp* subback2(SExp* root, unordered_map<SExp*, SExp*>& mp);


	unordered_map<string, SExp*> alist;
	unordered_map<string, SExp*> dlist;

public:
	Parser();
	void parse(Tokenizer& token);


};

#endif