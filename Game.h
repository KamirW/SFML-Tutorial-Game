#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	This class acts as the game engine.
	Wrapper class
*/
class Game {
private:

	// Variables
	// Window
	sf::RenderWindow* m_window;
	sf::VideoMode m_videoMode;
	sf::Event m_ev;

	// Mouse Positions
	sf::Vector2i m_mousePosWindow;
	sf::Vector2f m_mousePosView; 

	// Resources
	sf::Font m_font;

	// Text
	sf::Text m_uiText;

	// Game Logic
	unsigned m_points;
	float m_enemySpawnTimer;
	float m_enemySpawnTimerMax;
	int m_maxEnemies;
	bool m_isMouseHeld;
	int m_health;
	bool m_isGameEnded;



	// Game Objects
	std::vector<sf::RectangleShape> m_enemies;
	sf::RectangleShape m_enemy;

	// Private functions
	void initVariables();
	void initWindow();
	void initEnemies();
	void initFont();
	void initText();

public:
	Game(); // Constructor
	virtual ~Game(); // Destructor

	// Accessors
	const bool running() const;
	const bool isGameEnded() const;

	// Functions
	void pollEvents();
	void updateMousePositions();
	void spawnEnemy();

	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};


