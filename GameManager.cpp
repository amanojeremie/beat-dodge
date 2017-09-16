#include "GameManager.h"
#include <iostream>
#include "GameState.h"

void GameManager::init() {
}

void GameManager::update(WindowManager& window, SoundManager& sound, float delta) {
	if(!gameStates.empty()) {
		if(delta >= .07f) {
			std::cout << "Frame Drop";
		}
		gameStates.top()->update(*this, window, sound, delta);
	}
}

void GameManager::render(WindowManager& window) {
	if(!gameStates.empty()) {
		gameStates.top()->render(window);
	}
}

void GameManager::pushState(GameState* gameState) {
	if(!gameStates.empty()) {
		gameStates.top()->pause();
	}
	gameStates.push(gameState);
	gameStates.top()->init();
}

void GameManager::popState() {
	if(!gameStates.empty()) {
		gameStates.top()->cleanup();
		delete gameStates.top();
		gameStates.pop();
	}
}

void GameManager::switchState(GameState* gameState) {
	popState();
	pushState(gameState);
}

