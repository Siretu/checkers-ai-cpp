/*
 * board.h
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <list>
#include <fstream>
#include <utility>	//for std::pair

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
public:
	board();
	//use default copy constructor for copying boards
	//then use makeMove
	void modifyBoard(const std::ifstream&); //create a board from an input file
	void makeMove(const move&);
	void printBoard();	//expands and prints board
	void evaluate();	//Evaluation function
	void deleteMoveslist(std::list<move*>& mlist);
//do
	void createMove(std::list<move*>& mlist,const int& xi,const int& yi, int xf, int yf)
	{
		if (isValidPos(xf, yf) && arr[xf][yf] == 'e')
			mlist.push_back(new move(xi, yi, xf, yf));
	}

	void checkNeighbors(std::list<move*>&, int&, int&);	//check for move forward and backwards
	//will do appropriate version for a king
	bool listMoves(std::list<move*>&); //need to do
	bool jumpsAvailable(std::list<move*>&);	//checks entire board for jumps and list them if there are any
	void checkJump(std::list<move*>&, int&, int&);
	//checks if a jump is available at the point

	bool isKing(int& x, int& y)
	{
		if (arr[x][y] == toupper(color))
			return true;
		return false;
	}
	//judges if it's a king for the current color's turn

	void undoMove(move*); //used to reverse a jump
//end do
	bool movesAvailable(std::list<move*>& mlist)
	{
		if (jumpsAvailable(mlist))
			return true;
		if (listMoves(mlist))
			return true;
		return false;
	}

	bool isValidPos(int i, int j)
	{
		if (i >= 0 && i < 8 && j >= 0 && j < 8)
			return true;
		else return false;
	}

	bool terminalTest(std::list<move*>&); // test for end, done

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
	int xi;
	int yi;
	int xf;
	int yf;
	std::list<cJumped> jpoints;
	move(int xs, int ys, int xe, int ye): xi(xs), yi(ys), xf(xe), yf(ye) {}
	~move();
};

class cJumped
{
public:
	char c;
	int x;
	int y;
	cJumped(char p, int xc, int yc): c(p), x(xc), y(yc) {}
};


#endif /* BOARD_H_ */
