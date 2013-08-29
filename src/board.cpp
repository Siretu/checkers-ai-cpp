/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

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

bool board::isComputer[2];

int board::timeLimit = 0;

//public constructors
//---------------------------------------------------------------------------------
board::board()
{
	//initializes everything for the checker board
	reset();
}

board::~board()
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
}

board::board(const board& b): color(b.color)
{
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
			arr[i][j] = b.arr[i][j];
	for (int i = 0; i != 2; ++i)
	{
		piecesCount[i] = b.piecesCount[i];
		kingCount[i] = b.kingCount[i];
	}
}

move::~move()
{
	//frees the memory allocated on the heap for each jump pointer
	//avoids double freeing of memory by keeping track of the
	//number of times each jump was added to a moves jump list
	//first decrements each jump's numTimes
	//only deletes the jump if numTimes equals 0
	//this is necessary for multiple moves utilizing the same jumps,
	//such as in the case of branching jumps:
	//			1
	//		2
	//	3		3'
	//		4
	// 1 -> 2 would have numTimes equal to 2 since the jump would be utilized twice,
	//once for each move
	for (list<jump*>::iterator it = jpoints.begin(); it != jpoints.end(); ++it)
	{
		--(*it)->numTimes;
		if ((*it)->numTimes == 0)
			delete (*it);
	}
}
//---------------------------------------------------------------------------------

//resets the board, called by printEBoard in boardPublic.cpp
void board::reset()
{
	color = 'b';

	//sets initial piece count to 12 for each player
	piecesCount[0] = 12;
	piecesCount[1] = 12;

	//start out with no kings
	kingCount[0] = 0;
	kingCount[1] = 0;

	//create the start board
	//first three rows are filled with black pieces
	//next two rows are empty
	//last three rows are filled with red pieces
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

void board::modifyBoard(ifstream& fin)
{
	//store it all in a list
	//parse each line
	//modify the board per line
	//last line, the 9th line, will be turn
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

	//the last line, get the current piece color's turn
	//set color equal to it
	getline(fin, line);
	remove_carriage_return(line);
	stringstream ss(line);
	ss >> color;

	//reset piece counts
	piecesCount[0] = 0;
	piecesCount[1] = 0;
	kingCount[0] = 0;
	kingCount[1] = 0;

	//make sure the color is valid
	assert(color == 'b' || color == 'r');

	//increment the pieces count accordingly
	//by iterating through the entire matrix
	//and counting up the pieces
	for (int i = 0; i != 8; i++)
		for (int j =0; j != 4; j++)
		{
			if (arr[i][j] == 'b')
				++piecesCount[0];
			else if (arr[i][j] == 'r')
				++piecesCount[1];
			else if (arr[i][j] == 'B')
			{
				++kingCount[0];
				++piecesCount[0];
			}
			else if (arr[i][j] == 'R')
			{
				++kingCount[1];
				++piecesCount[1];
			}
		}
}


