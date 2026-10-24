//Developed by Shane Flynn
//Chess game with an AI using Alpha Beta pruning on a minimax algortihm
//Using SFML for the graphics
//Last edited 05/09/2021

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ChessBoard.h"
#include "StaticMemberInitialization.h"
#include "Player.h"
#include "MiniMax.h"
#include "BoardView.h"

void endScreen(sf::RenderWindow&, std::shared_ptr<ChessBoard>&, std::string);

int main(int argc, char* argv[]) {
	bool willThread = false,
		 willAI = false;
	if (argc <= 1) 	{ //default run
		willAI = true;
	}
	else 	{
		std::string useThread = argv[1],
			useAI = argv[2];
		if (useThread == "true" || useThread == "True") 	{ //use threading with AI. Testing purposes
			willThread = true;
		}
		if (useAI == "true" || useAI == "True") { //use AI. Testing purposes
			willAI = true;
		}
	}
	const int depthAI = 4; //set the depth the AI will search till
	float windowWidth = BOARD_PIXELS; //board size in drawing coordinates. Leave this alone and resize the window instead
	unsigned int startWindowSize = 960; //starting window width and height in pixels. The window can also be resized while playing
	sf::Color blkTileColor = sf::Color::Black;
	sf::Color whtTileColor = sf::Color((sf::Uint8)211, (sf::Uint8)211, (sf::Uint8)211); //change the color of the tiles easily
	
	const float frstSquareCentRef = windowWidth / 8 / 2;
	int mouseX, mouseY;
	sf::RenderWindow window(sf::VideoMode(startWindowSize, startWindowSize), "Chess AI Implementation");
	fitBoardToWindow(window);
	std::shared_ptr<ChessBoard> board = std::make_shared<ChessBoard>(windowWidth / 8, whtTileColor, blkTileColor, frstSquareCentRef);

	Player player(board);
	MiniMax allKnowingAI(board, depthAI, willThread);
	
	while (window.isOpen()) {

		sf::Event e;
		if (board->getIsCheckMate() == true) {
			endScreen(window, board, "Check Mate!");
		}
		 
		if (board->getIsStaleMate() == true) {
			endScreen(window, board, "Stale Mate!");
		}
		if (willAI) 	{
			if (board->getIsWhiteMove() == false) 	{	
				revCount = 0;
				uint16_t commitmentIsHard;
				{ //allow timing of the algo
					Timer time;
					commitmentIsHard = allKnowingAI.miniMax(false);
				}
				board->commitAIMove(commitmentIsHard);
				//std::cout << "Reverse bits went: " << revCount << " times.\n";
				//revCount = 0;
			}
		}
		while (window.pollEvent(e)) {
			handleResize(window, e);
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			else if (e.type == sf::Event::MouseButtonPressed) {
				if (e.mouseButton.button == sf::Mouse::Left) {
					//convert the click from window pixels to a board square so it still lines up after a resize
					if (pixelToTile(window, e.mouseButton.x, e.mouseButton.y, mouseX, mouseY)
						&& board->isPieceHere(mouseX, mouseY)) {
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

void endScreen(sf::RenderWindow& window, std::shared_ptr<ChessBoard>& board, std::string endTypeStr) {
	sf::Font font;
	sf::Text displayTxt;
	if (!font.loadFromFile("font/arial.ttf")) {
		std::cout << "Failed to load font from file!" << std::endl;
	}
	else {
		displayTxt.setFont(font);
		displayTxt.setString(endTypeStr);
		displayTxt.setFillColor(sf::Color::Green);
		displayTxt.setCharacterSize(120);
		displayTxt.setStyle(sf::Text::Bold);
		displayTxt.setOrigin(sf::Vector2f(310.f, 60.f));
		displayTxt.setPosition(sf::Vector2f(BOARD_PIXELS / 2.f, BOARD_PIXELS / 2.f)); //center of the board, whatever the window size
	}

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			handleResize(window, e);
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();
		board->drawBoard(window);
		window.draw(displayTxt);
		window.display();
	}
}
	//std::cout << (posX - 60)/120 << "   " << (posY-60)/120 << std::endl;