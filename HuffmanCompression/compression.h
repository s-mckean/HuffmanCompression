/*********************************************************************************************************
Author: Spencer McKean

Description: Header for compresssion.cpp
*********************************************************************************************************/

#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "contoller.h"

/*Function to be called in order to compress a file*/
void Compress(string filename);

/*Manipulate file for compresssion*/
string ReadToCompress(string filename, Node* lookUp[], list<Node*> *freqTable);

/*Create strings to write to compresssed file*/
string CreateFreqToFile(Node *topOfTree);
string CreateCompressedStr(Node* lookUp[], string strToEncode);

#endif COMPRESSION_H
