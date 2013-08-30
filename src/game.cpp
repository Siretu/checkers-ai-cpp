/*
 * game.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: Harrison
 */

#include <assert.h>
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

game::game(): currentB(sptr<board>(new board())), bestM(NULL), tempBestM(NULL), maxIterDepth(20), maxdepth(0),
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
	gameOver = true;
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
	//end game taken care of in printGame
	currentB->printMoves();
	cout << "The computer will make a move." << endl;
	if (currentB->mlist.size() == 1)
	{
		bestM = currentB->mlist.front();
		time(&startTime);
		time(&endTime);
		cdepth = 0;
	}
	else
	{
		time(&startTime);
		for (int i = 1; i != maxIterDepth; ++i)
		{
			maxdepth = i;
			alphabeta(currentB, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
			//assert(bestM != NULL);
			//cout << bestM->command << endl;
			if (timeUp)
				break;
			else
				bestM = tempBestM;
		}
	}
	assert(bestM != NULL);
	outputMessage();
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
	tempBestM = NULL;
	timeUp = false;
	reachedEnd = false;
}

void game::printGame()
{
	currentB->printBoard();
	if (currentB->terminalTest())
		endMessage();
	else if (currentB->isComputerTurn())
		computerTurn();
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
	if (depth != maxdepth && b->terminalTest())	//don't need to compute moves for depth 0
	{
		//b->printBoard();
		reachedEnd = true;
		if (b->getTurn() == 'r')
			return std::numeric_limits<int>::max();
		else return std::numeric_limits<int>::min();
	}
	if (depth == 0)
		return b->evaluate();
	list<move*>::iterator iter = b->mlist.begin();
	if (b->getTurn() == 'b')
	{
		for (; iter != b->mlist.end(); ++iter)
		{
			time(&endTime);
			if (difftime(endTime, startTime) >= (board::timeLimit - 1))
			{
				timeUp = true;
				break;
			}
			b->makeMove(*iter);
			sptr<board> newB(new board(*b));
			int value = alphabeta(newB, depth-1, alpha, beta);
			b->undoMove(*iter);
			b->changeTurn();
			if (value > alpha)	//found best move
			{
				alpha = value;
				if (depth == maxdepth)
					tempBestM = (*iter);
			}
			if (alpha >= beta)	//cut off
				return alpha;
		}
		if (!timeUp && depth == maxdepth)
			cdepth = depth;
		return alpha;
	}
	else // turn = 'r'
	{
		for (; iter != b->mlist.end(); ++iter)
		{
			time(&endTime);
			if (difftime(endTime, startTime) >= (board::timeLimit - 1))
			{
				timeUp = true;
				break;
			}
			b->makeMove(*iter);
			sptr<board> newB(new board(*b));
			int value = alphabeta(newB, depth-1, alpha, beta);
			b->undoMove(*iter);
			b->changeTurn();
			if (value < beta)	//found best move
			{
				beta = value;
				if (depth == maxdepth)
					tempBestM = (*iter);
			}
			if (alpha >= beta)	//cut off
				return beta;
		}
		if (!timeUp && depth == maxdepth)
			cdepth = depth;
		return beta;
	}
}
