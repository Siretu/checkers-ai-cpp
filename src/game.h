/*
 * game.h
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#ifndef GAME_H_
#define GAME_H_

#include "board.h"
#include "time.h"

template <class T>
class sptr
{
private:
	T* pData;
public:
	sptr(T* pVal): pData(pVal) {}
	~sptr()	{delete pData;}
	T& operator*() {return *pData;}
	T* operator->() {return pData;}
};

class game
{
	//black shall be max
	//red shall be min

	//maintains a pointer to the current board
	sptr<board> currentB;

	//upper bound for maximum depth that can be reached by alpha beta
	//highly unlikely to reach depth 20 within the given time limit
	static const int maxIterDepth;

	//best move from last complete alpha-beta search
	move* bestM;

	//temporary best move for current alpha-beta search
	move* tempBestM;

	//keeps track of the max depth of the current alpha-beta search
	int maxdepth;

	//current depth fully explored of alpha-beta search
	int cdepth;

	//tells whether or not total search time has elapsed
	//if it has, terminate the current search and use the previous best move
	bool timeUp;

	//becomes true if the game is over
	//condition that loops the game when false and creates more turns
	//set this to false when calling playTheGame()
	bool gameOver;

	//reached end of game space
	//marker set to true during end game when terminalTest
	//for a child node has been reached
	//disables the "out of time searching" to next depth message
	bool reachedEnd;

	//start and end times
	//uses time and diff time
	//to find the time elapsed in calendar time seconds
	time_t startTime;
	time_t endTime;

	//prints the game board and prompts user for appropriate move
	//if it's a computer's turn, it starts the alpha-beta search and
	//selects the computer's move
	//it is continuously called when gameOver is false
	void printGame();

	//for computer's turn
	//prints depth searched up to, whether or not time ran out, and computer's move
	void outputMessage();

	//everything related to the computer's turn
	void computerTurn();

	int alphabeta(sptr<board>&, int, int, int);
	void endMessage();

	//modifies max depth based on time left
	void modifyMaxIterDepth();

public:

	game();
	void playTheGame(); //calls startup



	//int alphabeta(POSITION *p, int depth, int alpha, int beta)
	//	{
	//	MOVE list[MAXMOVES];
	//	int i,n,value,localalpha=alpha,bestvalue=-INFINITY;
	//
	//	if(checkwin(p))
	//		return -INFINITY;
	//
	//	if(depth == 0)
	//		return evaluation(p);

	//	n = makemovelist(p,list);
	//	if(n == 0)
	//		return handlenomove(p);

	//	for(i=0; i<n; i++)
	//		{
	//		domove(&list[i],p);
	//		value = -alphabeta(p,d-1,-beta,-localalpha);
	//		undomove(&list[i],p);
	//		bestvalue = max(value,bestvalue);
	//		if(bestvalue>=beta)
	//			break;
	//		if(bestvalue>localalpha)
	//			localalpha=bestvalue;
	//		}
	//	return bestvalue;
	//	}

	//if (isComputerTurn() == 1) do max's stuff
	//if (isComputerTurn() == 2) do min's stuff
	//use copy constructor for board
	//after each call of undoMove, call changeTurn
	//change turn is already called in makeMove
	//pass by pointer
	//before return best value, delete the new bo	ard allocated

	//need a variable depth that depends on time limit
	//incorporate time limit too
	//before return best value
	//if (depth == 0) {makeMove (*it))
	//else delete (heap allocated board and return bestvalue)

	/*
	 alpha-beta(player,board,alpha,beta)
    if(game over in current board position)
        return winner

    children = all legal moves for player from this board
    if(max's turn)
        for each child
            score = alpha-beta(other player,child,alpha,beta)
            if score > alpha then alpha = score (we have found a better best move)
            if alpha >= beta then return alpha (cut off)
        return alpha (this is our best move)
    else (min's turn)
        for each child
            score = alpha-beta(other player,child,alpha,beta)
            if score < beta then beta = score (opponent has found a better worse move)
            if alpha >= beta then return beta (cut off)
        return beta (this is the opponent's best move)
	 */
};


#endif /* GAME_H_ */
