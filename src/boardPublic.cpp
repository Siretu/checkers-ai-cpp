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
//move this functionality to the game.h header file and
//make printBoard public
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
			arr[(*it)->xend][(*it)->yend] = 'e';
		}
	}
	arr[m->xf][m->yf] = 'e';
	//add the jumping piece in the start position of the move
	arr[m->xi][m->yi] = m->mP;
}

void evaluate()
{

}

void board::startup()		//determines whether or not players will be a computer calls modifyBoard
{
	whoComputer();
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
}




