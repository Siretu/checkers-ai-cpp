//============================================================================
// Name        : checkersai.cpp
// Author      : Harrison Zhao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define NDEBUG
#include <iostream>
#include "board.h"
#include <fstream>
#include <list>
#include "game.h"

using namespace std;

int main()
{
	//create the game and play it
	//playTheGame function found in game.cpp
	game g;
	g.playTheGame();
}
