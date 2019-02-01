/*********************************************************************************************************
Author: Spencer McKean

Description: main

A program that takes a text file input and implements Huffman encoding on it.
Using the letters' encoding it will compress the file into another text file, __compressed.txt
The program will then decompress __compressed.txt to __decompressed.txt. 
__decompressed.txt will be identical to the text file that the user input.

Notes: Behavior becomes unpredictable when using non-ASCII character file encoding.
*********************************************************************************************************/

#include <iostream>
#include "compression.h"
#include "decompression.h"
using namespace std;

int main() {
	string userIn;

	cout << "What file would you like to compress? " << endl;
	cin >> userIn;

	Compress(userIn);
	Decompress();

	system("PAUSE");
}