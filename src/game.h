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

//template for basic smart pointer
//used to wrap pointers to board
//makes memory freeing easy to handle
//in alpha beta searching and creating new temporary boards
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

	//start and end times for search to a certain depth
	//if the most recent search took too long, it will not alpha beta search to next depth
	time_t startTimeD;
	time_t endTimeD;
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

	//alpha beta searched called by computerTurn
	int alphabeta(sptr<board>&, int, int, int);

	//message that prints when the game is over
	void endMessage();

public:

	//constructor, initializes data members
	game();

	//calls startup
	//calls appropriate functions to play checkers
	void playTheGame();

};


#endif /* GAME_H_ */
