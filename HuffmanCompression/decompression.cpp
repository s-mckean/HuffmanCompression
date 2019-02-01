/*********************************************************************************************************
Author: Spencer McKean

Description: functions required for the decompresssion using Huffman encoding
*********************************************************************************************************/

#include "decompression.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <regex>
#include <sstream>
using namespace std;


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