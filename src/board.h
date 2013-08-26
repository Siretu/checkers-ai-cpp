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

class jump
{
public:
	//unique previous jump
	//example:
	//like in a diamond shape
	//			4
	//		3
	//	2		2'
	//		1
	//  1->2'->3->4 is a multiple jump
	// 	1->2 ->3->4 is another multiple jump
	// 	3->4 will be created twice, once for each path taken
	//	they will be exactly the same
	jump* prev;

	//when there are no next moves, noNext is true
	bool noNext;

	//used to keep track of how many times jump was inserted into a move,
	//increment every time jump is concatenated to a move
	//prevents double freeing of memory
	int numTimes;

	//prevents moves from getting created twice
	bool visited;

	//character jumped over
	char c;

	//start point's x and y coordinate
	int xs;
	int ys;

	//jumped character's point coordinates
	int x;
	int y;

	//end point of the jump
	int xend;
	int yend;

	//constructor for each data value
	jump(char piece, int xs, int ys, int xc, int yc, int xe, int ye, jump* p):
		prev(p), noNext(true), numTimes(0), visited(false), c(piece), xs(xs), ys(ys),
		 x(xc), y(yc), xend(xe), yend(ye){}
};

class move
{
public:
	//start point's coordinates
	int xi;
	int yi;

	//end point's coordinates
	int xf;
	int yf;

	//command used to map a string to the move
	//used for when the player enters a string for a move
	std::string command;

	//list storing all the jumps made by the move
	std::list<jump*> jpoints;

	//constructor for move
	move(int xs, int ys, int xe, int ye): xi(xs), yi(ys), xf(xe), yf(ye) {}

	//destructor for move, found in board.cpp
	//frees all the heap allocated memory for the jumps in jpoints
	~move();
};

class board
{
	std::list<move*> mlist;

	//first coordinate is x, second is y
	//don't need an 8x8 array since only half the spaces are legal positions for pieces
	char arr[8][4];

	//'b' for black, 'r' for red
	//indicates who's turn it is
	char color;

	//[0] for black, [1] for red
	int piecesCount[2];

	//[0] for black, [1] for red
	int kingCount[2];
	//counts are useful in an evaluation function

	//[0] for black, [1] for red
	//default initialized to false since it's a static array
	static bool isComputer[2];


	//
	//
	//functions for jumps: found in boardJumps.cpp
	//---------------------------------------------------------------------------------
	void createJump(std::list<jump*>&, char, int, int, int, int, int, int, jump*);

	void createJumpMove(std::list<jump*>&);

	void jumpAvailable(std::list<jump*>&, char c, int, int, jump*);

	//checks entire board for jumps and lists any moves with jumps, if there are any
	bool jumpsAvailable();

	bool jumpConditions(int, int, int, int);

	void checkJumpTR(std::list<jump*>&, int, int, jump*);

	void checkJumpTL(std::list<jump*>&, int, int, jump*);

	void checkJumpLR(std::list<jump*>&, int, int, jump*);

	void checkJumpLL(std::list<jump*>&, int, int, jump*);

	//reverses a jumping move
	void undoMove(move*);

	//---------------------------------------------------------------------------------


	//functions for regular moves, found in boardMoves.cpp
	//---------------------------------------------------------------------------------
	void checkNeighbors(int&, int&);

	void createMove(const int&, const int&, int, int);

	bool listMoves();
	//---------------------------------------------------------------------------------

	//general functions used for moves and jumps
	//---------------------------------------------------------------------------------
	//called in the terminalTest function
	//if there are any jumps, they are added to move list
	//else if there are any moves, they are added to the move list
	//if there are no jumps or moves available, it returns false
	bool movesAvailable()
	{
		if (jumpsAvailable())
			return true;
		if (listMoves())
			return true;
		return false;
	}

	//if a red piece 'r' reaches the red side's end, it becomes a red king 'R'
	//if a black piece 'b' reaches the black side's end, it becomes a black king 'B'
	//called at the end of the makeMove function, which is found in boardPublic.cpp
	void handleKinging(const int& x, const int& y)
	{
		if (x == 0 && arr[x][y] == 'r')
			arr[x][y] = 'R';
		if (x == 7 && arr[x][y] == 'b')
			arr[x][y] = 'B';
	}

	//returns true if the position arr[i][j] is a valid position on the checker board
	//called by jumpConditions, which is found in boardJumps.cpp
	//called by createMove, which is found in boardMoves.cpp
	bool isValidPos(int i, int j)
	{
		if (i >= 0 && i < 8 && j >= 0 && j < 4)
			return true;
		else return false;
	}

	//change turn, called after a move is made
	//called by makeMove, which is found in boardPublic.cpp
	void changeTurn()
	{
		if (color == 'r')
			color = 'b';
		else
			color = 'r';
	}
	//---------------------------------------------------------------------------------

	//functions for printing, found in boardPrint.cpp
	//---------------------------------------------------------------------------------
	//converts a point to string form and appends it to command list for a move
	//called by createJumpMove in boardJumps.cpp
	//called by createMove in boardMoves.cpp
	void convert(const int&, const int&, std::string&);

	//used for printing out moves, converting the y coordinate in the matrix
	//to the coordinate on the expanded 8x8 board
	//called in printMoves in boardPrint.cpp
	int convertY(const int& x, const int& y)
	{
		 if (x % 2 == 0)
			 return (2*y + 1);
		 else return (2*y);
	}

	//converts a command stored in the form 2 3 3 2 -1 to (2,3) -> (3, 2)
	//called in inputCommand in boardPrint.cpp
	void convertCommand(const std::string&);

	//prints out directions and available moves
	//need to add computer moves to it
	void inputCommand();

	//prints moves in order listed in the list
	//called by inputCommand in boardPrint.cpp
	void printMoves();

	//prints out a row of the checkers board
	//called by boardPrint in boardPublic.cpp
	void printline(const int&, const std::string&, const std::string&);

	//functions for printing lines and color characters in windows
	void printcolor(const char&);
	//change text color
	//found at http://msdn.microsoft.com/en-us/library/ms682088(VS.85).aspx#_win32_character_attributes
	//code for changing color found at http://www.cplusplus.com/forum/beginner/1640/
	//does not work in console, will work in executable

	WORD GetConsoleTextAttribute (HANDLE hCon)
	{
	  CONSOLE_SCREEN_BUFFER_INFO con_info;
	  GetConsoleScreenBufferInfo(hCon, &con_info);
	  return con_info.wAttributes;
	}
	//---------------------------------------------------------------------------------

	//modifies who is a computer, called by startup
	static void whoComputer();

public:

	//functions for board creation:
	//---------------------------------------------------------------------------------
	//constructor for initializing an initial board
	//found in board.cpp:
	board();
	//use default copy constructor for copying boards
	//then use makeMove

	~board();

	//create a board from an input file:
	//found in board.cpp
	void modifyBoard(std::ifstream&);
	//---------------------------------------------------------------------------------

	//create a list of moves by calling this
	//should be called each time a new board gets created after a move is made
	bool terminalTest()
	{
		if (!movesAvailable() || (color == 'b' && piecesCount[0] == 0) ||
				(color == 'r' && piecesCount[1] == 0))
			return true;
		return false;
	}
	//---------------------------------------------------------------------------------
	//functions found in boardPublic.cpp
	//---------------------------------------------------------------------------------
	//prints everything necessary, calls printBoard and inputCommand
	//also prints out a game over message if applicable
	void printEBoard();

	//makes the move
	//should be used on a copy of a board when alpha-beta searching
	void makeMove(move*);

	//expands and prints board
	//called by printEBoard
	void printBoard();

	//Evaluation function, need to do
	//will be implemented in alpha-beta search
	void evaluate();

	//determines whether or not players will be a computer calls modifyBoard
	void startup();
	//NEED TO IMPLEMENT TIMER STUFF

	char getTurn() {return color;}
};

//function for fixing strings obtained via getline
//called by modifyBoard in board.cpp
inline void remove_carriage_return(std::string& line)
//eliminate the \r character in a string or the \0 character
{
    if (*line.rbegin() == '\r' || *line.rbegin() == '\0')
    {
        line.erase(line.length() - 1);
    }
}



#endif /* BOARD_H_ */
