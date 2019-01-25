/*********************************************************************************************************
Author: Spencer McKean

Description: Header for controller.cpp
*********************************************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <list>
#include "node.h"
using namespace std;

/*Main Algorithms*/

void Compress(string filename);
void Decompress();

/*File Manipulations*/

string ReadToCompress(string filename, Node* lookUp[], list<Node*> *freqTable);
string ReadToDecompress(string filename, list<Node*> *freqTable, Node* lookUp[]);
bool WriteToFile(string filename, string toWrite);

/*Tree Traversals*/

void AddNode(list<Node*> *freqTable, Node* lookUp[], char letterToAdd);
void FormatList(list<Node*> *freqTable, Node* lookUp[]);
void CreateTree(list<Node*> *freqTable);
void AssignBits(string enString, Node *node);

/*Converted String Creators*/

string CreateFreqToFile(Node *topOfTree);
string CreateCompressedStr(Node* lookUp[], string strToEncode);
string DecompressStr(Node* topOfTree, string toDecompress);

#endif CONTROLLER_H
