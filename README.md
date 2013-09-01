Checkers in C++
============
This is a checkers game using an ASCII representation for the game board. The game features player vs computer
and computer vs computer modes. It utlizes iterative deepening and alpha-beta pruning with minimax to determine
which move the computer will make. The user can choose whether or not they want to load the game from a file.
The designated input file must look something like: (without the extra line of space between lines)

e e b e

e e e e

e e r e

e e e e

e R e B

e e e e

e e e e

e e e e

r

where the first 8 rows represent the actual board. There is no need for 64 positions since only half of the
squares are legal game spaces. R denotes a red king. B denotes a black king. r and b are red and black
normal pieces respectively. 'e' represents empty spaces. The 9th line of the input file must be either 
'r' or 'b' to denote whose turn it will be. The program uses escape sequences to print text in color.
Red pieces get printed in magenta and black pieces get printed in cyan.
