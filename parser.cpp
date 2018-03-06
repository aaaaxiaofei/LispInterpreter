#include "parser.hpp"

Parser::Parser() {

}

void Parser::parse(Tokenizer& token) {

	// Clear file first
	myfile.open("output.txt", std::ofstream::out | std::ofstream::trunc);
	myfile.close();

	// Open file
	myfile.open ("output.txt", ios::out | ios::app | ios::binary);

	while (token.hasNext()) {
		SExp* root = parseDot(token);
		if (root != NULL && (token.ckNextToken() == 6 || token.ckNextToken() == 8)) {
			printSexp(root);
			myfile << endl;
		}
		else {
			errorMsg(token);
			while (token.hasNext() && token.ckNextToken() != 6) token.skipToken();
		}
		if (token.ckNextToken() == 8) break; // "$$" termination
		token.skipToken();
	}

	myfile.close();
}

SExp* Parser::parseDot(Tokenizer& token) {
	
	if (!token.hasNext()) return NULL;

	SExp* root = new SExp(0);

	int nextToken = token.ckNextToken();

	if (nextToken == 2 || nextToken == 3 || nextToken >= 6) {
		// errorMsg(token);
		return NULL;
	}
	else if (nextToken == 1) {
		token.skipToken(); // skip the "("
		nextToken = token.ckNextToken();

		// Return NIL if nothing inside "()"
		if (nextToken == 2) { // ")"
			SExp* s = new SExp("NIL");
			token.skipToken();
			return s;
		}

		// Get CAR
		SExp* s1 = parseDot(token); 
		if (s1 == NULL) return NULL;

		// Get CDR
		SExp* s2 = NULL;

		// Check list or dot notation
		nextToken = token.ckNextToken();
		if (nextToken == 3) {
			token.skipToken(); // skip the "."
			s2 = parseDot(token);
			if (s2 == NULL) return NULL;	

			if (token.ckNextToken() == 2) {
				token.skipToken(); // skip the ")"		
			}
			else {
				// errorMsg(token);
				return NULL;
			}
		}
		else {
			s2 = parseList(token);
			if (s2 == NULL) return NULL;
		}

		root->left = s1;
		root->right = s2;
		return root;
	}
	else if (nextToken == 4) {
		int i = token.getIntToken();
		SExp* s = new SExp(i);
		return s;
	}
	else if (nextToken == 5) {
		string name = token.getNextToken();
		SExp* s = new SExp(name);
		return s;
	}

	return NULL;
}

SExp* Parser::parseList(Tokenizer& token) {
	
	if (!token.hasNext()) return NULL;

	SExp* root = new SExp(0);

	int nextToken = token.ckNextToken();

	if (nextToken == 2) {
		SExp* s = new SExp("NIL");
		token.skipToken();
		return s;
	}
	else if (nextToken == 3) {
		return NULL;
	}
	else if (nextToken == 1) {
		SExp* s1 = parseDot(token); 
		SExp* s2 = parseList(token);

		root->left = s1;
		root->right = s2;
		return root;

	}
	else if (nextToken == 4 || nextToken == 5) {

		SExp* s1 = parseDot(token); 
		SExp* s2 = parseList(token);

		if (s2 == NULL) return NULL;

		root->left = s1;
		root->right = s2;
		return root;
	}
	else if (nextToken >= 6) {
		return NULL;
	}

	return NULL;
}

void Parser::errorMsg(Tokenizer& token) {
	if (token.ckNextToken() == 1) {
		myfile << "== error: unexpected '(' ==" << endl;
	}
	else if (token.ckNextToken() == 2) {
		myfile << "== error: unexpected ')' ==" << endl;
	}
	else if (token.ckNextToken() == 3) {
		myfile << "== error: unexpected '.' ==" << endl;
	}
	else if (token.ckNextToken() == 4) {
		myfile << "== error: unexpected number ==" << endl;
	}
	else if (token.ckNextToken() == 5) {
		myfile << "== error: unexpected variable ==" << endl;
	}
	else if (token.ckNextToken() == 6) {
		myfile << "== error: unexpected '$' ==" << endl;
	}
	else if (token.ckNextToken() == 7) {
		myfile << "== error: unexpected '$' ==" << endl;
		token.skipToken();
	}
	else {
		myfile << "== error: illegal character ==" << endl;
	}
}

void Parser::printSexp(SExp* root) {
	if (root == NULL) {
		return;
	}

	if (root->left || root->right) {
		myfile << "(";
		printSexp(root->left);
		myfile << ".";
		printSexp(root->right);
		myfile << ")";
	}
	else {
		// Left node
		if (root->type == 4) {
			myfile << root->val;
		}
		else if (root->type == 5) {
			myfile << root->name;
		}
	}
}