#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

class Node {
	private:
		Node *lchild = NULL;
		Node *rchild = NULL;
		Node *parent = NULL;
		int freq = 0;
		string bitString = "";
		char letter = '\0';
		bool isEOF = false;

	public:
		Node(char sym);

		void SetChildren(Node *lc, Node *rc);
		void SetLetter(char sym);
		void SetBitString(string bs);
		void AddFrequency(int i);
		void SetIsEOF(bool b);


		char GetLetter();
		int GetFrequency();
		bool GetIsEOF();
		string GetBitString();
		Node* GetNode(string name);
};

#endif NODE_H