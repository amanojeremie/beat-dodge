#include "WindowManager.h"

static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;
static const int ANTI_ALIASING = 4;

void WindowManager::init() {
	gameWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
		"BeatDodge", sf::Style::Default, sf::ContextSettings(0, 0, ANTI_ALIASING));
	gameView.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void WindowManager::beginRender() {
	sf::Event event;
	while(gameWindow.pollEvent(event)) {
		if(event.type == sf::Event::Closed) {
			gameWindow.close();
		}
	}
	gameWindow.clear();
	gameView.clear();
}

void WindowManager::draw(const sf::Drawable& drawable) {
	gameView.draw(drawable);
}

void WindowManager::finalizeRender() {
	gameView.display();
	gameWindow.draw(sf::Sprite(gameView.getTexture()));
	gameWindow.display();
}

bool WindowManager::isOpen() const {
	return gameWindow.isOpen();
}

sf::Vector2i WindowManager::getMousePosition() const {
	return sf::Mouse::getPosition(gameWindow);
}


