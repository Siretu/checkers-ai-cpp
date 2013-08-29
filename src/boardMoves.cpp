/*
 * boardMoves.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <cctype>
#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::toupper;

//checks neighboring positions using the following rules:
//for y coordinate
//-if it's odd x, subtract or stay same
//-if it's even x, add or stay same
//will always add/subtract x coordinate
//depending on direction of motion (up/down)
//if it's a king (on appropriate turn), check backwards too
//draw out the board to see visually
//-top left is 0,0 lower right is 7,3
void board::checkNeighbors(int& x, int& y)
{
	//check down movement
	//cases depend on whether or not it's an even or odd row
	if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
	{
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
	//check up movement
	if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
	{
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

//creates the move given the beginning and end positions of the move
//also creates the command associated with the move
void board::createMove(const int& xi,const int& yi, int xf, int yf)
{
	//check for valid positions and
	//check to make sure the adjacent piece is empty
	if (isValidPos(xf, yf) && arr[xf][yf] == 'e')
	{
		move* m = new move(arr[xi][yi], xi, yi, xf, yf);
		mlist.push_back(m);
		convert(xi, yi, m->command);
		convert(xf, yf, m->command);
		m->command += "-1";
	}
}

//returns true if there are any regular moves
//called by movesAvailable in board.h
//called only if jumpsAvailable returns false
bool board::listMoves()
{
	//automatically clears all moves
	//need to get rid of this block when implementing alpha-beta
	//copy constructor already handles move clearing
	while (!mlist.empty())
	{
		//cout << "Only relevant when a final move is made" << endl;
		delete mlist.front();
		mlist.pop_front();
	}

	//iterate through the matrix
	//check neighboring positions
	//if the piece matches the current turn's color
	if (color == 'r')
	{
		for (int i = 0; i!= 8; ++i)
			for (int j = 0; j != 4; ++j)
				if (arr[i][j] == color || arr[i][j] == toupper(color))
					checkNeighbors(i, j);
	}
	//color == 'b'
	//order this differently so that the moves are ordered so that
	//pieces closest to being kings get checked first
	//useful for alpha-beta
	else
	{
		for (int i = 7; i!= -1; --i)
			for (int j = 0; j != 4; ++j)
				if (arr[i][j] == color || arr[i][j] == toupper(color))
					checkNeighbors(i, j);
	}


	//if any moves are added, return true
	//else return false
	if (mlist.empty())
		return false;
	return true;
}

