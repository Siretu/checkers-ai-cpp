/*
 * board.h
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <windows.h>


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

	//functions for jumps:
	void createJump(std::list<jump*>&, int, int, int, int, jump*);

	void createJumpMove(std::list<move*>&, std::list<jump*>&, const int&, const int&);

	void jumpAvailable(std::list<jump*>&, int, int, jump*);

	bool jumpsAvailable(std::list<move*>&);	//checks entire board for jumps and list them if there are any

	bool jumpConditions(int xj, int yj, int xe, int ye)
	{
		if (isValidPos(xj, yj) && isValidPos(xe, ye) && arr[xj][yj] != 'e' &&
				arr[xj][yj] != color && arr[xe][ye] == 'e' &&  arr[xj][yj] != std::toupper(color))
			return true;
		return false;
	}

	void recurseInc(jump*);

	void undoJump(jump* j) //used to reverse a jump
	{
		arr[j->x][j->y] = j->c;
	}

	//functions for regular moves:
	void checkNeighbors(std::list<move*>&, int&, int&);

	void createMove(std::list<move*>& mlist,const int& xi,const int& yi, int xf, int yf)
	{
		if (isValidPos(xf, yf) && arr[xf][yf] == 'e')
			mlist.push_back(new move(xi, yi, xf, yf));
	}

	bool isValidPos(int i, int j)
	{
		if (i >= 0 && i < 8 && j >= 0 && j < 8)
			return true;
		else return false;
	}

	bool listMoves(std::list<move*>&);

	bool movesAvailable(std::list<move*>& mlist)
	{
		if (jumpsAvailable(mlist))
			return true;
		if (listMoves(mlist))
			return true;
		return false;
	}

	//functions for printing lines and color characters in windows
	void printcolor(const char&);
	//change text color
	//found at http://msdn.microsoft.com/en-us/library/ms682088(VS.85).aspx#_win32_character_attributes
	//code for changing color found at http://www.cplusplus.com/forum/beginner/1640/

	WORD GetConsoleTextAttribute (HANDLE hCon)
	{
	  CONSOLE_SCREEN_BUFFER_INFO con_info;
	  GetConsoleScreenBufferInfo(hCon, &con_info);
	  return con_info.wAttributes;
	}

	void printline(const int&, const std::string&, const std::string&);

public:

	board(char c);
	//use default copy constructor for copying boards
	//then use makeMove

	bool board::terminalTest(list<move*>& mlist)	//use this in conjunction with color member
	//like if terminalTest and color = 'b' / 'r'
	//call terminal test first, movesAvailable will automatically create a list of moves
	//test for end
	{
		if (!movesAvailable(mlist))
			return true;
		if (color == 'b' && piecesCount[0] == 0)
			return true;
		if (color == 'r' && piecesCount[1] == 0)
			return true;
		return false;
	}

	void makeMove(move*);	//need to do
	void modifyBoard(std::ifstream&); //create a board from an input file
	void printBoard();	//expands and prints board
	void printMoves(const std::list<move*>&);
	void evaluate();	//Evaluation function, need to do
	void deleteMoveslist(std::list<move*>& mlist); //check

	bool isKing(int& x, int& y)
	{
		if (arr[x][y] == toupper(color))
			return true;
		return false;
	}
	//judges if it's a king for the current color's turn

//end do

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
	std::list<jump*> jpoints;
	move(int xs, int ys, int xe, int ye): xi(xs), yi(ys), xf(xe), yf(ye) {}
	~move();
};

class jump
{
public:
	jump* prev;				//unique previous jump, if it looks repetitive
	//like in a diamond shape
	//			4
	//		3
	//	2		2'
	//		1
	//  1->2'->3->4 is a multiple jump
	// 	1->2 ->3->4 is another multiple jump
	// 	3->4 will be created twice, once for each path taken
	//	they will be exactly the same
	bool noNext;	//when there are no next moves, noNext is true
	int numtimes; //number times the jump was utilized (for branching scenarios
	//when it hits zero, delete it because it's done
	char c;
	int x;		//jumped character point
	int y;
	int xend;	//x endpoint
	int yend;	//y endpoint
	jump(char piece, int xc, int yc, int xe, int ye, jump* p):
		c(piece), x(xc), y(yc), xend(xe), yend(ye), prev(p), numtimes(0), noNext(true) {}
};


#endif /* BOARD_H_ */
