#include "WindowManager.h"

void WindowManager::init() {
	gameWindow.create(sf::VideoMode(1280, 720), "BeatDodge", sf::Style::Default, sf::ContextSettings(0, 0, 4));
	gameView.create(1280, 720);
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

bool WindowManager::isOpen() {
	return gameWindow.isOpen();
}

sf::Vector2i WindowManager::getMousePosition() {
	return sf::Mouse::getPosition(gameWindow);
}


