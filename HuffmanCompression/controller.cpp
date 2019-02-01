/*********************************************************************************************************
Author: Spencer McKean

Description: functions required for both compression and decompresssion of Huffman encoding
*********************************************************************************************************/

#include "contoller.h"
#include <fstream>
#include <iostream>
#include <list>

/*Write an input string to a file*/
bool WriteToFile(string filename, string toWrite) {
	ofstream file(filename, ios::out | ios::binary);
	if (!file) {
		cout << "Invalid file name." << endl;

		return false;
	}
	else {
		file << toWrite;

		file.close();

		return true;
	}
}

/*Adds found symbol to lookUp array and found-symbol list*/
void AddNode(list<Node*> *freqTable, Node* lookUp[], char letterToAdd) {
	int sizeOf = freqTable->size();
	bool found = false;
	list<Node*>::iterator it;

	for (it = freqTable->begin(); it != freqTable->end(); ++it) {
		if ((*it)->GetLetter() == letterToAdd) {
			found = true;
			break;
		}
	}
	if (found) { //if the symbol has already been found add 1 frequency to count
		(*it)->AddFrequency(1);
	}
	else { //if not then create a new node
		Node *n = new Node(letterToAdd);
		lookUp[letterToAdd] = n; //add node to loookUp at its ASCII value location
		freqTable->push_back(n); //add node to list of found symbols
		freqTable->back()->AddFrequency(1);
	}
	found = false;
}

/*Presorts and adds EOF node to list of found symbols*/
/*Sorts the list twice to ensure that the order is consistant*/
void FormatList(list<Node*> *freqTable, Node* lookUp[]) {
	Node *node = new Node(NULL);
	node->AddFrequency(1);
	node->SetIsEOF(true);

	lookUp[256] = node;

	freqTable->sort([](Node* a, Node* b) { return a->GetLetter() < b->GetLetter(); });

	freqTable->push_front(node);

	freqTable->sort([](Node* a, Node* b) { return a->GetFrequency() < b->GetFrequency(); });
}

/*Pops nodes off 2 at a time to create a binary tree*/
void CreateTree(list<Node*> *freqTable) {
	list<Node*>::iterator it;

	while (freqTable->size() > 1) {
		Node *lchild = freqTable->front();
		freqTable->erase(freqTable->begin());
		Node *rchild = freqTable->front();
		freqTable->erase(freqTable->begin());

		/*Create and add a new node with left and right child's combined frequencies*/
		Node *n = new Node(NULL); 
		n->AddFrequency(lchild->GetFrequency() + rchild->GetFrequency());
		n->SetChildren(lchild, rchild);

		if (freqTable->size() == 0) {
			freqTable->push_back(n);
		}
		else {
			for (it = freqTable->begin(); it != freqTable->end(); ++it) {
				if ((*it)->GetFrequency() >= n->GetFrequency()) {
					freqTable->insert(it, n); //Insert new node at its sorted position
					break;
				}
			}
			if (it == freqTable->end()) {
				freqTable->push_back(n);
			}
		}
	}
}

/*Traverse tree recursively to assign a unique bit string to each letter*/
void AssignBits(string enString, Node *topOfTree) {
	if (topOfTree->GetLetter() != NULL || topOfTree->GetIsEOF()) {
		topOfTree->SetBitString(enString);
	}
	else {
		AssignBits(enString + "0", topOfTree->GetChild("LEFT"));
		AssignBits(enString + "1", topOfTree->GetChild("RIGHT"));
	}
}