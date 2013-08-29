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
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::list;

game::game(): currentB(sptr<board>(new board())), bestM(NULL), maxIterDepth(30), maxdepth(0),
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
	if (currentB->terminalTest())
	{
		endMessage();
		return;
	}
	currentB->printMoves();
	cout << "The computer will make a move." << endl;
	if (currentB->mlist.size() == 1)
	{
		bestM = currentB->mlist.front();
		time(&startTime);
		time(&endTime);
		cdepth = 0;
		outputMessage();
		return;
	}
	time(&startTime);
	for (int i = 0; i != maxIterDepth; ++i)
	{
		maxdepth = i;
		alphabeta(currentB, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		if (gameOver || timeUp)
			break;
	}
	if (!gameOver)
		outputMessage();
	else
		endMessage();
}

void game::outputMessage()
{
	currentB->makeMove(bestM);
	cout << "Completed search to depth " << cdepth << "." << endl;
	if (timeUp && cdepth != maxIterDepth && !reachedEnd) //or no way to get to maxdepth since gamespace end has been reached
		cout << "Out of time searching to depth " << cdepth + 1 << "." << endl;
	cout << "Searched for a total of " << difftime(endTime, startTime) << " seconds" << endl;
	cout << "The chosen move is: ";
	board::convertCommand(bestM->command);
	cout << endl;
	bestM = NULL;
	timeUp = false;
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

//need to debug a lot
//time spent searching???
//fix the algorithm to play better!!!!
//utilize reached end
int game::alphabeta(sptr<board>& b, int depth, int alpha, int beta)
{

	//char turn = b->getTurn();
	if (b->terminalTest() && depth != maxdepth)
		return std::numeric_limits<int>::min();
	if (depth == 0)
		return b->evaluate();
	int localalpha = alpha;
	int bestvalue = std::numeric_limits<int>::min();
	list<move*>::iterator iter = b->mlist.begin();
	//cout << "works up to here" << endl;
	for (; iter != b->mlist.end(); ++iter)
	{
		time(&endTime);
		if (difftime(endTime, startTime) >= (board::timeLimit - 1))
		{
			timeUp = true;
			break;
		}
		b->makeMove(*iter);
		//cout << "works here too" << depth << endl;
		sptr<board> newB(new board(*b));
		int value = -alphabeta(newB, depth-1, -beta, -localalpha);
		b->undoMove(*iter);
		b->changeTurn();
		bestvalue = max(value, bestvalue);
		if (bestvalue >= beta)
			break;
		if (bestvalue > localalpha)
		{
			localalpha = bestvalue;
			if (depth == maxdepth)
				bestM = (*iter);
		}
		//need to set bestM to something here
	}
	if (!timeUp && depth == maxdepth)
		cdepth = depth;
	//fix here, somehow make it return the best move
	return bestvalue;
}
