/*
 * game.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Harrison
 */

#include <algorithm>
#include <cctype>
#include <iostream>
#include "game.h"
#include <iostream>
#include <limits>
#include <list>
#include "board.h"
#include "time.h"

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::list;

game::game(): currentB(sptr<board>(new board())), bestM(NULL), maxIterDepth(0), maxdepth(0),
		cdepth(0), timeUp(false), gameOver(false), reachedEnd(false), startTime(0), endTime(0) {}

void game::playTheGame()
{
	gameOver = false;
	currentB->startup();
	while (!gameOver)
		printGame();
}

void game::endMessage()
{
	cout << "The game is over." << endl;
	cout << endl;
	if (currentB->getTurn() == 'r')
		cout << "Player 1 wins." << endl;
	else cout << "Player 2 wins." << endl;
	cout << "Do you want to play again? (Y/N):" << endl;
	char answer;
	cin >> answer;
	bool loop = true;
	while (loop)
	{
		if (tolower(answer) == 'y')
		{
			loop = false;
			playTheGame();
		}
		else if (tolower(answer) == 'n')
			loop = false;
		else
		{
			cout << "Do you want to play again? (Y/N):" << endl;
			cin >> answer;
		}
	}
}

void game::computerTurn()
{
	time(&startTime);
	for (int i = 0; i != maxIterDepth; ++i)
	{
		maxdepth = i;
		alphabeta(currentB, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		if (gameOver || timeUp)
			break;
	}
	if (!gameOver)
	{
		currentB->makeMove(bestM);
		cout << "The computer will make a move." << endl;
		cout << "Completed search to depth " << cdepth << "." << endl;
		if (timeUp && cdepth != maxdepth && !reachedEnd) //or no way to get to maxdepth since gamespace end has been reached
			cout << "Out of time searching to depth " << cdepth + 1 << "." << endl;
		cout << "Searched for a total of " << endTime - startTime << " seconds";
		cout << "The chosen move is: ";
		board::convertCommand(bestM->command);
		cout << endl;
	}
	else
		endMessage();
}

void game::printGame()
{
	currentB->printBoard();
	if (currentB->isComputerTurn())
		computerTurn();
	else if (currentB->terminalTest())
		endMessage();
	else
		currentB->inputCommand();
}
/*
 * The computer will make a move.
 * Completed search to depth _
 * Out of time searching to depth (next depth)
 * Searched for a total of t seconds
 * The chosen move is: (move)
 */
//start at max depth work toward 0
//almost done with this
//implement timer
//save the best move??
int game::alphabeta(sptr<board>& b, int depth, int alpha, int beta)
{

	char turn = b->getTurn();
	if (b->terminalTest())
	{
		if (depth == maxdepth)
			gameOver = true;
		return std::numeric_limits<int>::min();
	}
	if (depth == 0)
		return b->evaluate();
	int localalpha = alpha;
	int bestvalue = std::numeric_limits<int>::min();
	list<move*>::iterator iter = b->mlist.begin();
	for (; iter != b->mlist.end(); ++iter)
	{
		//incorporate time here
		b->makeMove(*iter);
		sptr<board> newB(new board(*b));
		int value = -alphabeta(newB, depth-1, -beta, -localalpha);
		b->undoMove(*iter);
		b->changeTurn();
		bestvalue = max(value, bestvalue);
		if (bestvalue >= beta)
			break;
		if (bestvalue > localalpha)
			localalpha = bestvalue;
		//need to set bestM to something here
	}
	if (iter == b->mlist.end())
		if (maxdepth - depth > cdepth)
			cdepth = maxdepth - depth;
	if (depth == maxdepth)
	{
		b->makeMove(bestM);
		cdepth = 0;
	}
	//fix here, somehow make it return the best move
	return bestvalue;
}
