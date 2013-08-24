/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */
//#define NDEBUG
#include <assert.h>
#include "board.h"
#include <fstream>
#include <list>
#include <string>
#include <sstream>

using std::getline;
using std::ifstream;
using std::list;
using std::string;
using std::stringstream;

//public constructors
board::board(): color('b')
{
	piecesCount[0] = 12;
	piecesCount[1] = 12;
	kingCount[0] = 0;
	kingCount[1] = 0;
	for (int i = 0; i != 3; ++i)
		for (int j = 0; j != 4; ++j)
			arr[i][j] = 'b';
	for (int i = 3; i != 5; ++i)
		for (int j = 0; j != 4; ++j)
			arr[i][j] = 'e';
	for (int i = 5; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
			arr[i][j] = 'r';
}

move::~move()
{
	while(!jpoints.empty())
	{
		delete jpoints.front();
		jpoints.pop_front();
	}
}

void board::modifyBoard(ifstream& fin)	//works fine
{
	//store it all in a list
	//parse each line
	//modify the board per line
	//last line (9th line will be turn)
	//a sample line of input would be
	//e b b e
	string line;
	int count = 0;
	while (getline(fin, line) && count != 8)
	{
		remove_carriage_return(line);
		stringstream ss(line);
		for (int jIndex = 0; jIndex != 4; ++jIndex)
			ss >> arr[count][jIndex];
		++count;
	}
	getline(fin, line);
	remove_carriage_return(line);
	stringstream ss(line);
	ss >> color;
	assert(color == 'b' || color == 'r');
}


