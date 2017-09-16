#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

class SoundManager;
class GameManager;
class WindowManager;

class GameState {
	public:
		virtual void init() = 0;
		virtual void update(GameManager& game, WindowManager& window, SoundManager& sound, float delta) = 0;
		virtual void render(WindowManager& window) = 0;
		virtual void resume() = 0;
		virtual void pause() = 0;
		virtual void cleanup() = 0;
		virtual ~GameState() {};
};

#endif // GAMESTATE_H_INCLUDED
