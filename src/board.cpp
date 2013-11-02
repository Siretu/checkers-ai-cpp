/*
 * board.cpp
 *
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <fstream>
#include <cctype>
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
move::~move()
{
	for (list<jump*>::iterator it = jpoints.begin(); it != jpoints.end(); ++it)
	{
		--(*it)->numTimes;
		if ((*it)->numTimes == 0)
			delete (*it);
	}
}

//initializes everything for the checker board
board::board()
{
	reset();
}

//destructor deallocates memory for all the moves in mlist
board::~board()
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
}

//copy constructor: copies over all data values except the move list
//useful for creating new boards for each move in alpha-beta search
board::board(const board& b): color(b.color)
{
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
			arr[i][j] = b.arr[i][j];
}

//resets the board, called by printEBoard in boardPublic.cpp
//create the start board
//first three rows are filled with black pieces
//next two rows are empty
//last three rows are filled with red pieces
void board::reset()
{
	color = 'b';
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

//store it all in a list
//parse each line
//modify the board per line
//last line, the 9th line, will be turn
//a sample line of input would be
//e b b e
//the last line, get the current piece color's turn
//set color equal to it
//make sure the color is valid
void board::modifyBoard(ifstream& fin)
{
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

//eliminate the \r character in a string or the \n character
inline void board::remove_carriage_return(std::string& line)
{
    if (*line.rbegin() == '\r' || *line.rbegin() == '\n')
    	line.erase(line.length() - 1);
}
