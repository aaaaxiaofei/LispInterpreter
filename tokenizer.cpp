#include "tokenizer.hpp"

Tokenizer::Tokenizer() {
	cur = 0;
}

int Tokenizer::getSize() {
	return tokens.size();
}

int Tokenizer::getCur() {
	return cur;
}

string Tokenizer::getNextToken() {
	return tokens[cur++];
}

string Tokenizer::thisToken() {
	return tokens[cur];
}

int Tokenizer::getIntToken() {
	string str = tokens[cur++];
	int num = 0; 
	for (char c : str) {
		num += num * 10 + (c - '0');
	}
	return num;
}

void Tokenizer::skipToken() {
	cur++;
}

bool Tokenizer::hasNext() {
	return cur < tokens.size();
}

void Tokenizer::print() {
	for (string token : tokens) {
		cout << token << ' ';
		if (token == "$") cout << endl;
	}
}

void Tokenizer::tokenize(string line) {
	int i = 0; 
	while (i < line.length()) {
		if (line[i] == ' ' || line[i] == '\t') i++;
		else if (isPunct(line[i])) {
			tokens.push_back(line.substr(i++, 1));
			// need to take care of '$$' case!!
		}
		else {
			string str = "";
			while (i < line.size() && line[i] != ' ' && !isPunct(line[i])) {
				str += line[i++];
			}
			tokens.push_back(str);
		}
	}
}


int Tokenizer::ckNextToken() {
	
	if (!hasNext()) return 0;	// Reach the end, no further token.

	if (tokens[cur] == "(") return 1;			// Left parenthesis (
	else if (tokens[cur] == ")") return 2;		// Right Parenthesis )
	else if (tokens[cur] == ".") return 3;		// Dot .
	else if (isdigit(tokens[cur][0])) return 4;	// Integer
	else if (tokens[cur] == "$") return 6;		// $
	return 5;
}