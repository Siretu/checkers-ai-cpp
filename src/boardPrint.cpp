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

using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::string;

void board::convert(const int& x, const int& y, string& s)	//converts an int to character form and returns it
{
	assert(0 <= x && x <= 7 && 0 <= y && y <= 3);
	if( 0 <= x && x <= 7 && 0 <= y && y <= 3)
	{
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
}

//functions for outputting commands
void board::inputCommand()		//need to modify this for computer
{
	string m;
	//edit below here, test if it's a computer, if it isn't run the below lines
	//else run something else
	cout << "Enter a sequence of integers indicating a move." << endl;
	cout << "Each set of two integers represents a position." << endl;
	cout << "End the sequence with -1." << endl;
	cout << "For example: 2 3 3 2 -1" << endl;
	cout <<	"	represents (2,3) -> (3,2)" << endl;
	cout << "Enter move: ";
	cin >> m;
	assert(*m.rbegin() != '\r');
	list<move*>::iterator it = mlist.begin();
	while (m != (*it)->command)
	{
		++it;
		if (it == mlist.end())
		{
			cin >> m;
			assert(*m.rbegin() != '\r');
			it = mlist.begin();
		}
	}
	makeMove(*it);
}

void board::printMoves()
{
	list<move*>::const_iterator it = mlist.begin();
	for (; it != mlist.end(); ++it)
	{
		cout << "Move: (" << (*it)->xi << ", " << (*it)->yi << ") -> ";
		if (!(*it)->jpoints.empty())
		{
			list<jump*>::const_iterator iter= (*it)->jpoints.begin();
			for (; iter != (*it)->jpoints.end(); ++iter)
				cout << "(" << (*iter)->xend << ", " << (*iter)->yend << ") -> ";
		}
		cout << "(" << (*it)->xf << ", " << (*it)->yf << ")" << endl;
	}
}

//function for printing a character in a different color in windows
void board::printcolor(const char& c)
{
	if (c == 'e')
		cout << " ";
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
		cout << lineEven;
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

void board::whoComputer()
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
}

