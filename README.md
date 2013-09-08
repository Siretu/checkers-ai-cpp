Checkers in C++
============
This is a checkers game using an ASCII representation for the game board. 

How does it work?
============
The checker board is represented using a 8 x 4 matrix of characters. There is no need for 64 positions since only half 
of the squares are legal game spaces. This program utilizes iterative deepening and alpha-beta pruning with minimax to determine which move the computer will
make.

What modes of play does it feature?
============
The game features player vs computer and computer vs computer modes. The computer can be allotted between 3 to 60 seconds
to decide it's move. There is no time limit for a human player to determine his or her move. The game can also be loaded from
a file. The designated input file must look something like: (without the extra line of space between lines)

e e b e

e e e e

e e r e

e e e e

e R e B

e e e e

e e e e

e e e e

r

where the first 8 rows represent the actual board. R denotes a red king. B denotes a black king. r and b are red and black
normal pieces respectively. 'e' represents empty spaces. The 9th line of the input file must be either 
'r' or 'b' to denote whose turn it will be. The program uses escape sequences to print text in color.
Red pieces get printed in magenta and black pieces get printed in cyan. Don't run it in console, run it as an executable
since the escape sequences will also be printed in console.

Where is it available?
============
The executable is available to download here: http://www.mediafire.com/download/05c4dyng6425hiq/checkersai.exe
Note: it is compiled under windows!
Please read the above modes of play to load a text file into the game and for general information about the game.
