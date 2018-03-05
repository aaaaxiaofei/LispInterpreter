#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "tokenizer.hpp"
#include "sexp.hpp"

using namespace std;

SExp* parser(Tokenizer& token);

void printSexp(SExp* root) {
	if (root == NULL) {
		return;
	}

	if (root->left || root->right) {
		cout << "(";
		printSexp(root->left);
		cout << ".";
		printSexp(root->right);
		cout << ")";
	}
	else {
		// Left node
		if (root->type == 4) {
			cout << root->val;
		}
		else if (root->type == 5) {
			cout << root->name;
		}
	}
}


int main(int argc, char *argv[]) {

	string line;
	Tokenizer token;

	ifstream myfile;
	myfile.open("test1.txt");


	// Read and Tokenize
	int i = 1;
	while (!myfile.eof()) {
		getline(myfile, line);
		token.tokenize(line);
	}


	token.print();
	// while (token.hasNext()) {
	// 	// cout << "see cur and size: " << token.getCur() << " vs " << token.getSize() << endl;
	// 	int next = token.ckNextToken();
	// 	cout <<  next << ", " << token.getNextToken() << endl;
	// }


	// Parser

	vector<SExp*> result;

	while (token.hasNext()) {
		SExp* root = parser(token);
		if (root != NULL && token.ckNextToken() == 6) {
			result.push_back(root);
		}
		else {
			while (token.hasNext() && token.ckNextToken() != 6) token.skipToken();
		}
		token.skipToken();
	}

	cout << "===== output s-expression =====" << endl;
	// Output
	for (int i = 0; i < result.size(); i++) {
		printSexp(result[i]);
		cout << endl;
	}

	return 0;
}


SExp* parser(Tokenizer& token) {
	
	// cout << "token is: " << token.ckNextToken() << endl;
	if (!token.hasNext()) return NULL;

	SExp* root = new SExp(0);

	int nextToken = token.ckNextToken();

	if (nextToken == 2 || nextToken == 3) {
		cout << "Illegal s-expression" << endl;
		token.skipToken();
	}

	if (nextToken == 1) {
		token.skipToken(); // skip the "("
		SExp* s1 = parser(token); 
		if (s1 == NULL) return NULL;

		nextToken = token.ckNextToken();
		if (nextToken != 3) {
			cout << "Should be Dot here" << endl;
			return NULL;
		}
		
		token.skipToken(); // skip the "."

		SExp* s2 = parser(token);

		if (s2 == NULL) return NULL;

		token.skipToken(); // skip the ")"

		root->left = s1;
		root->right = s2;
		return root;
	}

	if (nextToken == 4) {
		int i = token.getIntToken();
		SExp* s = new SExp(i);
		return s;
	}

	if (nextToken == 5) {
		string name = token.getNextToken();
		SExp* s = new SExp(name);
		return s;
	}

	return NULL;

}