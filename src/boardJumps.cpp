/*
 * boardJumps.cpp
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

//private member functions
//functions for jumps
void board::createJump(std::list<jump*>& jlist, int xj, int yj, int xe, int ye, jump* jp)
{
	jump* j = new jump(arr[xj][yj], xj, yj, xe, ye, jp);
	arr[xj][yj] = 'e';	//deletes the character temporarily
	recurseInc(j);
	if (jp != NULL)
		jp->noNext = false;
	jlist.push_front(j);	//pushes the front, iterate from start to end, last jump is in front
	jumpAvailable(jlist, xe, ye, j);
}

void board::createJumpMove(list<jump*>& jlist, const int& x, const int& y)
{
	while (!jlist.empty())
	{
		jump* jp = jlist.front();
		move* m = new move(x, y, -1, -1);
		while (jp->prev != NULL)
		{
			m->jpoints.push_front(jp);	//reverse ordering, so that last jump is at the end
			--jp->numtimes;
			if (jp->numtimes == 0)
				jlist.remove(jp);
			jp = jp->prev;
		}
		convert(x, y, m->command);
		for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
		{
			convert((*it)->xend, (*it)->yend, m->command);
			undoJump(*it);	//undoes each jump, replaces the 'e's with original characters
			if ((*it)->noNext)	//if there's no next move for the jump
			{
				m->xf = (*it)->xend;
				m->yf = (*it)->yend;
			}
		}
		if(m->xf != -1 && m->yf != -1)
		{
			mlist.push_back(m);
			m->command += "-1";
		}
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

bool board::jumpsAvailable()
{
	list<jump*> jlist;
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
			{
				jumpAvailable(jlist, i, j);
				createJumpMove(jlist, i, j);
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

