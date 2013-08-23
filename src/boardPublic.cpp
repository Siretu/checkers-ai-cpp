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

//
//
//
//public member functions:
void board::printEBoard(list<move*>& mlist)	//seems to work well
{
	printBoard(mlist);
	if (gameOver)
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
	{
		//call a function from game for alpha beta search results
		//cout << "The computer will make a move." << endl;
		//results of alpha beta search
		//ex. completed search to depth
		//searched for a total of t seconds.
		//The chosen move is: (1,2) -> (3,4)
	}
	inputCommand(mlist);
}
void board::makeMove(move* m)	//seems to work well
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
	arr[m->xf][m->yf] = arr[m->xi][m->yi];	//move the piece
	arr[m->xi][m->yi] = 'e';				//replace original position with 'e' (for empty)
	handleKinging(m->xf, m->yf);
	changeTurn();							//change player's turn
}

void board::printBoard(list<move*>& mlist)	//works
{
	cout << "Current board:" << endl;
	cout << endl;
	cout << "Player 1 is ";
	printcolor('b');
	cout << " (normal piece) and ";
	printcolor('B');
	cout << " (king)" << endl;
	cout << "Player 2 is ";
	printcolor('r');
	cout << " (normal piece) and ";
	printcolor('R');
	cout <<	" (king)" << endl;
	int count = 0;
	cout << "    " << count;
	++count;
	while (count != 8)
	{
		cout << "   " << count++;
	}
	cout << " " << endl;
	//padded 4 spaces in front
	//then first number
	//then 3 spaces
	//last number is followed by 1 space and end line
	string lineEven = "   XXX|   |XXX|   |XXX|   |XXX|   ";
	//padded 3 spaces
	string lineOdd = "      |XXX|   |XXX|   |XXX|   |XXX";
	//padded 6 spaces
	string linebreak = "   -------------------------------";
	//padded 3 spaces
	for (int i = 0; i != 8; ++i)
	{
		printline(i, lineEven, lineOdd);
		if (i != 7)
			cout << linebreak << endl;
	}
	cout << endl;
	if (color == 'b')
		cout << "Player 1 to move." << endl;
	else cout << "Player 2 to move." << endl;
	cout << "The legal moves are:" << endl;
	printMoves(mlist);
	cout << endl;
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




