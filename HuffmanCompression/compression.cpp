/*********************************************************************************************************
Author: Spencer McKean

Description: functions required for compression using Huffman encoding
*********************************************************************************************************/

#include "compression.h"
#include <fstream>
#include <iostream>
using namespace std;


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

/*Reads file to prepare for compression*/
string ReadToCompress(string filename, Node* lookUp[], list<Node*> *freqTable) {
	string readString = "";
	char symbol;

	ifstream file(filename, ios::in | ios::binary);

	if (!file) {
		cout << filename << " could not be found\n" << endl;
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