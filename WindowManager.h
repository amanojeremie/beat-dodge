#ifndef WINDOWMANAGER_H_INCLUDED
#define WINDOWMANAGER_H_INCLUDED

#include "SFML/Graphics.hpp"

class WindowManager {
protected:
	sf::RenderTexture pass;
	std::vector<sf::Shader*> shaders;
	sf::RenderTexture gameView;
	sf::RenderWindow gameWindow;
public:
	void init();
	void beginRender();
	void draw(const sf::Drawable& drawanle);
	void finalizeRender();
	bool isOpen();
	sf::Vector2i getMousePosition();
};

#endif // WINDOWMANAGER_H_INCLUDED
