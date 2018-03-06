#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "tokenizer.hpp"
#include "sexp.hpp"
#include <fstream>

using namespace std;

class Parser {
private:
	ofstream myfile;

	SExp* parseDot(Tokenizer& token);
	SExp* parseList(Tokenizer& token);
	void errorMsg(Tokenizer& token);
	void printSexp(SExp* root);

public:
	Parser();
	void parse(Tokenizer& token);


};

#endif