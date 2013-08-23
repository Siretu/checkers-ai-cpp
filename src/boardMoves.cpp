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

void board::checkNeighbors(int& x, int& y)
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		//if it's odd subtract or stay same for y coordinate
		//if it's even add or stay same
		//will always add x coordinate
		//if it's a king (on appropriate turn), check backwards too
		if (x % 2 == 0)
		{
			createMove(x, y, x+1, y);
			createMove(x, y, x+1, y+1);
		}
		else
		{
			createMove(x, y, x+1, y);
			createMove(x, y, x+1, y-1);
		}
	}
	if (color == 'r' || arr[x][y] == 'B')
	{
		//if it's even add or stay the same
		//if it's odd subtract or stay the same
		//will always subtract x coordinate
		if (x % 2 == 0)
		{
			createMove(x, y, x-1, y);
			createMove(x, y, x-1, y+1);
		}
		else
		{
			createMove(x, y, x-1, y);
			createMove(x, y, x-1, y-1);
		}
	}
}

void board::createMove(const int& xi,const int& yi, int xf, int yf)
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

bool board::listMoves()	//returns true if there are any regular moves
{
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
				checkNeighbors(i, j);
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

