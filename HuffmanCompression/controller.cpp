/*********************************************************************************************************
Author: Spencer McKean

Description: functions for required data manipulation in order to apply Huffman encoding
*********************************************************************************************************/

#include "contoller.h"
#include <fstream>
#include <iostream>
#include <list>
#include <bitset>
#include <regex>
#include <sstream>

/*Read and compress an input file name*/
void Compress(string filename) {
	Node* lookUp[257]; // Array for bit string lookup 256 ASCII chars + 1 for EOF bit string
	list<Node*> *tree = new list<Node*>(); // List to store nodes that are created
	string readText, freqString, compressedText;

	readText = ReadToCompress(filename, lookUp, tree);

	CreateTree(tree);
	AssignBits("", tree->front());

	freqString = CreateFreqToFile(tree->front());
	compressedText = CreateCompressedStr(lookUp, readText);

	// first line is frequency counts and the rest of the file is compressed text
	WriteToFile("__compressed.txt", freqString + "\n" + compressedText);
	
	delete tree;
}

/*Read compressed file and outputs original text to file*/
void Decompress() {
	Node* lookUp[257]; // Array for bit string lookup 256 ASCII chars + 1 for EOF bit string
	list<Node*> *tree = new list<Node*>(); // List to store nodes that are created

	string readText = ReadToDecompress("__compressed.txt", tree, lookUp);

	CreateTree(tree);
	AssignBits("", tree->front());
	WriteToFile("__decompressed.txt", DecompressStr(tree->front(), readText));

	delete tree;
}

/*Reads file to prepare for compression*/
string ReadToCompress(string filename, Node* lookUp[], list<Node*> *freqTable) {
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
			AddNode(freqTable, lookUp, symbol);
		}

		file.close();

		FormatList(freqTable, lookUp);

		return readString;
	}
}

/*Reads file to prepare for decompression*/
string ReadToDecompress(string filename, list<Node*> *freqTable, Node* lookUp[]) {
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
		getline(file, freqs); // get first line of symbols and respective counts
		while (file >> noskipws >> symbol) {
			readString += symbol;
		}
	}

	file.close();

	/*Read first line in order to create new tree*/
	smatch match;
	string matchStr;
	int letterFreq;
	char letter;

	regex freqParse("^(.|new|\\s)([0-9]+),?"); //new for '\n' so all counts on first line
	string::const_iterator begin = freqs.begin();
	string::const_iterator end = freqs.end();
	match_results<string::const_iterator> matches;

	while (regex_search(begin, end, matches, freqParse)) {
		string firstMatch = string(matches[1].first, matches[1].second);
		letter = (firstMatch == "new" ? '\n' : firstMatch[0]);
		stringstream getFreq(string(matches[2].first, matches[2].second));
		getFreq >> letterFreq;

		AddNode(freqTable, lookUp, letter);
		freqTable->back()->AddFrequency(letterFreq - 1);

		begin = matches[2].second + 1;
	}

	FormatList(freqTable, lookUp);

	return readString;
}

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

/*Create a string that contains letters and their counts to write to the compressed file*/
string CreateFreqToFile(Node *topOfTree) {
	string freqToFile = "";
	if (topOfTree->GetLetter() != NULL) {
		return((topOfTree->GetLetter() == '\n' ? "new" : string(1, topOfTree->GetLetter())) + to_string(topOfTree->GetFrequency()) + ",");
	}
	else {
		if (topOfTree->GetChild("LEFT") != NULL) {
			freqToFile += CreateFreqToFile(topOfTree->GetChild("LEFT"));
		}
		if (topOfTree->GetChild("RIGHT") != NULL) {
			freqToFile += CreateFreqToFile(topOfTree->GetChild("RIGHT"));
		}
	}
	return freqToFile;
}

/*Use the unique bit strings of each letter to create the compressed text*/
string CreateCompressedStr(Node* lookUp[], string strToEncode) {
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
		else { //Add EOF bit string at the end
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
	if (bitcount != 0) {//fill the remainder of the byte
		while (bitcount != 8) {
			currbyte <<= 1;
			bitcount++;
		}
		enString += currbyte;
	}

	return enString;
}

/*Read bits until the EOF bit string is found*/
string DecompressStr(Node *topOfTree, string toDecompress) {
	string decomprsStr = "";
	Node* currentNode = topOfTree;
	int sizeOf = toDecompress.size();

	for (int i = 0; i < sizeOf; i++) {
		bitset<8>byteToRead(toDecompress[i]);
		for (int j = byteToRead.size() - 1; j >= 0; j--) {
			if (byteToRead[j] == 0) {
				currentNode = currentNode->GetChild("LEFT");
			}
			else if (byteToRead[j] == 1) {
				currentNode = currentNode->GetChild("RIGHT");
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