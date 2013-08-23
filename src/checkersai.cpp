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
	b.terminalTest(m);
	b.printEBoard(m);
	b.terminalTest(m);
	b.printEBoard(m);
	b.terminalTest(m);
	cout << "works" << endl;
	b.printEBoard(m);
	b.terminalTest(m);
	b.printEBoard(m);
	//stops working after a few times??? whY?!?!?!?
	//JUMPS NOT WORKING!!!!!!!!!!! FIXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//delete b;
	//need to move moves out of board
}
