/*
 * boardPrint.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: Harrison
 */

#include <assert.h>
#include "board.h"
#include <iostream>
#include <list>
#include <string>
#include <windows.h>

using std::endl;
using std::cin;
using std::cout;
using std::getline;
using std::endl;
using std::list;
using std::string;

//converts a position on the compressed 8x4 matrix
//to a component of a command for the expanded 8x8 matrix
//string s is the command the point is appended to
//don't need to bound check because that has
//already been done when creating moves and jumps
//called by createJumpMove in boardJumps.cpp
//called by createMove in boardMove.cpp
void board::convert(const int& x, const int& y, string& s)
{
	//assert(0 <= x && x <= 7 && 0 <= y && y <= 3);
	 char c1 = '0' + x;
	 char c2;
	 if (x % 2 == 0)
	 {
		 c2 = '0' + (2*y + 1);
	 }
	 else
	 {
		 c2 = '0' + (2*y);
	 }
	 s += c1;
	 s += ' ';
	 s += c2;
	 s += ' ';
}

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

//prints a line of the board
//that does not contain any pieces
//i.e something like: XXX|   |XXX|   |XXX|   |XXX|
//cases vary by whether or not it's an even or odd row
//called by printBoard in boardPublic.cpp
void board::printline(const int& i, const string& lineEven, const string& lineOdd)
{
	if (i % 2 == 0)
	{
		cout << lineEven << endl;
		cout << " " << i << " XXX|";
		for (int j = 0; j != 3; ++j)
		{
			cout << " ";
			printcolor(arr[i][j]);
			cout << " |XXX|";
		}
		cout << " ";
		printcolor(arr[i][3]);
		cout << " " << endl;;
		cout << lineEven << endl;
	}
	else
	{
		cout << lineOdd << endl;
		cout << " " << i << " ";
		for (int j = 0; j != 3; ++j)
		{
			cout << " ";
			printcolor(arr[i][j]);
			cout << " |XXX|";
		}
		cout << " ";
		printcolor(arr[i][3]);
		cout << " |XXX" << endl;;
		cout << lineOdd << endl;
	}
}

//function for printing a character in a different color in windows
void board::printcolor(const char& c)
{
	if (c == 'e')
		cout << ' ';
	else if (c == 'r' || c == 'R')	//sets piece as red color
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		const int saved_colors = GetConsoleTextAttribute(hConsole);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << c;
		SetConsoleTextAttribute(hConsole, saved_colors);
	}
	else	//c is 'b' or 'B', sets pieces as green color
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		const int saved_colors = GetConsoleTextAttribute(hConsole);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << c;
		SetConsoleTextAttribute(hConsole, saved_colors);
	}
}


/*void board::whoComputer()
{
	char c = ' ';
	while (tolower(c) != 'y' || tolower(c) != 'n')
	{
		cout << "Will player # 1 be a computer? (Y/N):" << endl;
		cin >> c;
	}
	if (tolower(c) == 'y')
		board::isComputer[0] = true;
	else board::isComputer[0] = false;
	c = ' ';
	while (tolower(c) != 'y' || tolower(c) != 'n')
	{
		cout << "Will player # 2 be a computer? (Y/N):" << endl;
		cin >> c;
	}
	if (tolower(c) == 'y')
		board::isComputer[1] = true;
	else board::isComputer[1] = false;
}*/

