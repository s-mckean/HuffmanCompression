/*********************************************************************************************************
Author: Spencer McKean

Description: constructor, destructor, getters, and setters for Node class
*********************************************************************************************************/

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

Node* Node::GetChild(string name) {
	if (name == "LEFT") {
		return lchild;
	}
	else if (name == "RIGHT") {
		return rchild;
	}
	else {
		cout << "Not a valid child name." << endl;
		return NULL;
	}
}

Node::~Node() {
	if (GetChild("LEFTCHILD") != NULL) {
		delete GetChild("LEFTCHILD");
	}
	if (GetChild("RIGHTCHILD") != NULL) {
		delete GetChild("RIGHTCHILD");
	}
}
