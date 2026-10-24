#ifndef BOARDVIEW_H
#define BOARDVIEW_H
#include <SFML/Graphics.hpp>

// The board is always drawn in a fixed 960x960 space. When the window is resized,
// SFML scales that drawing to fit, but mouse positions still arrive as raw window
// pixels. These helpers keep the drawing and the mouse in sync.

const float BOARD_PIXELS = 960.f; // size of the board in drawing coordinates
const float TILE_PIXELS = BOARD_PIXELS / 8.f;

// Fit the board into the window as a centered square. Extra space on the longer
// side is left as bars so the pieces don't get stretched.
inline void fitBoardToWindow(sf::RenderWindow& window) {
	float winW = (float)window.getSize().x;
	float winH = (float)window.getSize().y;
	if (winW <= 0.f || winH <= 0.f)
		return;

	sf::View view(sf::FloatRect(0.f, 0.f, BOARD_PIXELS, BOARD_PIXELS));
	if (winW > winH) {
		float widthRatio = winH / winW;
		view.setViewport(sf::FloatRect((1.f - widthRatio) / 2.f, 0.f, widthRatio, 1.f));
	}
	else {
		float heightRatio = winW / winH;
		view.setViewport(sf::FloatRect(0.f, (1.f - heightRatio) / 2.f, 1.f, heightRatio));
	}
	window.setView(view);
}

// Call for every polled event so a resize is handled no matter which loop is running.
inline void handleResize(sf::RenderWindow& window, const sf::Event& e) {
	if (e.type == sf::Event::Resized)
		fitBoardToWindow(window);
}

// Convert a window pixel (mouse position) into board drawing coordinates.
inline sf::Vector2f toBoardCoords(const sf::RenderWindow& window, int pixelX, int pixelY) {
	return window.mapPixelToCoords(sf::Vector2i(pixelX, pixelY));
}

// Convert a window pixel into a board column and row (0-7).
// Returns false if the pixel is outside the board (for example on the side bars).
inline bool pixelToTile(const sf::RenderWindow& window, int pixelX, int pixelY, int& col, int& row) {
	sf::Vector2f pos = toBoardCoords(window, pixelX, pixelY);
	if (pos.x < 0.f || pos.y < 0.f || pos.x >= BOARD_PIXELS || pos.y >= BOARD_PIXELS)
		return false;

	col = (int)(pos.x / TILE_PIXELS);
	row = (int)(pos.y / TILE_PIXELS);
	return true;
}

#endif
