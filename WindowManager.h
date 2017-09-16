#ifndef WINDOWMANAGER_H_INCLUDED
#define WINDOWMANAGER_H_INCLUDED

#include "SFML/Graphics.hpp"

class WindowManager {
	private:
		sf::RenderTexture pass;
		std::vector<sf::Shader*> shaders;
		sf::RenderTexture gameView;
		sf::RenderWindow gameWindow;
	public:
		void init();
		void beginRender();
		void draw(const sf::Drawable& drawable);
		void finalizeRender();
		bool isOpen() const;
		sf::Vector2i getMousePosition() const;
};

#endif // WINDOWMANAGER_H_INCLUDED
