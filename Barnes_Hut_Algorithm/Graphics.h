#pragma once

#include "SFML/Graphics.hpp"

class Graphics
{
private:
	sf::RenderWindow *window;

	bool isRunning();
	void update();
	void render();
	void renderEntities();
};

