#ifndef PLAYSTATE_H_INCLUDED
#define PLAYSTATE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "GameState.h"

class SoundManager;
class WindowManager;

#ifndef PI
#define PI 3.141592653589793238460
#endif

#ifndef DEGRAG
#define DEGRAG 180/PI
#endif

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
	Player player;
	std::vector<Beat> beats;
	void spawnBeat(int i);
	bool collideBeat(std::vector<Beat>::iterator beat);
	void cleanupBeat(std::vector<Beat>::iterator beat);
public:
	void init();
	void update(GameManager& game, WindowManager& window, SoundManager& sound, float delta);
	void pause();
	void resume();
	void render(WindowManager& window);
	void cleanup();
};

#endif // PLAYSTATE_H_INCLUDED
