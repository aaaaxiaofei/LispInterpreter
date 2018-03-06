#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "tokenizer.hpp"
#include "sexp.hpp"
#include "parser.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc < 2) {
		cout << "Please give an input file name" << endl;
		return 0;
	}

	// Token Object
	Tokenizer token;

	// Open file
	ifstream myfile;
	myfile.open(argv[1]);

	// Read and Tokenize
	string line;
	while (!myfile.eof()) {
		getline(myfile, line);
		token.tokenize(line);
	}

	// Parse the tokens and Output
	Parser par;
	par.parse(token);

	return 0;
}

