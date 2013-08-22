/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */
//#define NDEBUG
#include <assert.h>
#include "board.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <windows.h>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::string;
using std::stringstream;
using std::toupper;

//public constructors
board::board(char c = 'b'): color(c)
{
	piecesCount[0] = 12;
	piecesCount[1] = 12;
	kingCount[0] = 0;
	kingCount[1] = 0;
	for (int i = 0; i != 3; ++i)
		for (int j = 0; j != 3; ++j)
			arr[i][j] = 'b';
	for (int i = 3; i != 5; ++i)
		for (int j = 0; j != 3; ++j)
			arr[i][j] = 'e';
	for (int i = 5; i != 8; ++i)
		for (int j = 0; j != 3; ++j)
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

//private member functions
void board::createJump(std::list<jump*>& jlist, int xj, int yj, int xe, int ye, jump* jp)
{
	jump* j = new jump(arr[xj][yj], xj, yj, xe, ye, jp);
	arr[xj][yj] = 'e';	//deletes the character temporarily
	recurseInc(j);
	if (jp != NULL)
		jp->noNext = false;
	jlist.push_front(j);	//pushes the front, iterate from start to end
	jumpAvailable(jlist, xe, ye, j);
}

void board::createJumpMove(list<move*>& mlist, list<jump*>& jlist, const int& x, const int& y)
{
	while (!jlist.empty())
	{
		jump* jp = jlist.front();
		move* m = new move(x, y, -1, -1);
		while (jp->prev != NULL)
		{
			m->jpoints.push_back(jp);	//reverse ordering
			--jp->numtimes;
			if (jp->numtimes == 0)
				jlist.remove(jp);
			jp = jp->prev;
		}
		for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
		{
			undoJump(*it);	//undoes each jump, replaces the 'e's with original characters
			if ((*it)->noNext)	//if there's no next move for the jump
			{
				m->xf = (*it)->xend;
				m->yf = (*it)->yend;
			}
		}
		if(m->xf != -1 && m->yf != -1)
			mlist.push_back(m);
		else
		{
			assert(m->jpoints.empty());
			delete m;
		}
	}
	assert(jlist.empty());
}

void board::jumpAvailable(list<jump*>& jlist, int x, int y, jump* jp= NULL)	//i, j are start points
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x+1, y, x+2, y-1))	//checks left down jump
				createJump(jlist, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x+1, y+1, x+2, y+1))	//checks right down jump
				createJump(jlist, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x+1, y-1, x+2, y-1))	//checks left down jump
				createJump(jlist, x+1, y-1, x+2, y-1, jp);
			if (jumpConditions(x+1, y, x+2, y+1))	//checks right down jump
				createJump(jlist, x+1, y, x+2, y+1, jp);
		}
	}
	if (color == 'r' || arr[x][y] == 'B')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x-1, y, x-2, y-1))	//checks left up jump
				createJump(jlist, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x-1, y+1, x-2, y+1))	//checks right up jump
				createJump(jlist, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x-1, y-1, x-2, y-1))	//checks left up jump
				createJump(jlist, x-1, y-1, x-2, y-1, jp);
			if (jumpConditions(x-1, y, x-2, y+1))	//checks right up jump
				createJump(jlist, x-1, y, x-2, y+1, jp);
		}
	}
}

bool board::jumpsAvailable(list<move*>& mlist)
{
	list<jump*> jlist;
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
			{
				jumpAvailable(jlist, i, j);
				createJumpMove(mlist, jlist, i, j);
			}
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

void board::recurseInc(jump* j)	//recursively increment numtimes
{
	++j->numtimes;
	jump* jp = j;
	while (j->prev != NULL)
	{
		jp = j->prev;
		++jp->numtimes;
	}
}

void board::checkNeighbors(list<move*>& mlist, int& x, int& y)
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		//if it's odd subtract or stay same for y coordinate
		//if it's even add or stay same
		//will always add x coordinate
		//if it's a king (on appropriate turn), check backwards too
		if (x % 2 == 0)
		{
			createMove(mlist, x, y, x+1, y);
			createMove(mlist, x, y, x+1, y+1);
		}
		else
		{
			createMove(mlist, x, y, x+1, y);
			createMove(mlist, x, y, x+1, y-1);
		}
	}
	if (color == 'r' || arr[x][y] == 'B')
	{
		//if it's even add or stay the same
		//if it's odd subtract or stay the same
		//will always subtract x coordinate
		if (x % 2 == 0)
		{
			createMove(mlist, x, y, x-1, y);
			createMove(mlist, x, y, x-1, y+1);
		}
		else
		{
			createMove(mlist, x, y, x-1, y);
			createMove(mlist, x, y, x-1, y-1);
		}
	}
}

bool board::listMoves(list<move*>& mlist)	//returns true if there are any regular moves
{
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
				checkNeighbors(mlist, i, j);
		}
	}
	if (mlist.empty())
		return false;
	return true;
}
//
//
//
//public member functions:
void board::deleteMoveslist(list<move*>& mlist)	//deletes all the moves allocated by new in the list
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
}

void board::makeMove(move* m)
{
	if (!m->jpoints.empty())
	{
		list<jump*>::iterator it = m->jpoints.begin();
		for (; it != m->jpoints.end(); ++it)
		{
			arr[(*it)->x][(*it)->y] = 'e';
			if ((*it)->c == 'r')
				--piecesCount[1];
			else if ((*it)->c == 'b')
				--piecesCount[0];
			else if ((*it)->c == 'R')
			{
				--piecesCount[1];
				--kingCount[1];
			}
			else	//(*it)->c == 'B'
			{
				--piecesCount[0];
				--kingCount[0];
			}
		}
	}
	arr[m->xf][m->yf] = arr[m->xi][m->yi];	//move the piece
	arr[m->xi][m->yi] = 'e';				//replace original position with 'e' (for empty)
}

void board::modifyBoard(ifstream& fin)
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

void board::printline(const int& i, const string& lineEven, const string& lineOdd)
{
	if (i % 2 == 0)
	{
		cout << lineEven << endl;
		cout << " " << i << " XXX|";
		for (int j = 0; j != 3; ++j)
		{
			cout << " " << arr[i][j] << " |XXX|";
		}
		cout << " " << arr[i][3] << " " << endl;;
		cout << lineEven;
	}
	else
	{
		cout << lineOdd << endl;
		cout << " " << i << " ";
		for (int j = 0; j != 3; ++j)
		{
			cout << " " << arr[i][j] << " |XXX|";
		}
		cout << " " << arr[i][3] << " |XXX" << endl;;
		cout << lineOdd << endl;
	}
}
//need to implement text color change
void board::printBoard()
{
	int count = 0;
	cout << "    " << count;
	while (count != 8)
	{
		cout << "   " << ++count;
	}
	cout << " " << endl;
	//padded 4 spaces in front
	//then first number
	//then 3 spaces
	//last number is followed by 1 space and end line
	string lineEven = "   XXX|   |XXX|   |XXX|   |XXX|   ";
	//padded 3 spaces
	string lineOdd = "      |XXX|   |XXX|   |XXX|   |XXX";
	//padded 6 spaces
	string linebreak = "   -------------------------------";
	//padded 3 spaces
	for (int i = 0; i != 8; ++i)
	{
		printline(i, lineEven, lineOdd);
		if (i != 7)
			cout << linebreak << endl;
	}
}

//miscellaneous functions for parsing
inline void remove_carriage_return(string& line)
//eliminate the \r character in a string
//this is needed in some cases of getline, such as when I read my input file
{
    if (*line.rbegin() == '\r')
    {
        line.erase(line.length() - 1);
    }
}
