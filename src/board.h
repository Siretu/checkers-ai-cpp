/*
 * board.h
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <list>

class board
{
	char arr[8][4];
	//first coord is x, second is y
	char color;
	//'b' for black, 'r' for red
	//indicates who's turn it is
	int piecesCount[2];
	//[0] for black, [1] for red
	int kingCount[2];
	//[0] for black, [1] for red
	board();
	//use default copy constructor for copying boards
	//then use makeMove
public:
	void listMoves(std::list<move*>&);
	void makeMove(const move&);
	bool isValidMove(const move&);
	void printBoard();	//expands and prints board
	void evaluate();	//Evaluation function
	bool jumpsAvailable();	//checks entire board for jumps
	bool jumpIsAvailable(point p);	//checks if a jump is available at the point
	bool moveIsAvailable(int, int);
	bool movesAvailable(); //don't know if necessary or not
	bool terminalTest()
	{
		if (piecesCount[0] == 0 || piecesCount[1] == 0 || movesAvailable() == false)
			return true;
		return false;
	}
	char getTurn() {return color;}
	void changeTurn()
	{
		if (color == 'r')
			color = 'b';
		else
			color = 'r';
	}

};

class move
{
public:
	point start;
	point end;
	bool isJump;
	std::list<point> jpoints;
	move(point s, point e): start(s), end(e), isJump(false) {}
};

class point
{
public:
	int x;
	int y;
	point(int i, int j): x(i), y(j) {}
};


#endif /* BOARD_H_ */
