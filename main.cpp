#include <iostream>
#include "SFML/Graphics.hpp"
#include "mpg123.h"
#include "WindowManager.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "PlayState.h"
int main(int argc, char* argv[])
{
	if(argc != 2) {
		std::cout << "Missing argument, need to specify song" << std::endl;
		std::cout << argv[0] << " xyz.mp3" << std::endl;
		exit(1);
	}

	WindowManager windowManager;
	SoundManager soundManager;
	GameManager gameManager;

	windowManager.init();
	soundManager.init(std::string(argv[1]));
	gameManager.init();
	gameManager.pushState(new PlayState{});

	sf::Clock frameClock;
	frameClock.restart();
	while(windowManager.isOpen()) {
		soundManager.update();
		gameManager.update(windowManager, soundManager, frameClock.restart().asSeconds());
		windowManager.beginRender();
		gameManager.render(windowManager);
		windowManager.finalizeRender();
		gameManager.render(windowManager);
	}
	gameManager.popState();

    return 0;
}
