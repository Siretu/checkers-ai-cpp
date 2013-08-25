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
	list<move*> m;
	ifstream fin("testfile1.txt");
	b.modifyBoard(fin);
	b.terminalTest(m);
	/*list<move*>::iterator it = m.begin();
	for (; it != m.end(); it++)
		cout << (*it)->command << endl;*/
	b.printEBoard(m);
	b.terminalTest(m);
	b.printEBoard(m);

	//STACKDUMP OPENS FOR BRANCHING TESTFILE1.TXT WHY?
	//testfile 2 piece disappears!!!!!!!!, FIXEEEEEEEEEEDDD
	//In the case where there are different jump sequences available,
	//the player may chose which sequence to make, whether it results in the most pieces being taken or not.
	//need to fix this!!!!!!!!!! especially in the case of testfile2
	/*
	for (int i = 0; i != 100; i++)
	{
		b.terminalTest(m);
		b.printEBoard(m);
	}*/
	//messes up for multiple jumps needs fix
}
