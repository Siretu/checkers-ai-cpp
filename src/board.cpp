/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */
//#define NDEBUG
#include <assert.h>
#include "board.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <windows.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::string;
using std::stringstream;
using std::toupper;

//public constructors
board::board(char c = 'b'): color(c)
{
	piecesCount[0] = 12;
	piecesCount[1] = 12;
	kingCount[0] = 0;
	kingCount[1] = 0;
	for (int i = 0; i != 3; ++i)
		for (int j = 0; j != 3; ++j)
			arr[i][j] = 'b';
	for (int i = 3; i != 5; ++i)
		for (int j = 0; j != 3; ++j)
			arr[i][j] = 'e';
	for (int i = 5; i != 8; ++i)
		for (int j = 0; j != 3; ++j)
			arr[i][j] = 'r';
}

board::~board()		//deletes all the moves allocated by new in the list
{
	while (!mlist.empty())
	{
		delete mlist.front();
		mlist.pop_front();
	}
}

move::~move()
{
	while(!jpoints.empty())
	{
		delete jpoints.front();
		jpoints.pop_front();
	}
}

//private member functions
void board::createJump(std::list<jump*>& jlist, int xj, int yj, int xe, int ye, jump* jp)
{
	jump* j = new jump(arr[xj][yj], xj, yj, xe, ye, jp);
	arr[xj][yj] = 'e';	//deletes the character temporarily
	recurseInc(j);
	if (jp != NULL)
		jp->noNext = false;
	jlist.push_front(j);	//pushes the front, iterate from start to end, last jump is in front
	jumpAvailable(jlist, xe, ye, j);
}

void board::createJumpMove(list<jump*>& jlist, const int& x, const int& y)
{
	while (!jlist.empty())
	{
		jump* jp = jlist.front();
		move* m = new move(x, y, -1, -1);
		while (jp->prev != NULL)
		{
			m->jpoints.push_front(jp);	//reverse ordering, so that last jump is at the end
			--jp->numtimes;
			if (jp->numtimes == 0)
				jlist.remove(jp);
			jp = jp->prev;
		}
		convert(x, y, m->command);
		for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
		{
			convert((*it)->xend, (*it)->yend, m->command);
			undoJump(*it);	//undoes each jump, replaces the 'e's with original characters
			if ((*it)->noNext)	//if there's no next move for the jump
			{
				m->xf = (*it)->xend;
				m->yf = (*it)->yend;
			}
		}
		if(m->xf != -1 && m->yf != -1)
		{
			mlist.push_back(m);
			m->command += "-1";
		}
		else
		{
			assert(m->jpoints.empty());
			delete m;
		}
	}
	assert(jlist.empty());
}

void board::jumpAvailable(list<jump*>& jlist, int x, int y, jump* jp= NULL)	//i, j are start points
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x+1, y, x+2, y-1))	//checks left down jump
				createJump(jlist, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x+1, y+1, x+2, y+1))	//checks right down jump
				createJump(jlist, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x+1, y-1, x+2, y-1))	//checks left down jump
				createJump(jlist, x+1, y-1, x+2, y-1, jp);
			if (jumpConditions(x+1, y, x+2, y+1))	//checks right down jump
				createJump(jlist, x+1, y, x+2, y+1, jp);
		}
	}
	if (color == 'r' || arr[x][y] == 'B')
	{
		if (x % 2 == 0)	//even x
		{
			if (jumpConditions(x-1, y, x-2, y-1))	//checks left up jump
				createJump(jlist, x+1, y, x+2, y-1, jp);
			if (jumpConditions(x-1, y+1, x-2, y+1))	//checks right up jump
				createJump(jlist, x+1, y+1, x+2, y+1, jp);
		}
		else	//odd x
		{
			if (jumpConditions(x-1, y-1, x-2, y-1))	//checks left up jump
				createJump(jlist, x-1, y-1, x-2, y-1, jp);
			if (jumpConditions(x-1, y, x-2, y+1))	//checks right up jump
				createJump(jlist, x-1, y, x-2, y+1, jp);
		}
	}
}

bool board::jumpsAvailable()
{
	list<jump*> jlist;
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
			{
				jumpAvailable(jlist, i, j);
				createJumpMove(jlist, i, j);
			}
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

void board::recurseInc(jump* j)	//recursively increment numtimes
{
	++j->numtimes;
	jump* jp = j;
	while (j->prev != NULL)
	{
		jp = j->prev;
		++jp->numtimes;
	}
}

void board::checkNeighbors(int& x, int& y)
{
	if (color == 'b' || arr[x][y] == 'R')
	{
		//if it's odd subtract or stay same for y coordinate
		//if it's even add or stay same
		//will always add x coordinate
		//if it's a king (on appropriate turn), check backwards too
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
	if (color == 'r' || arr[x][y] == 'B')
	{
		//if it's even add or stay the same
		//if it's odd subtract or stay the same
		//will always subtract x coordinate
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

bool board::listMoves()	//returns true if there are any regular moves
{
	for (int i = 0; i!= 8; ++i)
	{
		for (int j = 0; j != 4; ++j)
		{
			if (arr[i][j] == color || arr[i][j] == toupper(color))
				checkNeighbors(i, j);
		}
	}
	if (mlist.empty())
		return false;
	return true;
}

void board::modifyBoard(ifstream& fin)
{
	//store it all in a list
	//parse each line
	//modify the board per line
	//last line (9th line will be turn)
	//a sample line of input would be
	//e b b e
	string line;
	int count = 0;
	while (getline(fin, line) && count != 8)
	{
		remove_carriage_return(line);
		stringstream ss(line);
		for (int jIndex = 0; jIndex != 4; ++jIndex)
			ss >> arr[count][jIndex];
		++count;
	}
	getline(fin, line);
	remove_carriage_return(line);
	stringstream ss(line);
	ss >> color;
	assert(color == 'b' || color == 'r');
}

//functions for outputting commands
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

void board::inputCommand()		//need to modify this for computer
{
	string m;
	cout << endl;
	if (color == 'b')
		cout << "Player 1 to move." << endl;
	else cout << "Player 2 to move." << endl;
	cout << "The legal moves are:" << endl;
	printMoves();
	cout << endl;
	//edit below here, test if it's a computer, if it isn't run the below lines
	//else run something else
	cout << "Enter a sequence of integers indicating a move." << endl;
	cout << "Each set of two integers represents a position." << endl;
	cout << "End the sequence with -1." << endl;
	cout << "For example: " << (mlist.front())->command << " represents " <<
			convertCommand(mlist.front()->command) << endl;
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

//
//
//
//public member functions:

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
	arr[m->xf][m->yf] = arr[m->xi][m->yi];	//move the piece
	arr[m->xi][m->yi] = 'e';				//replace original position with 'e' (for empty)
	handleKinging(m->xf, m->yf);
}

void board::printBoard()
{
	cout << "Current board:" << endl;
	cout << endl;
	cout << "Player 1 is " << printcolor('b') << " (normal piece) and " << printcolor('B') <<
			" (king)" << endl;
	cout << "Player 2 is " << printcolor('r') << " (normal piece) and " << printcolor('R') <<
			" (king)" << endl;
	int count = 0;
	cout << "    " << count;
	while (count != 8)
	{
		cout << "   " << ++count;
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
}

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
}
//miscellaneous functions for parsing
inline void remove_carriage_return(string& line)
//eliminate the \r character in a string
//this is needed in some cases of getline
{
    if (*line.rbegin() == '\r')
    {
        line.erase(line.length() - 1);
    }
}
