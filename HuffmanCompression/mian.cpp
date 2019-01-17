#include <iostream>
#include "fileContoller.h"
using namespace std;

void DeleteTreeNodes(Node* topOfTree);

int main() {
	char userIn;
	list<Node*> *tree = new list<Node*>();
	bool loop = true, validSave = false;
	Node* encodeLookUp[257];
	Node* decodeLookUp[257];
	string original, encoded, fileToCompress, compressedFile, decompressedFile;// , encode, freqString;

	while (loop) {
		original = "";
		encoded = "";
		//string original, encoded, fileToCompress, compressedFile, decompressedFile;

		cout << "--------------------------\n"
				"What would you like to do?\n"
				"\n(C)ompress File\n"
				"(D)ecompress File\n"
				"(Q)uit" << endl;
		cin >> userIn;

		switch (userIn) {
			case 'C':
			case 'c':
				while (original == "") {
					cout << "What file would you like to compress? ";
					cin >> fileToCompress;
					original = ReadToEncode(fileToCompress, encodeLookUp, tree); //read file and sort based on freq
				}

				CreateTree(tree); //pop from list to create tree
				Encode("", tree->front());

				while (!validSave) {
					cout << "Where would you like to save the compression? " << endl;
					cin >> compressedFile;
					validSave = WriteToFile(compressedFile, PrintTable(tree->front()) + "\n" + WriteEncodedStr(encodeLookUp, original)); //write encoded string and tree to file
				}

				//original = "";
				//compressedFile = "";
				//fileToCompress = "";

				break;

			case 'D':
			case 'd':
				while (encoded == "") {
					cout << "What file would you like to decompress? ";
					cin >> compressedFile;
					encoded = ReadToDecode(compressedFile, tree, decodeLookUp);
				}
				CreateTree(tree);
				Encode("", tree->front());
				//PrintTable(tree->front());

				while (!validSave) {
					cout << "Where would you like to save the decompression?" << endl;
					cin >> decompressedFile;
					validSave = WriteToFile(decompressedFile, Decompress(tree->front(), encoded));
				}

				//encoded = "";

				break;

			case 'Q':
			case 'q':
				loop = false;
				break;

			default:
				cout << "Invalid input" << endl;
				break;
		}

		/*original = "";
		encoded = "";/*
		fileToCompress = "";
		compressedFile = "";
		decompressedFile = "";*/

		validSave = false;

		cout << endl;

		if (tree->size() != 0) {
			DeleteTreeNodes(tree->front());
		}
	}
	delete tree;
}

void DeleteTreeNodes(Node* topOfTree) {
	Node* currentNode = topOfTree;
	if (currentNode->GetNode("LEFTCHILD") != NULL) {
		DeleteTreeNodes(currentNode->GetNode("LEFTCHILD"));
	}
	if (currentNode->GetNode("RIGHTCHILD") != NULL) {
		DeleteTreeNodes(currentNode->GetNode("RIGHTCHILD"));
	}
	delete currentNode;
}