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
	int sign = 1;
	int num = 0;
	int i = 0;

	if (str[0] == '+') i++;
	else if (str[0] == '-') {
		sign = -1;
		i++;
	}
	 
	for (; i < str.size(); i++) {
		char c = str[i];
		num += num * 10 + (c - '0');
	}
	return sign * num;
}

void Tokenizer::skipToken() {
	cur++;
}

bool Tokenizer::hasNext() {
	return cur < tokens.size();
}

void Tokenizer::tokenize(string line) {

	// Tokenize line with only "$"
	int numChars = 0;
	string str = "";
	for (int i = 0; i < line.size(); i++) {
		char c = line[i];
		if (c != ' ' && c != '\t') {
			str += c;
		}
	}
	if (str == "$" || str == "$$") {
		tokens.push_back(str);
		return;
	}

	// Tokenize other cases
	int i = 0; 
	while (i < line.length()) {
		if (line[i] == ' ' || line[i] == '\t') i++;
		else if (isPunct(line[i])) {
			tokens.push_back(line.substr(i++, 1));
			if (tokens.back() == "$") {
				tokens.back() = "#";
				break;
			}
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
	string str = tokens[cur];

	if (str == "(") return 1;											// Left parenthesis (
	else if (str == ")") return 2;										// Right Parenthesis )
	else if (str == ".") return 3;										// Dot .
	else if (isdigit(str[0]) || str[0] == '+' || str[0] == '-') {		// Integer
		// Only one char '+' or '-'
		if (str.size() == 1 && !isdigit(str[0])) return 7;				// Illegal input

		// Check non-digit char in the string
		for (int i = 1; i < str.size(); i++) {
			if (!isdigit(str[i])) return 7;								// Illegal input
		}
		// Return integer
		return 4;	
	}
	else if (isalpha(str[0])) {
		for (int i = 0; i < str.size(); i++) {
			char c = str[i];
			if (!isdigit(c) && !isalpha(c)) return 7; 					// Illegal input
		}
		return 5; 														// Character
	}	
	else if (str == "$") return 6;										// $
	else if (str == "#") return 7;										// inline $
	else if (str == "$$") return 8;										// inline $
	return 9;															// Any other illegal input
}