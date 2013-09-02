/*
 * boardPublic.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::string;
using std::tolower;

//used to print out all available moves
//takes a command as an argument and displays it
//2 3 3 2 -1
//is converted to (2, 3) -> (3, 2)
//called by inputCommand
void board::convertCommand(const string& s)
{
	string::const_iterator it = s.begin();
	cout << "(" << (*it) << ", ";
	it += 2;
	cout << (*it) << ") ";
	it += 2;
	while (*it != '-')
	{
		cout << "-> (" << (*it) << ", ";
		it += 2;
		cout << (*it) << ") ";
		it += 2;
	}
}

//functions for outputting commands
//for humans, ask for move
//for computer, this function will never be called
//instead another case will run in printEBoard which is found in boardPublic.cpp
void board::inputCommand()
{
	printMoves();
	string m;
	cout << "Enter a sequence of integers indicating a move." << endl;
	cout << "Each set of two integers represents a position." << endl;
	cout << "End the sequence with -1." << endl;
	cout << "For example: 2 3 3 2 -1" << endl;
	cout <<	"	represents (2,3) -> (3,2)" << endl;
	cout << "Enter move: ";

	//enter a command
	//try to match the command with one in the list of moves
	//if the end of the list is reached
	//input command again until one is matched
	getline(cin, m);
	remove_carriage_return(m);
	assert(*m.rbegin() != '\0');
	list<move*>::iterator it = mlist.begin();
	while (it != mlist.end())
	{
		if ((*it)->command == m)
		{
			cout << "You have chosen the move: ";
			convertCommand((*it)->command);
			cout << endl;
			break;
		}
		++it;
		if (it == mlist.end())
		{
			getline(cin, m);
			remove_carriage_return(m);
			assert(*m.rbegin() != '\0');
			it = mlist.begin();
		}
	}

	//make the move selected
	makeMove(*it);
}

//print the board
//called by printEBoard
void board::printBoard()
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
	string lineEven = "   XXX|   |XXX|   |XXX|   |XXX|   ";		//padded 3 spaces
	string lineOdd = "      |XXX|   |XXX|   |XXX|   |XXX";		//padded 6 spaces
	string linebreak = "   -------------------------------";	//padded 3 spaces

	//print the board
	for (int i = 0; i != 8; ++i)
	{
		printline(i, lineEven, lineOdd);
		if (i != 7)
			cout << linebreak << endl;
	}

	//output a blank line before outputting moves
	cout << endl;
}

//decides whose turn it is to move based on color
//prints out all the legal moves for the current board
void board::printMoves()
{
	if (color == 'b')
		cout << "Player 1 to move." << endl;
	else cout << "Player 2 to move." << endl;
	cout << "The legal moves are:" << endl;
	list<move*>::const_iterator it = mlist.begin();
	for (; it != mlist.end(); ++it)
	{
		cout << "Move: ";
		convertCommand((*it)->command);
		cout << endl;
	}
	cout << endl;
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

//undoes a move
void board::undoMove(move* m)
{
	//replaces the starting jump point only if the starting jump hasn't already been replaced
	//iterate through its list of jumps
	//add back all the characters that were temporarily deleted
	if (!m->jpoints.empty())
	{
		for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
		{
			arr[(*it)->xs][(*it)->ys] = 'e';
			arr[(*it)->x][(*it)->y] = (*it)->c;
			if ((*it)->c == 'b')
				++piecesCount[0];
			else if ((*it)->c == 'r')
				++piecesCount[1];
			else if ((*it)->c == 'B')
			{
				++piecesCount[0];
				++kingCount[0];
			}
			//(*it)->c == 'R'
			else
			{
				++piecesCount[0];
				++kingCount[0];
			}
			arr[(*it)->xend][(*it)->yend] = 'e';
		}
	}
	arr[m->xf][m->yf] = 'e';
	//add the jumping piece in the start position of the move
	arr[m->xi][m->yi] = m->mP;
}

//black is more positive
//red is more negative
//aabbccdd
//aa is a count of all pieces: king is worth a total of 3, regular pieces are 2 (black - red)
//bb is measuring how close a normal piece is to becoming a king
//cc is a piece count difference
//dd is pseudo-random in the case that multiple moves tie for best
int board::evaluate()
{
	int a1 = ((piecesCount[0] * 2) + kingCount[0]) * 1000000;
	int a2 = ((piecesCount[1] * 2) + kingCount[1]) * 1000000;
	int b = 0;
	for (int i = 0; i != 8; ++i)
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == 'b')
				b += i;
			else if (arr[i][j] == 'r')
				b -= (7 - i);
		}
	b *= 10000;
	int c = (piecesCount[0] - piecesCount[1]) * 100;
	int d = rand() % 100;
	if (color == 'r')
		d = -d;
	return a1 - a2 + b + c + d;
}

//determines whether or not players will be a computer calls modifyBoard
void board::startup()
{
	//reset the board
	reset();
	whoComputer();
	bool b = true;
	cout << "Do you want to load a game from a file? (Y/N):" << endl;
	char c = ' ';
	while (b)
	{
		cin >> c;
		if (tolower(c) == 'y' || tolower(c) == 'n')
			b = false;
	}
	if (tolower(c) == 'y')
	{
		string name;
		cout << "Enter filename: " << endl;
		cin >> name;
		ifstream fin(name.c_str());
		while (!fin.good())
		{
			cout << "Enter filename: " << endl;
			cin >> name;
			fin.open(name.c_str());
		}
		modifyBoard(fin);
	}
}




