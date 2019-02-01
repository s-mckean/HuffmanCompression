/*********************************************************************************************************
Author: Spencer McKean

Description: Header for decompresssion.cpp
*********************************************************************************************************/

#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include "controller.h"

/*Function to be called in order to decompress a file*/
void Decompress();

/*Manipulate file for decompression*/
string ReadToDecompress(string filename, list<Node*> *freqTable, Node* lookUp[]);

/*Creates decompressed string*/
string DecompressStr(Node* topOfTree, string toDecompress);

#endif DECOMPRESSION_H