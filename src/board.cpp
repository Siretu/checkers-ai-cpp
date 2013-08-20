/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#include "board.h"
#include <list>
#include <cctype>
#include <utility>

using std::list;
using std::toupper;
using std::pair;

board::board(): color('b')
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

void board::deleteMoveslist(list<move*>& mlist)	//deletes all the moves allocated by new in the list
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
}

bool board::terminalTest(list<move*>& mlist)	//use this in conjunction with color member
//like if terminalTest and color = 'b' / 'r'
//call terminal test first, movesAvailable will automatically create a list of moves
{
	if (!movesAvailable(mlist))
		return true;
	if (color == 'b' && piecesCount[0] == 0)
		return true;
	if (color == 'r' && piecesCount[1] == 0)
		return true;
	return false;
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

bool board::jumpsAvailable(list<move*>& mlist)
{
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
				checkJump(mlist, i, j);
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

void board::undoMove(move* m)
{
	char c = arr[m->xf][m->yf];
	if (!(m->jpoints.empty()))
	{
		list<cJumped>::iterator it = m->jpoints.begin();
		while (it != m->jpoints.end())
		{
			arr[it->x][it->y] = it->c;
			++it;
		}
	}
	arr[m->xf][m->yf] = 'e';
	arr[m->xi][m->yi] = c;
}

void board::checkJump(list<move*>& mlist, int& x, int& y)
{
	if (color == 'b' || arr[x][y] == 'R')
	{

	}
	if (color == 'r' || arr[x][y] == 'B')
	{

	}
}



