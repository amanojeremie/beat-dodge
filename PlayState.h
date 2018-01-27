#ifndef PLAYSTATE_H_INCLUDED
#define PLAYSTATE_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "GameState.h"

class SoundManager;
class WindowManager;

struct Beat {
	sf::Vector2f position;
	sf::Vector2f velocity;
	int band;
	sf::CircleShape shape;
};

struct Player {
	sf::Vector2f position;
	sf::CircleShape shape;
};

class PlayState : public GameState {
private:
	unsigned int stackSize;
	Player player;
	std::vector<Beat> beats;
	void spawnBeat(int i);
	bool collideBeat(const Beat& beat) const;
	bool cleanupBeat(const Beat& beat) const;
public:
	void init();
	void update(GameManager& game, WindowManager& window, SoundManager& sound, float delta);
	void pause();
	void resume();
	void render(WindowManager& window);
	void cleanup();
};

#endif // PLAYSTATE_H_INCLUDED
