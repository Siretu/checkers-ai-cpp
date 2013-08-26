//============================================================================
// Name        : checkersai.cpp
// Author      : Harrison Zhao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "board.h"
#include <fstream>
#include <list>

using namespace std;

int main()
{
	//cout << "Hello World!" << endl;
	board b;
	//list<move*> m;
	//ifstream fin("testfile2.txt");
	//b.modifyBoard(fin);
	b.terminalTest();
	b.printEBoard();
	board ba(b);
	ba.terminalTest();
	ba.printEBoard();
	/*list<move*>::iterator it = m.begin();
	for (; it != m.end(); it++)
		cout << (*it)->command << endl;*/
	//b.printEBoard(m);
	/*for (int i = 0; i != 100; i++)
	{
		b.terminalTest();
		b.printEBoard();
	}*/

	//should implement a smart pointer for move!!!!!!!!!
	/*while (!m.empty())
	{
		delete m.front();
		m.pop_front();
	}*/
	/*
	for (int i = 0; i != 100; i++)
	{
		b.terminalTest(m);
		b.printEBoard(m);
	}*/
	//messes up for multiple jumps needs fix
}
