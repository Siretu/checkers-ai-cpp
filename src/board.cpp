/*
 * board.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: Harrison
 */

#include "board.h"

board::board(): color('b')
{
	piecesCount[0] = 12;
	piecesCount[1] = 12;
	kingCount[0] = 0;
	kingCount[1] = 0;
	for (int i = 0; i != 3; i++)
		for (int j = 0; j != 3; j++)
			arr[i][j] = 'b';
	for (int i = 3; i != 5; i++)
		for (int j = 0; j != 3; j++)
			arr[i][j] = 'e';
	for (int i = 5; i != 8; i++)
		for (int j = 0; j != 3; j++)
			arr[i][j] = 'r';
}






