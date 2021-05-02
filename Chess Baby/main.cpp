//Developed by Shane Flynn
//Chess game with an AI using Alpha Beta pruning on a minimax algortihm
//Using SFML for the graphics
//Last edited 05/02/2021

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ChessBoard.h"
#include "StaticMemberInitialization.h"
#include "Player.h"
#include "MiniMax.h"

int main(int argc, char* argv[]) {
	bool willThread = false,
		 willAI = false;
	std::string useThread = argv[1],
		useAI = argv[2];
	if (useThread == "true" || useThread == "True") 	{ //use threading with AI. Testing purposes
		willThread = true;
	}
	if (useAI == "true" || useAI == "True") { //use AI. Testing purposes
		willAI = true;
	}
	const int depthAI = 4; //set the depth the AI will search till
	float windowWidth = 960; //x and y of window
	sf::Color blkTileColor = sf::Color::Black;
	sf::Color whtTileColor = sf::Color(211.f,211.f,211.f); //change the color of the tiles easily
	
	const float frstSquareCentRef = windowWidth / 8 / 2;
	int mouseX, mouseY;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowWidth), "Chess AI Implementation");
	std::shared_ptr<ChessBoard> board = std::make_shared<ChessBoard>(windowWidth / 8, whtTileColor, blkTileColor, frstSquareCentRef, depthAI);

	Player player(board);
	MiniMax allKnowingAI(board, depthAI, willThread);
	
	while (window.isOpen()) {

		sf::Event e;
		if (board->getIsCheckMate() == true) {
			std::cout << "Checkmate FTW!\n";
			std::cout << "Hit Enter to end program\n";
			std::cin.get();
			window.close();
		}
		 
		if (board->getIsStaleMate() == true) {
			std::cout << "The Stale Has Been Mated\n";
			std::cout << "Hit Enter to end program\n";
			std::cin.get();
			window.close();
		}
		if (willAI) 	{
			if (board->getIsWhiteMove() == false) 	{	
				uint16_t commitmentIsHard;
				{ //allow timing of the algo
					Timer time;
					commitmentIsHard = allKnowingAI.miniMax(false);
				}
				board->commitAIMove(commitmentIsHard);
			}
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
	//std::cout << (posX - 60)/120 << "   " << (posY-60)/120 << std::endl;