# Huffman Compression

Author: Spencer McKean

Project Start Date: 	 12/7/18
Project Completion Date: 12/18/18
Restructured on 2/1/19

## Description

A program that takes a text file input and implements Huffman encoding on it.
Using the letters' encoding it will compress the file into another text file, __compressed.txt.
The program will then decompress __compressed.txt to __decompressed.txt. 
__decompressed.txt will be identical to the text file that the user input.



Notes: Behavior becomes unpredictable when using non-ASCII character file encoding.

Included are some sample input files: data1.txt, data2.txt, data3.txt, data4.txt, data5.txt

When prompted the user must enter in the full name of the text file they wish to compress.


An image of example user input:

![Image](https://user-images.githubusercontent.com/42820224/51720024-add4bd00-2000-11e9-902e-19800339786c.PNG)

## File Structure:

README.md
HuffmanCompression.sln
HuffmanCompression(folder):

	-> data1.txt - sample input file
	-> data2.txt - sample input file
	-> data3.txt - sample input file
	-> data4.txt - sample input file
	-> data5.txt - sample input file
	
	-> __compression.txt  - output file with compressed version of the input file
	-> __decompressed.txt - output file of decompressed version of __compression.txt

	-> node.h		- header for node.cpp
	-> controller.h		- header for controller.cpp
	-> compression.h	- header for compression.cpp
	-> decompresssion.h	- header for decompresssion.cpp

	-> node.cpp		- contains information for letter nodes used in assigning bit values to each letter
	-> controller.cpp	- functions to manipulate files used in both compresssion and decompression
	-> compresssion.cpp	- used to manipulate input file for compression
	-> decompresssion.cpp	- used to manipulate input file for decompresssion
	-> main.cpp		- to run compresssion and decompression on user inputed file
