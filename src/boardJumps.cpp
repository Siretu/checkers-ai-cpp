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


//works completely now, even for branching, pass along the actual character instead of matrix position
//since matrix position gets replaced when creating jumps

//creates a new jump given the start position (xs, ys), jumped over position (xj, yj), and end position (xe, ye)
//also takes a jump pointer jp which represents the previous jump
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
	jump* j = new jump(c, arr[xj][yj], xs, ys, xj, yj, xe, ye, jp);

	//deletes the character jumped over temporarily
	//it will be added in when the jumps are connected into a move
	//and the move is undone
	//the end space is not replaced because bugs will occur if it is
	//instead the character is passed
	arr[xj][yj] = 'e';
	arr[xs][ys] = 'e';

	//check if the previous jump existed
	//if it did, make noNext for that jump false
	//since the newly created jump is the next jump for that jump
	if (jp != NULL)
		jp->noNext = false;

	//add the newly created jump's address to the front of the list of jumps
	//the last jump will be at the front
	//order does not matter for createJumpMove
	jlist.push_front(j);

	//recursively call jumpAvailable, using the new end position
	//keep track of what piece is currently making the jumps by passing it as parameter
	jumpAvailable(jlist, c, xe, ye, j);
}


void board::createJumpMove(list<jump*>& jlist)
{
	if (!jlist.empty())
	{
		//iterate through the list of jumps
		//create new moves when appropriate
		list<jump*>::const_iterator it = jlist.begin();
		for (; it != jlist.end(); ++it)
		{
			if ((*it)->noNext && (*it)->visited == false)
			{
				move* m = new move((*it)->jumpingPiece, -1, -1, -1, -1);
				jump* jp = (*it);

				//add all the appropriate jumps to the move's list of jumps
				//reverse ordering, so that last jump is at the end of the list
				//for each jump added, set visited as true
				//visited is only needed for the last jump though
				//increment the jump's numTimes
				//repeat the loop until the first jump was added to the move's jump list
				while (jp != NULL)
				{
					m->jpoints.push_front(jp);
					++jp->numTimes;
					jp->visited = true;
					jp = jp->prev;
				}

				//add the start point to the move's start position and to command
				m->xi = m->jpoints.front()->xs;
				m->yi = m->jpoints.front()->ys;
				convert(m->jpoints.front()->xs, m->jpoints.front()->ys, m->command);

				//iterate through the move's list of jumps
				for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
				{
					//append the jump's end point to command
					convert((*it)->xend, (*it)->yend, m->command);

					//if there's no next move for the jump
					if ((*it)->noNext)
					{
						m->xf = (*it)->xend;
						m->yf = (*it)->yend;
					}
				}

				//add the move to the move list
				mlist.push_back(m);

				//adds the -1 to the end of the command
				m->command += "-1";

				//undoes each jump
				//replaces the 'e's with original characters
				//resets current jumping piece's position, if necessary
				undoMove(m);
			}
		}
	}
}

//compiled the code for jumping in all four directions
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

//checks top right jump
void board::checkJumpTR(list<jump*>& jlist, int x, int y, jump* jp = NULL)
{
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
		//even x
		if ((x % 2 == 0) && (jumpConditions(x-1, y+1, x-2, y+1)))
			createJump(jlist, arr[x][y], x, y, x-1, y+1, x-2, y+1, jp);
		//odd x
		else if ((x % 2 != 0) && jumpConditions(x-1, y, x-2, y+1))
			createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y+1, jp);
	}
}

//checks top left jump
void board::checkJumpTL(list<jump*>& jlist, int x, int y, jump* jp = NULL)
{
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
		//even x
		if ((x % 2 == 0) && (jumpConditions(x-1, y, x-2, y-1)))
			createJump(jlist, arr[x][y], x, y, x-1, y, x-2, y-1, jp);
		//odd x
		else if ((x % 2 != 0) && jumpConditions(x-1, y-1, x-2, y-1))
			createJump(jlist, arr[x][y], x, y, x-1, y-1, x-2, y-1, jp);
	}
}

//checks lower right jump
void board::checkJumpLR(list<jump*>& jlist, int x, int y, jump* jp = NULL)
{
	if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
		//even x
		if ((x % 2 == 0) && (jumpConditions(x+1, y+1, x+2, y+1)))
			createJump(jlist, arr[x][y], x, y, x+1, y+1, x+2, y+1, jp);
		//odd x
		else if ((x % 2 != 0) && jumpConditions(x+1, y, x+2, y+1))
			createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y+1, jp);
	}
}

//checks lower left jump
void board::checkJumpLL(list<jump*>& jlist, int x, int y, jump* jp = NULL)
{
	if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
		//even x
		if ((x % 2 == 0) && (jumpConditions(x+1, y, x+2, y-1)))
			createJump(jlist, arr[x][y], x, y, x+1, y, x+2, y-1, jp);
		//odd x
		else if ((x % 2 != 0) && jumpConditions(x+1, y-1, x+2, y-1))
			createJump(jlist, arr[x][y], x, y, x+1, y-1, x+2, y-1, jp);
	}
}

//add moves to the move list
//called by movesAvailable in board.h
bool board::jumpsAvailable()
{
	//automatically clears all moves
	//might need to get rid of this block when implementing alpha-beta
	//copy constructor already handles move clearing
	while (!mlist.empty())
	{
		//cout << "Only relevant when a final move is made" << endl;
		delete mlist.front();
		mlist.pop_front();
	}

	//a list of jumps
	list<jump*> jlist;

	//iterate through the matrix
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			//if the piece belongs to the current turn's color
			//check it for jumps
			if (arr[i][j] == color || arr[i][j] == toupper(color))
			{
				//check jumps in each of the four directions
				//then create jumping moves once the search is finished
				//this will enable branching on the first jump, if necessary
				//and it will restore the board each time
				//so that all cases are accounted for
				//TR - top right, TL - top left, LR - lower right, LL - lower left
				checkJumpTR(jlist, i, j, NULL);
				createJumpMove(jlist);
				checkJumpTL(jlist, i, j, NULL);
				createJumpMove(jlist);
				checkJumpLR(jlist, i, j, NULL);
				createJumpMove(jlist);
				checkJumpLL(jlist, i, j, NULL);
				createJumpMove(jlist);
			}
		}
	}

	//if no jumping moves were added, return false
	//else return true
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


