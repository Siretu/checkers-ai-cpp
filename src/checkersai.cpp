//============================================================================
// Name        : checkersai.cpp
// Author      : Harrison Zhao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "board.h"
#include <list>

using namespace std;

int main()
{
	cout << "Hello World!" << endl;
	board b;
	list<move*> m;
	for (int i = 0; i != 100; i++)
	{
		b.terminalTest(m);
		b.printEBoard(m);
	}
	//messes up for multiple jumps needs fix
}
