checkers.exe: checkersai.o board.o boardJumps.o boardMoves.o boardPrint.o boardPublic.o game.o
	g++ -o checkers.exe checkersai.o board.o boardJumps.o boardMoves.o boardPrint.o boardPublic.o game.o

checkerai.o: checkersai.cpp board.h game.h
	g++ -c checkersai.cpp

board.o: board.cpp board.h
	g++ -c board.cpp

boardJumps.o: boardJumps.cpp board.h
	g++ -c boardJumps.cpp

boardMoves.o: boardMoves.cpp board.h
	g++ -c boardMoves.cpp

boardPrint.o: boardPrint.cpp board.h
	g++ -c boardPrint.cpp

boardPublic.o: boardPublic.cpp board.h
	g++ -c boardPublic.cpp

game.o: game.cpp game.h board.h
	g++ -c game.cpp
	
debug:
	g++ -g -o checkersDebug.exe checkersai.cpp board.cpp boardJumps.cpp boardMoves.cpp boardPrint.cpp boardPublic.cpp game.cpp

clean:
	rm *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups