#include "SFML/Graphics.hpp"
#include "mpg123.h"
#include "WindowManager.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "PlayState.h"
int main()
{

	WindowManager windowManager;
	SoundManager soundManager;
	GameManager gameManager;

	windowManager.init();
	soundManager.init();
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
