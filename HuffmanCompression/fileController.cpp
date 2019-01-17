#include "fileContoller.h"
#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <regex>
#include <sstream>


string ReadToEncode(string filename, Node* lookUp[], list<Node*> *freqTable) {
	string readString = "";
	char symbol;

	ifstream file(filename, ios::in | ios::binary);
	
	if (!file) {
		cout <<  filename << " could not be found\n" << endl;
		return "";
	}

	else {
		cout << "Loading " << filename << " for encoding" << endl;
		while (file >> noskipws >> symbol) {
			readString += symbol;
			AddLetterNode(freqTable, lookUp, symbol);
		}

		file.close();

		FormatTable(freqTable, lookUp);

		return readString;
	}
}

void FormatTable(list<Node*> *freqTable, Node* lookUp[]) {
	Node *node = new Node(NULL);
	node->AddFrequency(1);
	node->SetIsEOF(true);

	lookUp[256] = node;

	freqTable->sort([](Node* a, Node* b) { return a->GetLetter() < b->GetLetter(); });

	freqTable->push_front(node);

	freqTable->sort([](Node* a, Node* b) { return a->GetFrequency() < b->GetFrequency(); });
}

void AddLetterNode(list<Node*> *freqTable, Node* lookUp[], char letterToAdd) {
	int sizeOf = freqTable->size();
	bool found = false;
	list<Node*>::iterator it;

	for (it = freqTable->begin(); it != freqTable->end(); ++it) {
		if ((*it)->GetLetter() == letterToAdd) {
			found = true;
			break;
		}
	}
	if (found) {
		(*it)->AddFrequency(1);
	}
	else {
		Node *n = new Node(letterToAdd);
		lookUp[letterToAdd] = n;
		freqTable->push_back(n);
		freqTable->back()->AddFrequency(1);
	}
	found = false;
}

string ReadToDecode(string filename, list<Node*> *freqTable, Node* lookUp[]) {
	string readString = "";
	string freqs;
	char symbol;
	bool found = false;
	list<Node*>::iterator it;

	ifstream file(filename, ios::in | ios::binary);

	if (!file) {
		cout << filename << " could not be found\n" << endl;
	}

	else {
		cout << "Loading " << filename << " for decoding" << endl;
		getline(file, freqs); // get first line
		while (file >> noskipws >> symbol) {
			readString += symbol;
		}
	}

	file.close();

	smatch match;
	string matchStr;
	int letterFreq;
	char letter;
	regex freqParse("^(.|new|\\s)([0-9]+),?");
	string::const_iterator begin = freqs.begin();
	string::const_iterator end = freqs.end();
	match_results<string::const_iterator> matches;
	while (regex_search(begin, end, matches, freqParse)) {
		string firstMatch = string(matches[1].first, matches[1].second);
		letter = ( firstMatch == "new" ? '\n' : firstMatch[0]);
		stringstream getFreq(string(matches[2].first, matches[2].second));
		getFreq >> letterFreq;

		AddLetterNode(freqTable, lookUp, letter);
		freqTable->back()->AddFrequency(letterFreq - 1);

		begin = matches[2].second + 1;
	}

	FormatTable(freqTable, lookUp);

	return readString;
}

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

string WriteEncodedStr(Node* lookUp[], string strToEncode) {
	unsigned char currbyte = 0;
	int bitcount = 0;
	int bit = 0;
	string bitString = "";
	string enString = "";

	int sizeOfStr = strToEncode.length();
	int sizeOfBit = 0;

	for (int i = 0; i <= sizeOfStr; i++) {
		if (i < sizeOfStr) {
			bitString = lookUp[strToEncode[i]]->GetBitString();
		}
		else {
			bitString = lookUp[256]->GetBitString();
		}

		sizeOfBit = bitString.length();

		for (int j = 0; j < sizeOfBit; j++) {
			bit = bitString[j] - 48;
			currbyte = currbyte << 1 | bit;
			bitcount++;
			if (bitcount == 8) {
				enString += currbyte;
				currbyte = 0;
				bitcount = 0;
			}
		}
	}
	if (bitcount != 0) {
		while (bitcount != 8) {
			currbyte <<= 1;
			bitcount++;
		}
		enString += currbyte;
	}

	return enString;
}

void CreateTree(list<Node*> *freqTable) {
	list<Node*>::iterator it;

	while (freqTable->size() > 1) {
		Node *lchild = freqTable->front();
		freqTable->erase(freqTable->begin());
		Node *rchild = freqTable->front();
		freqTable->erase(freqTable->begin());

		Node *n = new Node(NULL);
		n->AddFrequency(lchild->GetFrequency() + rchild->GetFrequency());
		n->SetChildren(lchild, rchild);

		if (freqTable->size() == 0) {
			freqTable->push_back(n);
		}
		else {
			for (it = freqTable->begin(); it != freqTable->end(); ++it) {
				if ((*it)->GetFrequency() >= n->GetFrequency()) {
					freqTable->insert(it, n);
					break;
				}
			}
			if (it == freqTable->end()) {
				freqTable->push_back(n);
			}
		}
	}
}

string PrintTable(Node *topOfTree) {
	string freqToFile = "";
	if (topOfTree->GetLetter() != NULL) {
		return((topOfTree->GetLetter() == '\n' ? "new" : string(1, topOfTree->GetLetter())) + to_string(topOfTree->GetFrequency()) + ",");
	}
	else {
		if (topOfTree->GetNode("LEFTCHILD") != NULL) {
			freqToFile += PrintTable(topOfTree->GetNode("LEFTCHILD"));
		}
		if (topOfTree->GetNode("RIGHTCHILD") != NULL) {
			freqToFile += PrintTable(topOfTree->GetNode("RIGHTCHILD"));
		}
	}
	return freqToFile;
}

void Encode(string enString, Node *topOfTree) {
	if (topOfTree->GetLetter() != NULL || topOfTree->GetIsEOF()) {
		topOfTree->SetBitString(enString);
	}
	else {
		Encode(enString + "0", topOfTree->GetNode("LEFTCHILD"));
		Encode(enString + "1", topOfTree->GetNode("RIGHTCHILD"));
	}
}

string Decompress(Node *topOfTree, string toDecompress) {
	string decomprsStr = "";
	Node* currentNode = topOfTree;
	int sizeOf = toDecompress.size();

	for (int i = 0; i < sizeOf; i++) {
		bitset<8>byteToRead(toDecompress[i]);
		for (int j = byteToRead.size() - 1; j >= 0; j--) {
			if (byteToRead[j] == 0) {
				currentNode = currentNode->GetNode("LEFTCHILD");
			}
			else if (byteToRead[j] == 1) {
				currentNode = currentNode->GetNode("RIGHTCHILD");
			}

			if (currentNode->GetLetter() != NULL) {
				decomprsStr += currentNode->GetLetter();
				currentNode = topOfTree;
			}
			else if (currentNode->GetIsEOF()) {
				return decomprsStr;
			}
		}
	}

	cout << endl << "Didnt find EOF code." << endl;
	return decomprsStr;
}