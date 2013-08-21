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
#include <vector>

using std::list;
using std::toupper;
using std::vector;

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

move::~move()
{
	while(!jpoints.empty())
	{
		delete jpoints.front();
		jpoints.pop_front();
	}
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

//
//
//

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
				//undo jumps here + create moves from jumps
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

void board::createJump(std::list<jump*>& jlist, int xj, int yj, int xe, int ye, jump* jp)
{
	jump* j = new jump(arr[xj][yj], xj, yj, xe, ye, jp);
	arr[xj][yj] = 'e';	//deletes the character temporarily
	recurseInc(j);
	if (jp != NULL)
		jp->next.push_back(j);
	jlist.push_front(j);	//pushes the front, iterate from start to end
	jumpAvailable(jlist, xe, ye, j);
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
			if ((*it)->next.empty())
			{
				m->xf = (*it)->xend;
				m->yf = (*it)->yend;
			}
		}
	}
}
/*
void board::createJump(move* m, int& xi, int& yi, int xj, int yj, int xf, int yf)
{
	if (isValidPos(xj, yj) && isValidPos(xf, yf))
	{
		char jp = arr[xj][yj];		//jump point's char
		if (jp != 'e' && jp != color && jp != toupper(color) && arr[xf][yf] == 'e')
		{
			m->jpoints.push_back(jump(jp, xj, yj));
			m->xf = xf;
			m->yf = yf;
			xi = xf;
			yi = yf;
			arr[xj][yj] = 'e';
		}
	}
}

//need to delete points as we go, create copies
void board::jumpAvailable(move* m)
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		if (x % 2 == 0)
		{
			if (arr[x+1][y] != 'e' && arr[x+1][y] != color && arr[x+1][y] != toupper(color))
			{

			}
		}
	}
	if (color == 'r' || arr[x][y] == 'B')
	{

	}
}

*/

