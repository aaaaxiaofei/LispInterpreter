#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Tokenizer {

private:
	vector<string> tokens; 
	int cur;

	bool isPunct(char c) {
		return c == '(' || c == ')' || c == '$' || c == '.';
	}

public:

	Tokenizer();

	int getSize(); 
	int getCur();
	string getNextToken();
	int getIntToken();
	bool hasNext();
	
	void tokenize(string line);
	int ckNextToken();
	void skipToken();
	string thisToken();

};

#endif