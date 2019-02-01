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

/*function to write strings to file*/
bool WriteToFile(string filename, string toWrite);

/*Tree Traversals*/
void AddNode(list<Node*> *freqTable, Node* lookUp[], char letterToAdd);
void FormatList(list<Node*> *freqTable, Node* lookUp[]);
void CreateTree(list<Node*> *freqTable);
void AssignBits(string enString, Node *node);

#endif CONTROLLER_H
