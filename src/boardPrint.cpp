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

void board::convert(const int& x, const int& y, string& s)
//converts an int to character form and returns it
//works fine
{
	//assert(0 <= x && x <= 7 && 0 <= y && y <= 3);
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

//used for computer's move and for selected move, to print out move
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
void board::inputCommand(list<move*>& mlist)		//need to modify this for computer, PROBLEM!!!!!!!!!
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
	makeMove(*it);
}

void board::printMoves(list<move*>& mlist)	//works fine
{
	list<move*>::const_iterator it = mlist.begin();
	for (; it != mlist.end(); ++it)
	{
		//cout << (*it)->command << endl;
		cout << "Move: (" << (*it)->xi << ", " << convertY((*it)->xi, (*it)->yi) << ")";
		if (!(*it)->jpoints.empty())
		{
			list<jump*>::const_iterator iter= (*it)->jpoints.begin();
			for (; iter != (*it)->jpoints.end(); ++iter)
				cout << " -> (" << (*iter)->xend << ", " << convertY((*iter)->xend, (*iter)->yend) << ")";
			cout << endl;
		}
		else
			cout << " -> (" << (*it)->xf << ", " << convertY((*it)->xf, (*it)->yf) << ")" << endl;
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

