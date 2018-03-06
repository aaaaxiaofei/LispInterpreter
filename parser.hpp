#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "tokenizer.hpp"
#include "sexp.hpp"

using namespace std;

class Parser {

public:
	Parser();
	void parse(Tokenizer& token);
	SExp* parseDot(Tokenizer& token);
	SExp* parseList(Tokenizer& token);
	void errorMsg(Tokenizer& token);
	void printSexp(SExp* root);

};

#endif