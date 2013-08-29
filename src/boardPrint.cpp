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

//called by startup, which is found in board.h
//prompts user to assign who is/ is not a computer
void board::whoComputer()
{
	bool b = true;
	char c = ' ';
	while (b)
	{
		cout << "Will player # 1 be a computer? (Y/N):" << endl;
		cin >> c;
		if (tolower(c) == 'y')
		{
			board::isComputer[0] = true;
			b = false;
		}
		else if (tolower(c) == 'n')
		{
			board::isComputer[0] = false;
			b = false;
		}
	}
	b = true;
	c = ' ';
	while (b)
	{
		cout << "Will player # 2 be a computer? (Y/N):" << endl;
		cin >> c;
		if (tolower(c) == 'y')
		{
			board::isComputer[1] = true;
			b = false;
		}
		else if (tolower(c) == 'n')
		{
			board::isComputer[1] = false;
			b = false;
		}
	}
	if (board::isComputer[0] == true || board::isComputer[1] == true)
	{
		cout << "Enter a time limit for the computer in seconds (3-60):" << endl;
		cin >> timeLimit;
		if (timeLimit > 60 || timeLimit < 3)
			cin >> timeLimit;
	}
}

