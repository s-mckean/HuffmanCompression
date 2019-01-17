#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <string>
#include <list>
#include "node.h"
using namespace std;

string ReadToEncode(string filename, Node* lookUp[], list<Node*> *freqTable);
string ReadToDecode(string filename, list<Node*> *freqTable, Node* lookUp[]);
bool WriteToFile(string filename, string toWrite);
void AddLetterNode(list<Node*> *freqTable, Node* lookUp[], char letterToAdd);
void FormatTable(list<Node*> *freqTable, Node* lookUp[]);
void CreateTree(list<Node*> *freqTable);
string PrintTable(Node *topOfTree);
void Encode(string enString, Node *node);
string WriteEncodedStr(Node* lookUp[], string strToEncode);
string Decompress(Node* topOfTree, string toDecompress);

#endif FILECONTROLLER_H
