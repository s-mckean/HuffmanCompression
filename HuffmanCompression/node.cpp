#include <iostream>
#include "node.h"

Node::Node(char sym) {
	letter = sym;
}

void Node::SetChildren(Node * lc, Node * rc)
{
	lchild = lc;
	rchild = rc;
}

void Node::SetLetter(char sym)
{
	letter = sym;
}

void Node::SetBitString(string bs)
{
	bitString = bs;
}

void Node::AddFrequency(int i) {
	freq += i;
}

void Node::SetIsEOF(bool b)
{
	isEOF = true;
}


char Node::GetLetter()
{
	return letter;
}

int Node::GetFrequency()
{
	return freq;
}

bool Node::GetIsEOF()
{
	return isEOF;
}

string Node::GetBitString()
{
	return bitString;
}

Node* Node::GetNode(string name) {
	if (name == "LEFTCHILD") {
		return lchild;
	}
	else if (name == "RIGHTCHILD") {
		return rchild;
	}
	else {
		cout << "Not a valid child name." << endl;
		return NULL;
	}
}
