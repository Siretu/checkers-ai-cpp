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
  
What does each class do?
============
The board class holds a representation of the checkerboard using an 8 x 4 matrix of characters. 32 positions are needed instead of 64 positions since only half of the squares are legal game spaces. The board class also keeps track of turns, time limits, and whether or not a player is a computer. The board class has member functions that create moves, test whether or not the game has ended, print the board to screen, and modify the board. Several members of the board class are used in the game class to implement alpha-beta search. 

The move class contains a list of jumps as well as all the data necessary to move a piece from one location to another on the board. The moves are created after all the jumps are created. The jumps are created by checking each direction for a valid piece and checking for valid jumps. Repetitions for jumps (jumping over the same square twice) are prevented by giving each jump a key and by maintaining predecessors for each jump. This key is a linear combination of its position on the board. By reversing the key (for example 123 -> 321) and maintaining pointers to previous jumps, the program can make sure a jump over a certain square is never repeated during one multiple jump.

The game class implements alpha-beta search. It also calls the members of the board class to print the board to the screen and to start up the game. It keeps track of the current state of the game by maintaining a pointer to the current board. With the smart pointer wrapper, which automatically de-allocates dynamic memory, alpha-beta search is able to generate many game boards and not have to manually call delete on each board created. The game class also maintains the time_t values to time a computer’s turn. The main function creates a game and makes the game start.
The evaluation function is calculated based on number and type of pieces left, how close a regular piece is to be a king, endgame positions, and a pseudorandom number. The comments for the evaluation function describe the breakdown of how it’s calculated.

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
