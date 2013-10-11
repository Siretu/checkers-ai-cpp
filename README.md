Checkers in C++
============
This is a checkers game using an ASCII representation for the game board. 

How does it work?
============
The checker board is represented using a 8 x 4 matrix of characters. 32 positions are needed instead of 64 positions since only half 
of the squares are legal game spaces. This program utilizes iterative deepening and alpha-beta pruning with minimax to determine which move the computer will
make. A heuristic function is utilized for assigning values to states generated in the alpha-beta minimax tree. Player 1 is black and player 2
is red. Player 1 tries to maximize the heuristic function while player 2 tries to minimize it. The heuristic evaluates a state
where player 1 loses as -infinity while it evaluates a state where player 2 loses as +infinity. 
There are four instances in which a terminal state can occur: 
  - 1. Red's (R) pieces are reduced to 0.
  - 2. Black's (B) pieces are reduced to 0.
  - 3. Red has no available moves left.
  - 4. Black has no available moves left.
  

What modes of play does it feature?
============
The game features player vs computer and computer vs computer modes. The computer can be allotted between 3 to 60 seconds
to decide it's move. There is no time limit for a human player to determine his or her move. The game can also be loaded from
a file. The designated input file must look something like:]

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
Red pieces get printed in magenta and black pieces get printed in cyan. 
Note: Run the program's executable in terminal instead of in an IDE (such as Eclipse's) console
since the escape sequences may also be printed in the IDE's console.

Where is it available?
============
The executable is available to download here: 
http://www.mediafire.com/download/ftg19vgd8ezawdp/checkersai.exe

Note: it is compiled with cygwin for windows!
Please read the above modes of play to load a text file into the game and for general information about the game.
