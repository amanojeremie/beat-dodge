#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED
#include <stack>

class GameState;
class SoundManager;
class WindowManager;

class GameManager {
private:
	std::stack<GameState*> gameStates;

public:
	void init();
	void update(WindowManager&, SoundManager& sound, float delta);
	void render(WindowManager& window);
	void cleanup();

	void switchState(GameState* gameState);
	void pushState(GameState* gameState);
	void popState();
};

#endif // GAMEMANAGER_H_INCLUDED
