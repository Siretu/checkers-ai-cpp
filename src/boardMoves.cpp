/*
 * boardMoves.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <cctype>
#include <list>

using std::list;
using std::toupper;

void board::checkNeighbors(list<move*>& mlist, int& x, int& y)	//seems to work fine
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

void board::createMove(list<move*>& mlist, const int& xi,const int& yi, int xf, int yf)	//works fine
{
	if (isValidPos(xf, yf) && arr[xf][yf] == 'e')
	{
		move* m = new move(xi, yi, xf, yf);
		mlist.push_back(m);
		convert(xi, yi, m->command);
		convert(xf, yf, m->command);
		m->command += "-1";
	}
}

bool board::listMoves(list<move*>& mlist)	//returns true if there are any regular moves, seems to work fine
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
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

