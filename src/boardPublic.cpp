/*
 * boardPublic.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::string;
using std::tolower;

//prints everything needed for a board
//prints out game over message when necessary
//need to add functionality for playing again
//need to add functionality for computer calling alpha-beta search
void board::printEBoard()
{
	printBoard();
	if (mlist.empty() || (color == 'b' && piecesCount[0] == 0) ||
			(color == 'r' && piecesCount[1] == 0))
	{
		cout << "The game is over." << endl;
		cout << endl;
		if (color == 'r')
			cout << "Player 1 wins." << endl;
		else cout << "Player 2 wins." << endl;
		//add functionality for playing again
		//cout << "Do you want to play again? (Y/N):" << endl
		//cin
	}
	//else if ((color == 'b' && board::isComputer[0]) || (color == 'r' && board::isComputer[1]))
		//call a function from game for alpha beta search results
		//cout << "The computer will make a move." << endl;
		//results of alpha beta search
		//ex. completed search to depth
		//searched for a total of t seconds.
		//The chosen move is: (1,2) -> (3,4)
	else inputCommand();
}

//makes a move
//if there's any jumps, they are implemented
//pieces are erased and subtracted from the total count if necessary
//moves the piece from one position to another
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

	//save the piece
	char c = arr[m->xi][m->yi];

	//replace the start position with an empty space
	arr[m->xi][m->yi] = 'e';

	//add back in the saved piece at the end point
	arr[m->xf][m->yf] = c;

	//check if the piece should be changed to a king
	handleKinging(m->xf, m->yf);

	//change player's turn
	changeTurn();
}

/*
void board::startup()		//determines whether or not players will be a computer calls modifyBoard
{
	board::whoComputer();
	char c = ' ';
	while (tolower(c) != 'y' || tolower(c) != 'n')
	{
		cout << "Do you want to load a game from a file? (Y/N):" << endl;
		cin >> c;
	}
	if (tolower(c) == 'y')
	{
		string name;
		cout << "Enter filename: " << endl;
		cin >> name;
		ifstream fin(name.c_str());
		assert(fin.good());
		modifyBoard(fin);
	}
	if (board::isComputer[0] == true || board::isComputer[1] == true)
	{
		//implement timer stuff
	}
}*/




