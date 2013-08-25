/*
 * boardJumps.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <iostream>
#include <cctype>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::toupper;


//problem remains for double jumps, needs better cleaning up, re-think backtracking and undoing jumps
//seems to be problems there
//seems to be able to handle single jumps but no double jumps
//private member functions
//functions for jumps
//just need to fix character replacement now!!!!!!!!!!!
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
	//cout << xj << " " << yj << " " << jp << " ";
	jump* j = new jump(arr[xj][yj], xs, ys, xj, yj, xe, ye, jp);
	arr[xj][yj] = 'e';	//deletes the character temporarily
	arr[xs][ys] = 'e';
	arr[xe][ye] = c;
	if (jp != NULL)
		jp->noNext = false;
	jlist.push_front(j);	//pushes the front, iterate from start to end, last jump is in front
	jumpAvailable(jlist, xe, ye, j);
}

void board::createJumpMove(list<move*>& mlist, list<jump*>& jlist)
//better than before still bugs
//and stackdump error
{
	if (!jlist.empty())
	{
		list<jump*>::const_iterator it = jlist.begin();
		for (; it != jlist.end(); ++it)
		{
			if ((*it)->noNext)
			{
				move* m = new move(-1, -1, -1, -1);
				jump* jp = (*it);
				while (jp != NULL)	//PROBLEM WAS HERE FIXED!!!!!!!!
				{
					m->jpoints.push_front(jp);	//reverse ordering, so that last jump is at the end
					++jp->numTimes;
					jp = jp->prev;
				}
				m->xi = m->jpoints.front()->xs;
				m->yi = m->jpoints.front()->ys;
				convert(m->jpoints.front()->xs, m->jpoints.front()->ys, m->command);
				for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
				{
					convert((*it)->xend, (*it)->yend, m->command);
					if ((*it)->noNext)	//if there's no next move for the jump
					{
						m->xf = (*it)->xend;
						m->yf = (*it)->yend;
					}
				}
				mlist.push_back(m);
				m->command += "-1";
				undoMove(m);	//undoes each jump, replaces the 'e's with original characters, resets char position
			}
		}
		/*jump* jp = jlist.front();
		move* m = new move(x, y, -1, -1);
		while (jp != NULL)	//PROBLEM WAS HERE FIXED!!!!!!!!
		{
			m->jpoints.push_front(jp);	//reverse ordering, so that last jump is at the end
			--jp->numtimes;
			if (jp->numtimes == 0)
				jlist.remove(jp);
			jp = jp->prev;
		}*/
		jlist.clear();
	}
}
//need up to 8 cases!!!!
//check each initial direction
//create move
//after each one terminates
//create the move and undo it
//repeat for the remaining cases

void board::jumpAvailable(list<jump*>& jlist, int x, int y, jump* jp= NULL)	//i, j are start points
{
	checkJumpTR(jlist, x, y, jp);
	checkJumpTL(jlist, x, y, jp);
	checkJumpLR(jlist, x, y, jp);
	checkJumpLL(jlist, x, y, jp);
	//cout << jlist.size() << endl;
	//the code below works, problem with branching
	/*if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x+1, y, x+2, y-1))	//checks left down jump
				createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x+1, y+1, x+2, y+1))	//checks right down jump
				createJump(jlist, arr[x][y], x, y, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x+1, y-1, x+2, y-1))	//checks left down jump
				createJump(jlist, arr[x][y], x, y, x+1, y-1, x+2, y-1, jp);
			if (jumpConditions(x+1, y, x+2, y+1))	//checks right down jump
				createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y+1, jp);
		}
	}
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x-1, y+1, x-2, y+1))	//checks right up jump
				createJump(jlist, arr[x][y], x, y, x-1, y+1, x-2, y+1, jp);
			if (jumpConditions(x-1, y, x-2, y-1))	//checks left up jump
				createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y-1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x-1, y-1, x-2, y-1))	//checks left up jump
				createJump(jlist, arr[x][y], x, y, x-1, y-1, x-2, y-1, jp);
			if (jumpConditions(x-1, y, x-2, y+1))	//checks right up jump
				createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y+1, jp);
		}
	}*/
}

void board::checkJumpTR(list<jump*>& jlist, int x, int y, jump* jp = NULL)
//checks right up jump
{
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
		if ((x % 2 == 0) && (jumpConditions(x-1, y+1, x-2, y+1))) //even x
			createJump(jlist, arr[x][y], x, y, x-1, y+1, x-2, y+1, jp);
		//odd x
		else if (jumpConditions(x-1, y, x-2, y+1))	//checks right up jump
			createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y+1, jp);
	}
}

void board::checkJumpTL(list<jump*>& jlist, int x, int y, jump* jp = NULL)
//checks left up jump
{
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
		if ((x % 2 == 0) && (jumpConditions(x-1, y, x-2, y-1)))	//even x
			createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y-1, jp);
		//odd x
		else if (jumpConditions(x-1, y-1, x-2, y-1))
			createJump(jlist, arr[x][y], x, y, x-1, y-1, x-2, y-1, jp);
	}
}

void board::checkJumpLR(list<jump*>& jlist, int x, int y, jump* jp = NULL)
//checks right down jump
{
	if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
		if ((x % 2 == 0) && (jumpConditions(x+1, y+1, x+2, y+1)))
			createJump(jlist, arr[x][y], x, y, x+1, y+1, x+2, y+1, jp);
		//odd x
		else if (jumpConditions(x+1, y, x+2, y+1))
			createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y+1, jp);
	}
}

void board::checkJumpLL(list<jump*>& jlist, int x, int y, jump* jp = NULL)
//checks left down jump
{
	if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
		if ((x % 2 == 0) && (jumpConditions(x+1, y, x+2, y-1)))	//even x
			createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y-1, jp);
		//odd x
		else if (jumpConditions(x+1, y-1, x+2, y-1))
			createJump(jlist, arr[x][y], x, y, x+1, y-1, x+2, y-1, jp);
	}
}

bool board::jumpsAvailable(list<move*>& mlist)
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
	list<jump*> jlist;
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
			{
				checkJumpTR(jlist, i, j, NULL);
				createJumpMove(mlist, jlist);
				checkJumpTL(jlist, i, j, NULL);
				createJumpMove(mlist, jlist);
				checkJumpLR(jlist, i, j, NULL);
				createJumpMove(mlist, jlist);
				checkJumpLL(jlist, i, j, NULL);
				createJumpMove(mlist, jlist);
			}
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

bool board::jumpConditions(int xj, int yj, int xe, int ye)
{
	if (isValidPos(xj, yj) && isValidPos(xe, ye) && arr[xj][yj] != 'e' &&
			arr[xj][yj] != color && arr[xe][ye] == 'e' &&  arr[xj][yj] != std::toupper(color))
		return true;
	return false;
}

void board::undoMove(move* m)
{
	char c;
	if (arr[m->xi][m->yi] != 'e')
		c = arr[m->xi][m->yi];
	else c = arr[m->xf][m->yf];
	// replaces the starting jump point only if the starting jump hasn't already been replaced
	if (!m->jpoints.empty())
	{
		for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
		{
			arr[(*it)->xs][(*it)->ys] = 'e';
			arr[(*it)->x][(*it)->y] = (*it)->c;
			arr[(*it)->xend][(*it)->yend] = 'e';
		}
	}
		arr[m->xi][m->yi] = c;
}
