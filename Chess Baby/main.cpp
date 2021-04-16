//Developed by Shane Flynn
//Chess game with an AI using Alpha Beta pruning on a minimax algortihm
//Last edited 04/15/2021

/* Things to Ponder

How will you know which piece is grabbed when on click? //Could make a find who is clicked function. Just loop through the pieces on which ever color's turn it is

Can the pieces be in an array and be used effectively? 

When a piece is captured, deallocate it from the array.

Where to make the piece movement rules?

For movement, check each square in the path and see if there is a piece in the way. Except for knights

May need a value in ChessBoard to keep track if a piece exists on a space or not Could save index of a certain piece and no piece has a null type as a value.
	Positive for white and negative for black. Pawns start at 10 and take a modular of them.
	Could also just make a gameState 2D array inside of main to keep track of it

Create a Tile class that holds if a piece is in a spot on chessBoard. Also hold The piece value it has. Can hold if black is there or white

Have a 1D array for the pieces on the board. The index of the piece would just be the row*col. 
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ChessBoard.h"
#include "StaticMemberInitialization.h"
#include "Player.h"
#include "MiniMax.h"

int main() {
	float windowWidth = 960; //x and y of window
	sf::Color whtTileColor = sf::Color(211.f,211.f,211.f); //change the color of the tiles easily
	sf::Color blkTileColor = sf::Color::Black;
	const int depthAI = 3; //set the depth the AI will search till
	const float frstSquareCentRef = windowWidth / 8 / 2;
	int mouseX, mouseY;
	
	long long cake = 0L;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowWidth), "Chess AI Implementation");
	
	//std::cout << (posX - 60)/120 << "   " << (posY-60)/120 << std::endl;
	std::shared_ptr<ChessBoard> board = std::make_shared<ChessBoard>(windowWidth / 8, whtTileColor, blkTileColor, frstSquareCentRef, depthAI);
	//ChessBoard *board = new ChessBoard(windowWidth / 8, whtTileColor, blkTileColor, frstSquareCentRef, depthAI);
	//board->notCapturable = ~(board->whPieces | board->blKing);
	Player player(board);
	MiniMax allKnowingAI(board, depthAI);

	while (window.isOpen()) {

		sf::Event e;
		if (board->getIsCheckMate() == true)
			std::cout << "Checkmate Bitch!\n";
		 
		if (board->getIsStaleMate() == true)
			std::cout << "The Stale Has Been Mated\n";
		
		if (board->getIsWhiteMove() == false) 	{
			uint16_t commitmentIsHard;
			{
				Timer time;
				commitmentIsHard = std::get<1>(allKnowingAI.miniMaxAlgo(0, false));
			}
			board->commitAIMove(commitmentIsHard);
		}
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			else if (e.type == sf::Event::MouseButtonPressed) {
				if (e.mouseButton.button == sf::Mouse::Left) {
					mouseX = e.mouseButton.x / 120;
					mouseY = e.mouseButton.y / 120;
					if (board->isPieceHere(mouseX, mouseY)) {
						player.movePiece(mouseX, mouseY, window);
					}
				}
			}
		}

		window.clear();
		board->drawBoard(window);
		window.display();
	}
	

	return 0;
}
/*
void initializePiecePosition(const float& frstSquareCentRef) {

	for (int i = 0; i < 8; i++) { //set empty tiles to NULL
		for (int j = 2; j < 7; j++) {
			pieceBoard[i][j] = NULL;
		}
	}

	const float pieceScale = (1 - (tileWidth / (tileWidth * 8))) * 0.40;
	float whiteYOffset = frstSquareCentRef + (tileWidth * 7);

	const int kingVal = 10000,
		queenVal = 900,
		rookVal = 500,
		knightVal = 300,
		bishopVal = 300,
		pawnVal = 100;
	//                            Value, Image Scale,       img path,                x cord,         y cord    whitePiece
	pieceBoard[0][7] = new Rook(rookVal, pieceScale, "assets/white-rook.png", frstSquareCentRef, whiteYOffset, true);
	pieceBoard[1][7] = new Knight(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef + (tileWidth * 1), whiteYOffset, true);
	pieceBoard[2][7] = new Bishop(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef + (tileWidth * 2), whiteYOffset, true);
	pieceBoard[3][7] = new Queen(queenVal, pieceScale, "assets/white-queen.png", frstSquareCentRef + (tileWidth * 3), whiteYOffset, true);
	pieceBoard[4][7] = new King(kingVal, pieceScale, "assets/white-king.png", frstSquareCentRef + (tileWidth * 4), whiteYOffset, true);
	pieceBoard[5][7] = new Bishop(bishopVal, pieceScale, "assets/white-bishop.png", frstSquareCentRef + (tileWidth * 5), whiteYOffset, true);
	pieceBoard[6][7] = new Knight(knightVal, pieceScale, "assets/white-knight.png", frstSquareCentRef + (tileWidth * 6), whiteYOffset, true);
	pieceBoard[7][7] = new Rook(rookVal, pieceScale, "assets/white-rook.png", whiteYOffset, whiteYOffset, true);


	//Black pieces
	pieceBoard[0][0] = new Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef, frstSquareCentRef, false);
	pieceBoard[1][0] = new Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef + (tileWidth * 1), frstSquareCentRef, false);
	pieceBoard[2][0] = new Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef + (tileWidth * 2), frstSquareCentRef, false);
	pieceBoard[3][0] = new Queen(queenVal * -1, pieceScale, "assets/black-queen.png", frstSquareCentRef + (tileWidth * 3), frstSquareCentRef, false);
	pieceBoard[4][0] = new King(kingVal * -1, pieceScale, "assets/black-king.png", frstSquareCentRef + (tileWidth * 4), frstSquareCentRef, false);
	pieceBoard[5][0] = new Bishop(bishopVal * -1, pieceScale, "assets/black-bishop.png", frstSquareCentRef + (tileWidth * 5), frstSquareCentRef, false);
	pieceBoard[6][0] = new Knight(knightVal * -1, pieceScale, "assets/black-knight.png", frstSquareCentRef + (tileWidth * 6), frstSquareCentRef, false);
	pieceBoard[7][0] = new Rook(rookVal * -1, pieceScale, "assets/black-rook.png", frstSquareCentRef + (tileWidth * 7), frstSquareCentRef, false);

	for (int i = 0; i < 8; i++) {
		pieceBoard[i][6] = new Pawn(pawnVal, pieceScale, "assets/white-pawn.png", frstSquareCentRef + (tileWidth * i), frstSquareCentRef + (tileWidth * 6), true); //white pawns
		pieceBoard[i][1] = new Pawn(pawnVal, pieceScale, "assets/black-pawn.png", frstSquareCentRef + (tileWidth * i), frstSquareCentRef + (tileWidth), false); //black pawns
	}

}
*/

/*	UPDATES IN PROCESS

02/25/2021:
	Board was made and pieces put on it. None can move. Changed method of creating and storing pieces from
	being separate from the tiles to being a part of the tiles.

	Update: Trashed the BoardSquare being its own class idea. Just have the pieces on the board be their own 2D array

02/26/2021:
	Current method of storing the pieces on the board will not work. Cannot connect the different pieces easily
	Trying a 1D array of Pieces inside of main

	Update: Got the method that was not working to start working. However, there are doubts about how well it will continue to perform.
	Though changing up the format down the like when the AI starts to be integrated will be easier if there is already some kind of framework in place.
	The software can be updated to fit the needs of the program when it is needed at the time.

	Update: Changing the validMoves function from going inside of the pieces to being part of the ChessBoard class and try to do things in there and not in the
	individual piece. Having issues trying to get a king to castle properly and not update other stuff


03/04/2021:
	Added Masks to be used for moving sliding pieces such as rooks, queens, and bishops. Useful for the Hyperbola Quintessence. Fancy math tricks with bitboard stuff
	
	In the process of adding piece moves. Current idea is to return a vector of possible moves for the future AI.
	Considering having slightly separate code for the player when they play a piece
	
	Graphical side is on the backburner until the moves are set and working.
	
	Had an issue with the masks where incorrect values were in some spots. Definitely not from being up till 2am working on this.

03/05/2021:
	Working on changing the return data type for the moves. Using uint16_t to store before move and after move location index of a piece.
	Currently using a vector to store this information. However, a string that can be decoded might serve better. Read in segments of 4 for the informaiton needed.
		This method will take a max of 4 bytes per move set instead of the current 2, but speed wise might be faster by avoiding vectors.

	Added the moves logic for Bishop, Queens, Rooks, King, and Knights. Had to debug all of them based on some oversighted code that led to an infinite loop.

03/06/2021
	Tested the efficiency of dynamically created possible moves when compared to standard copying. Tests were done before any AI was implemented through 1 million loops of the starting position moves.
	Also missing a few minor elements to the game that might add a few milliseconds, but shouldn't be anything of significance. A future test of the third one may be done after the AI is set up.
	Any possible enPassant moves and castling were not part of this testing session.
	First three tests were a small approximate average of a few runs. The last was tested 10 times and averaged out.

	Standard Copying:
		4 million positions took 16.8 seconds
		2 million positions took 8.9 seconds

	Dynamic Creation:
		4 million positions took 13.175 seconds
		2 million positions took 6.832 seconds

	Dynamic Creation but Not Copied Into One Vector:
		4 million positions took 9.869 seconds
		2 million positions took 5.252 seconds

	Results show it might be better to avoid consolidating all the moves into one large container. Keeping them separate may be best to reduce the time it takes

	Dynamic Creation but Not Copied Into One Vector plus Trailing Zeros Optimization:
		4 million positions took 9.250 seconds
		2 million posiitons took 4.852 seconds

	All attemps at optimizing the reversedBits() function resulted in longer times. Could be something to look further into again later since it is used several times for one move generation call.

03/08/2021
	Started to implement the unsafe squares for each piece. As of now, the project is moderately lacking in the OOP concept for ease of idea flow having it all consolidated together.
	So far it has been easy to consolidate most of the code to work for either color. Moves such as pawn moves, enPassant, and castling have been separate.
	Now with implementing the unsafe squares, the idea is to store where each piece attacks for each color. This can be updated after a move has been chosen. This should be a faster method than
	having to search for every possible attacked square in a given instance. This information may have to be stored separate from the game board as each branch would have different attacked squares.
	Though that may not be necessary as each branch is done one at a time, unless multiprocessing is used somewhere. Though that will be determined later.
	Updating the attacked squares can be done inside move generations for each piece.
	Keep separate ones in case of a double check. This could be checked for first before checking if a check can be blocked.
	Store the squares when the empty for castling. If any piece attacks a square, it turnes off and thus cannot castle. Have bools for if king moved, queen side rook, or king side rook.

03/09/2021
	Accepted my realization that perhaps I should change the set up to be a 64 bit process instead of 32 bit. It wasn't clear right away that I was limiting myself on speed because I didn't realize it was in 32 bit.
	After changing, a few test runs were done for time. Also nearly lost a section of code from a mistype that erased the body of a whole funtion. Luckily had a recent backup of the code.

	Dynamic Creation but Not Copied Into One Vector plus Trailing Zeros Optimization using 64bit compiler:
		4 million positions took 7.674 seconds
		2 million positions took 3.613 seconds

	The results are welcomed, but it is still a long way to go before the AI is set up fully. EnPassant and Castling are the two that need to be put in. The last thing is checks and three fold repition.

03/18/2021
	The last moves to put in was castling, but it came to a point where it seemed I should implement the class structure in a more proper OOP format. Was just having all moves in the ChessBoard, but
	this is causing a conflict in trying to keep track of the attacked squares. There is a work around that is straightforward, but it would make it harder to change it all into OOP format. So, OOP will
	take priority.
	Goal is to finish the AI in the barest form by the end of the month.
	Stretch goal is to have Alpha Beta pruning implemented before the end of the month.

03/19/2021
	Restructured the program to be more OOP centric.
	Added unique_ptr to avoid potential memory leakes.

03/22/2021
	Started to implement the simple task of moving the pieces and displaying them properly.
	Not as simple as expected. Running into many different read access violations.
	However, this does show where things are not linked the way I was initially intending and will hopefully result in a cohesive game by the time the AI can be implemented

03/24/2021
	Pieces are displayed using bitboard information.
	Need to update piece specific bitboard info to update display on the board.
	Need to look into layering of images on board since a grabbed piece might be on a lower layer.
	Still no functioning movement of pieces, but progress is being made.

	Found the display bug. I had some test code that was updating the position of the piece inside of the board info, but neglected to update it inside the piece itself where
		the pieces are actually read and displayed.
	The layering issue was fixed by alternating the sequence that something is drawn to the board.
	Pieces can now be moved.
	The bug also showed a potential spot for improved speed in updating a piece's position on the board by having a pointer saved to boardInfo that can be updated.

	Way to update the attacked squares is to run through every piece and find the squares it attacks each for every move. Though don't have any vectors to keep it as simple as possible.
		The less memory allocation the better.

	Need to add logic to update the attacked squares when castling. However, that can be found after the movement is made and then calculated which I kinda what is happening already.
	Still need to put in the castling logic in general too.

	Have to update both white and black squares that they attack after each fold. A moved piece of one color can expose the sight of enemy piece.
	Though this may not be needed since the opponents attacked squares would be updated after their turn.

03/25/2021
	Trying to add highlighting of squares to better visualize attacked squares.
	Need to implement it for every piece other than pawns.

	For piece values in MiniMax, can keep the number of pieces that a player has and just multiply with value.
	For adding location weights, keep an array for each piece that holds weights corresponding to the square.
		Keeping the location index of each piece in a vector will allow quick indexing of any potential weights to influence the algorithm

	Taking the time to add highlighting of moveable squares helped find a bug in the enPassant feature. Shocker.
	Need to update the locations where a piece can enPassant only after the move has been chosen.
	May use the second to last bit for this special move

	For pawn promotion, can just use one of the empty bits between the two numbers to indicate when a pawn promotes

03/26/2021
	Added highlighting of moveable squares for every piece when a player grabbs it.
	Added ability to check for special moves:
		-EnPassant
		-Castling
		-Promotion
		-Two square pawn push
	Added player ability to promote a pawn, currently only allows for queen.
	Added player ability to castle, currently king disappears.
	Added player ability to enPassant, seems to work properly now.
	Added images appearing for pawn promotion

	Although the highlighting of squares shows moveable spots, it does not show the exact squares that are being found by the move generator of a piece.
		This is a result of finding the highlighted squares in a separate function which could cause a separation in results.
		However, things should be the same as the same process of finding a move is used for each one.

	This process could be used by holding where each piece is held in an array and then using the highlighted squares in the set up to find the moveable spot.

03/27/2021
	Adding the castling feature.
	This has resulted in the discovery of many bugs which are being ironed out as they are found.
	The highlighting of squares was used to find a tricky bug where if a knight captured a rook, then the king could castle with that knight still.
		A video gave me warning about that bug since they also had the same issue.

03/29/2021
	Successfully implemented castling
	Last major thing to implement is the concept of checks and checkmate.
	Also need to add three-fold repetition
	AI can hopefully be started this week or next.

	For checks, can possibly combine the attackSquares for a piece with the findPathToCheck. The change to attackSquares would be the inclusion of the
		king as a spot to attack. This info will play into the spots a king can move. Should fix the castling while in check issue.

	Need to split the finding of horiz, vert, diag, and antiDag moves based on where the piece is located. This way the path of attack to king is correct.
	Need to include the piece that is attacking to the squares since it is currently not included

	To get half of a bitboard including the initial spot to be 1s, multiply by -1
		Can use this to find which half of the attacking squares go towards king.
		Would still need to | the piece since the result of diag, horz, etc. moves does not include the piece.

04/09/2021
	Fixed display issue where the piece could not be captured if it was checking a king that was on the bottom half of the piece attack direction relative to white's view
	King cannot move to spots where pieces are attacking now after fixing the updating of where the enemy pieces attack

	Idea for implementing a pinned piece:
		King sends out a beacon in all directions.
		The first piece of its own color that it hits, that direction stops there. Same logic for finding a piece to capture, but reversed.
		When a piece is about to find its legal squares, check if it was hit by the king
		Somehow check path of line of attack that includes piece and the king

04/10/2021
	Continuing the ideas
		King sends out beacon signal.
		First piece it hits stops the beacon.
		A piece is pinned if the path of attack for a sliding piece connects with the other color piece and king.
			-Can be found by adding king onto squares the enemy piece attacks.
			-Direction is found by & operator the attacked squares with the pair of piece and king combo
		Can find a pinned piece if it is on a board for color specific pinned pieces.

	Added ability for pieces to see if it is pinning an enemy piece.
		Each direction was checked one at a time to prevent more than one piece being thought of as being pinned when it was not.
		This was done inside the function for finding attacked squares for specific pieces to avoid some redundant computation.
			However, this may have caused more computation to find the path a piece can move, but is impossible to say for certain without having an alternative idea.
		Implemented for:
			-Queen
			-Rook
			-Bishop
	Added ability for a piece to find its moveable squares if it is pinned
		-A knight cannot move if pinned
		-Queen, rook, bishop, and pawn use a series of if statements to find which direction the friendly king is in and use that to find moveable squares

	Bug
		-King can capture a piece that is defended by another.
		-Pawn can enPassant a piece which will cause their own king to be in check which is illegal

04/11/2021
	Fixed bug
		-King capturing a defended piece
		-Queen not being able to place back on last spot of file 4
		-King being able to move to an attacked square
	Bug
		-King can move to an attacked square in same path as in check

	When aligning the legal moves with the display moves, make a function for pinned pieces that returns the possible spots. Can work for display and legal move purposes.

	Fixed bug
		-EnPassent into check

	Add a board of enemy pieces that are defending another. Can be found by negating notCapturable

04/12/2021
	Continuing to update the legal moves for pawns.
	The king now follows legal moves

04/13/2021
	Added ability to check for checkmate
	Added ability to check for stalemate
	Added SavedGameState to save the game state for the minimax algorithm
	Fixed king movement
	Fixed pawn movement
	Fixed bishop pin movement
	Fixed rook pin movement

	Game should be ready to implement the MiniMax algorithm

04/14/2021
	Adding the make move and undo move for AI.
	Added a stack class to facilitate the addition or removal of pieces from the board from AI moves.
	Fixed pawn movement "feature" that was due to a lack of implementing the legal features for the pawn when finding attacking squares that hit the king.
	Attempting to add counter for total number of pieces on the board for each type and color.
		Requiring a bit of redesign for the player class and method of removing a captured piece to make it work.
			Hoping the end result will be worth it.

	Fixed pawn attacking squares, again.
	Fixed pawn attacking squares, once again. Missed a plus sign that should have been a minus sign.

	Added move logic for AI making a white move.
	Added logic to handle the making and eventual unmaking of any type of move.
	Modified Player.h to fit the new set up

	Ready to start the undo move process for the AI.
		Following step will be the implementation of move generation in the algorithm.

04/15/2021
	Fully added undo logic for black and white moves.
	Added element inside of stack class to take care of saving an enPassant bit board

*/

/* Stuff from the Chess Wiki
	Add a reversable move counter. Any move that is not an irreversable move is added to the count and then reset when an irreversable move is made.
		Irreversable moves are captures, pawn pushes, and pawn promotions.







*/

/* MINIMUM VIABLE PRODUCT
	A chess AI that implements minimax to play chess using all of the standard chess rules except the fifty-move rule.

*/

/* OPTIMIZATION IDEAS
	Can eliminate the trailing zeros function by holding an idex for each piece location. Pawns would benefit more from bitboards potentially
*/

/* STRETCH GOALS

	1. Alpha-beta pruning
	2. Able to compute four levels deep in under 1.5 seconds
	3. Able to compute six levels deep in under 3 seconds.
	4. Transposition table
	5. Squares have weights for each piece. Improve AI piece development and placement.
	6. Able to calculate the assumed number of possible moves in a given position based on generally accepted numbers. Shows the AI knows all the legal moves of chess.
	7. AI accounts for quiescent positions by performing a quiescence search.
	8. Add book openings that the AI will randomly chose to start with.
	9. AI can access GM games to follow for opening principles and set up.
	10. Zobrist hashing.

	11. AI can beat me at a game
	12. AI can beat 1600 elo rated computers
	13. AI can beat 2000 elo rated computers
	14. AI can trash talk when it knows it is winning
	15. Add difficulty levels for the AI. Different levels allow certain depths that the AI can look into.

	16. Multiprocessing
*/

/* SOURCES
	Hyperbola Quintessence: https://www.chessprogramming.org/Hyperbola_Quintessence



*/