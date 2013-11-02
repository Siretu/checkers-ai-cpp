/*
 * boardJumps.cpp
 *
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

//create a "code" for each jump, used to prevent repetitions
inline int board::createkey(int xs, int ys, int xj, int yj, int xe, int ye)
{
    return ye + xe*10 + yj*100 + xj*1000 + ys*10000 + xs*100000;
}

//reverses a jump's key
//so 233444 will be broken up into 23 34 44 and reorganized to become 444332
//this is to prevent any repeated jumps
int board::reverse(int i)
{
    int num = 0;
    while (i/100 > 0)
    {
        num += i % 100;
        num *= 100;
        i /= 100;
    }
    num += i;
    return num;

}

//creates a new jump given the start position (xs, ys),
//jumped over position (xj, yj), and end position (xe, ye)
//also takes a jump pointer jp which represents the previous jump
//it will be added in when the jumps are connected into a move
//instead the character is passed
//recursively call jumpAvailable, using the new end position
//keep track of what piece is currently making the jumps by passing it as parameter
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
    arr[xs][ys] = 'e';
    //+1 to each because 0 will mess up the keys and reverse for an edge case such as xs=0 ys =0
    int key = createkey(xs+1, ys+1, xj+1, yj+1, xe+1, ye+1);
    jump* jcheck = jp;
    while (jcheck != NULL)
    {
        if (key == jcheck->key || key == reverse(jcheck->key))
            return;
        jcheck = jcheck->prev;
    }
	jump* j = new jump(c, arr[xj][yj], xs, ys, xj, yj, xe, ye, jp, key);
	if (jp != NULL)
		jp->noNext = false;
	jlist.push_front(j);
	jumpAvailable(jlist, c, xe, ye, j);
}

//iterate through the list of jumps
//create new moves only when encountering the last jump
//add all the appropriate jumps to the move's list of jumps via backtracking
//increment the jump's numTimes
//repeat the loop until the first jump was added to the move's jump list
//add the start point to the move's start position and to command
//undoes each jump, so the starting character is where it began before getting erased
//replaces the 'e's with original characters
void board::createJumpMove(list<jump*>& jlist)
{
	if (!jlist.empty())
	{
		list<jump*>::const_iterator it = jlist.begin();
		for (; it != jlist.end(); ++it)
		{
			if ((*it)->noNext)
			{
				move* m = new move((*it)->jumpingPiece, -1, -1, -1, -1);
				jump* jp = (*it);
				while (jp != NULL)
				{
					m->jpoints.push_front(jp);
					++jp->numTimes;
					jp = jp->prev;
				}
				m->xi = m->jpoints.front()->xs;
				m->yi = m->jpoints.front()->ys;
				convert(m->jpoints.front()->xs, m->jpoints.front()->ys, m->command);
				for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
				{
					convert((*it)->xend, (*it)->yend, m->command);
					if ((*it)->noNext)
					{
						m->xf = (*it)->xend;
						m->yf = (*it)->yend;
					}
				}

				mlist.push_back(m);
				m->command += "-1";
				undoMove(m);
			}
		}
	}
}

//checking for jumping in all four directions
//(x,y) is the start point
void board::jumpAvailable(list<jump*>& jlist, char c, int x, int y, jump* jp= NULL)
{
	if (tolower(c) == 'b' || c == 'R')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x+1, y, x+2, y-1))	//checks left down jump
				createJump(jlist, c, x, y, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x+1, y+1, x+2, y+1))	//checks right down jump
				createJump(jlist, c, x, y, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x+1, y-1, x+2, y-1))	//checks left down jump
				createJump(jlist, c, x, y, x+1, y-1, x+2, y-1, jp);
			if (jumpConditions(x+1, y, x+2, y+1))	//checks right down jump
				createJump(jlist, c, x, y, x+1, y, x+2, y+1, jp);
		}
	}
	if (tolower(c) == 'r' || c == 'B')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x-1, y+1, x-2, y+1))	//checks right up jump
				createJump(jlist, c, x, y, x-1, y+1, x-2, y+1, jp);
			if (jumpConditions(x-1, y, x-2, y-1))	//checks left up jump
				createJump(jlist, c, x, y, x-1, y, x-2, y-1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x-1, y-1, x-2, y-1))	//checks left up jump
				createJump(jlist, c, x, y, x-1, y-1, x-2, y-1, jp);
			if (jumpConditions(x-1, y, x-2, y+1))	//checks right up jump
				createJump(jlist, c, x, y, x-1, y, x-2, y+1, jp);
		}
	}
}

//called by movesAvailable in board.h
//clears all moves
//if the piece belongs to the current turn's color
//check it for jumps
//then create jumping moves once the search is finished
bool board::jumpsAvailable()
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
			{
                list<jump*> jlist;
                jumpAvailable(jlist, arr[i][j], i, j, NULL);
				createJumpMove(jlist);
			}
		}
	}

	//if no jumping moves were added, return false, else return true
	if (mlist.empty())
		return false;
	return true;
}

//checks for jumping conditions
//checks if the jumped point is valid and has the enemy's color
//checks if the end point is valid and empty
//returns true if conditions are satisfied
bool board::jumpConditions(int xj, int yj, int xe, int ye)
{
	if (isValidPos(xj, yj) && isValidPos(xe, ye) && arr[xj][yj] != 'e' &&
			arr[xj][yj] != color && arr[xe][ye] == 'e' &&  arr[xj][yj] != std::toupper(color))
		return true;
	return false;
}


