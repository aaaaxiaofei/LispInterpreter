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

			SExp* result = evaluate(root);
			printSexp(result);
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

	// // ****** Debug only, Please delete this block
	// if (root->left || root->right) {
	// 	cout << "(";
	// 	printSexp(root->left);
	// 	cout << ".";
	// 	printSexp(root->right);
	// 	cout << ")";
	// }
	// else {
	// 	if (root->type == 4) {
	// 		cout << root->val;
	// 	}
	// 	else if (root->type == 5) {
	// 		cout << root->name;
	// 	}
	// }
	// // ****** Debug only, Please delete this block

	if (root->left || root->right) {
		myfile << "(";
		printSexp(root->left);
		myfile << ".";
		printSexp(root->right);
		myfile << ")";
	}
	else {
		if (root->type == 4) {
			myfile << root->val;
		}
		else if (root->type == 5) {
			myfile << root->name;
		}
	}
}

SExp* Parser::evaluate(SExp* root) {
	if (root == NULL) {
		return NULL;
	}
	SExp *nodeL = root->left, *nodeR = root->right;
	SExp *boolNode = new SExp("T");
	

	// Left child is string
	if (nodeL && nodeL->type == 5) {

		string key = nodeL->name;
		// QUOTE
		if (key == "QUOTE") {
			if (nodeR && nodeR->right && nodeR->right->left) {
				SExp* errorNode = new SExp("Error, more than one arguments for QUOTE");
				return errorNode;
			}
			return nodeR->left;
		}
		// CAR
		else if (key == "CAR") {
			SExp* node = evaluate(nodeR->left);
			return node->left;
		}
		// CDR
		else if (key == "CDR") {
			SExp* node = evaluate(nodeR->left);
			return node->right;
		}
		// CONS
		else if (key == "CONS") {

			if (!nodeR->right || !nodeR->right->left) {
				SExp* errorNode = new SExp("Error, need more arguemnts for CONS");
				return errorNode;
			}
			if (nodeR->right && nodeR->right->right && nodeR->right->right->left) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}

			SExp* node = new SExp(0);
			SExp* node1 = evaluate(nodeR->left);
			SExp* node2 = evaluate(nodeR->right->left);
			
			node->left = node1;
			node->right = node2;
			return node;
		}
		// ATOM
		else if (key == "ATOM") {
			SExp* node = evaluate(nodeR->left);
			if (nodeR->right && nodeR->right->right) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}

			boolNode->name = !node->left ? "T" : "NIL";
			return boolNode;
		}
		// EQ
		else if (key == "EQ") {

			if (!nodeR->right || !nodeR->right->left) {
				SExp* errorNode = new SExp("Error, too few arguemnts");
				return errorNode;
			}
			if (nodeR->right && nodeR->right->right && nodeR->right->right->left) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}

			SExp* node1 = evaluate(nodeR->left);
			SExp* node2 = evaluate(nodeR->right->left);

			if (node1->left || node2->left) {
				SExp* errorNode = new SExp("Error, EQ can only be applied to two atomic arguments.");
				return errorNode;
			}

			boolNode->name = (!node1->left && !node2->left && node1->val == node2->val &&
				 			  node1->name == node2->name) ? "T" : "NIL";
			return boolNode;
		}
		// NULL
		else if (key == "NULL" || key == "NIL") {
			if (nodeR->right && nodeR->right->right) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}
			SExp* node = evaluate(nodeR->left);
			boolNode->name = (!node->left && node->name == "NIL") ? "T" : "NIL";
			return boolNode;
		}
		// T
		else if (key == "T") {
			if (nodeR->right && nodeR->right->right) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}
			SExp* node = evaluate(nodeR->left);
			boolNode->name = (!node->left && node->name == "T") ? "T" : "NIL";
			return boolNode;
		}
		// INT
		else if (key == "INT") {
			if (nodeR->right && nodeR->right->right) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}
			SExp* node = evaluate(nodeR->left);
			boolNode->name = (!node->left && node->type == 4) ? "T" : "NIL";
			return boolNode;
		}
		// Math Operation
		else if (key == "PLUS" || key == "MINUS" || key == "TIMES" || 
				 key == "QUOTIENT" || key == "REMAINDER") {


			if (!nodeR->right || !nodeR->right->left) {
				SExp* errorNode = new SExp("Error, too few arguemnts");
				return errorNode;
			}
			if (nodeR->right && nodeR->right->right && nodeR->right->right->left) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}


			SExp* node = new SExp(0);
			SExp* node1 = evaluate(nodeR->left);
			SExp* node2 = evaluate(nodeR->right->left);
			
			if (!node1->left && !node2->left && node1->type == 4 && node2->type == 4) {
				if (key == "PLUS") node->val = node1->val + node2->val;
				else if (key == "MINUS") node->val = node1->val - node2->val;
				else if (key == "TIMES") node->val = node1->val * node2->val;
				else if (key == "QUOTIENT") {
					if (node2->val != 0) {
						node->val = node1->val / node2->val;
					}
					else {
						SExp* errorNode = new SExp("Error, divide by zero");
						return errorNode;
					}
				}
				else if (key == "REMAINDER") {
					if (node2->val != 0) {
						node->val = node1->val % node2->val;
					}
					else {
						SExp* errorNode = new SExp("Error, mod by zero");
						return errorNode;
					}
				}
			}
			else {
				SExp* errorNode = new SExp("Error, operating on non-numeric variables");
				return errorNode;
			}
			return node;
		}
		// Compare
		else if (key == "LESS" || key == "GREATER") {
			if (!nodeR->right || !nodeR->right->left) {
				SExp* errorNode = new SExp("Error, too few arguemnts");
				return errorNode;
			}
			if (nodeR->right && nodeR->right->right && nodeR->right->right->left) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}


			SExp* node1 = evaluate(nodeR->left);
			SExp* node2 = evaluate(nodeR->right->left);
			
			if (!node1->left && !node2->left && node1->type == 4 && node2->type == 4) {
				if (key == "LESS") {
					boolNode->name = node1->val < node2->val ? "T" : "NIL";
				}
				else if (key == "GREATER") {
					boolNode->name = node1->val > node2->val ? "T" : "NIL";					
				}
			}
			else {
				SExp* errorNode = new SExp("Error, comparing on non-numeric variables");
				return errorNode;
			}
			return boolNode;
		}
		// COND
		else if (key == "COND") {
			SExp* node = nodeR;

			if (!node->left || !node->left->left || !node->left->right->left) {
				SExp* errorNode = new SExp("Error, too few arguemnts");
				return errorNode;
			}
			else if (node->left->right && node->left->right->right && node->left->right->right->left) {
				SExp* errorNode = new SExp("Error, too many arguemnts");
				return errorNode;
			}

			while (node->left) {

				if (!node->left || !node->left->left || !node->left->right->left) {
					SExp* errorNode = new SExp("Error, too few arguemnts");
					return errorNode;
				}
				else if (node->left->right && node->left->right->right && node->left->right->right->left) {
					SExp* errorNode = new SExp("Error, too many arguemnts");
					return errorNode;
				}
				SExp* node1 = evaluate(node->left->left);
				if (node1->name == "T" || (node1->type == 4 && node1->val != 0)) return evaluate(node->left->right->left);
				node = node->right;
			}
			return node;
		}
		// DEFUN
		else if (key == "DEFUN") {
			return defun(root);
		}
		// Existing Function
		else if (alist.count(key) > 0) {
			return evalFunc(root);
		}
		// else {
		// 	SExp* errorNode = new SExp("Error, undefined function");
		// 	return errorNode;
		// }
	}
	else if (nodeL && nodeL->type == 4) {
		SExp* errorNode = new SExp("Error - Arguments for function application are not a valid list.");
		errorNode->type = -1;
		return errorNode;
	}
	// else if (!nodeL && root->type == 5 && root->name != "NIL" && root->name != "T") {
	// 	SExp* errorNode = new SExp("Error - Unbound Atom.");
	// 	errorNode->type = -1;
	// 	return errorNode;
	// }
	else {
		return root;
	}

	return NULL;
}

SExp* Parser::defun(SExp* root) {

	SExp *nodeL = root->left, *nodeR = root->right;
	SExp *func = evaluate(nodeR->left);

	if (func) {
		string key = func->name;
		alist[key] = nodeR->right->left;
		dlist[key] = nodeR->right->right->left;
	}

	return func;
}

SExp* Parser::evalFunc(SExp* root) {

	unordered_map<string, SExp*> mp;
	unordered_map<int, SExp*> mp2;

	unordered_map<SExp*, SExp*> mp3;

	SExp *nodeL = root->left, *nodeR = root->right;
	string key = nodeL->name;

	SExp* value = nodeR;
	SExp* variable = alist[key];

	while (value->left) {
		mp[variable->left->name] = value->left;
		mp2[value->left->val] = variable->left;

		// cout << "map: " << variable->left->name << ", " << value->left->val << endl;
		variable = variable->right;
		value = value->right;
	}

	// substitute
	SExp* exp = dlist[key];
	
	exp = substitute(exp, mp, mp3);
	// return exp;
	SExp* result = evaluate(exp);

	// return result;

	return result;
}

SExp* Parser::substitute(SExp* root, unordered_map<string, SExp*>& mp, unordered_map<SExp*, SExp*>& mp2) {

	if (root) {
		if (mp.count(root->name) > 0) {
			mp2[mp[root->name]] = root;
			return mp[root->name];
		}
		root->left = substitute(root->left, mp, mp2);
		root->right = substitute(root->right, mp, mp2);
	}
	return root;
}

SExp* Parser::subback(SExp* root, unordered_map<int, SExp*>& mp) {

	if (root) {
		if (mp.count(root->val) > 0) {
			return mp[root->val];
		}
		root->left = subback(root->left, mp);
		root->right = subback(root->right, mp);
	}

	return root;
}

SExp* Parser::subback2(SExp* root, unordered_map<SExp*, SExp*>& mp) {

	if (root) {
		if (mp.count(root) > 0) {
			root = mp[root];
		}
		root->left = subback2(root->left, mp);
		root->right = subback2(root->right, mp);
	}

	return root;
}