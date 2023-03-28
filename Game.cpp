#include "Game.h"

// Private Functions
void Game::initVariables() {
	this->m_window = nullptr;

	// Game Logic
	this->m_points = 0;
	this->m_enemySpawnTimerMax = 10.f;
	this->m_enemySpawnTimer = this->m_enemySpawnTimerMax;
	this->m_maxEnemies = 15;
	this->m_isMouseHeld = false;
	this->m_health = 10;
	this->m_isGameEnded = false;

}

void Game::initWindow() {

	/*
	* @return void
	* Creating a window instance and customizing it
		- Set the width and height 
		- Instantiate the window object
		- Restrict the framerate maunally so that the game doesn't run too fast 
	*/

	this->m_videoMode.height = 600;
	this->m_videoMode.width = 800;

	this->m_window = new sf::RenderWindow(this->m_videoMode, "My First Game", sf::Style::Titlebar | sf::Style::Close);
	this->m_window->setFramerateLimit(60);
}

void Game::initEnemies() {

	/*
	* @return void
	* Creating an enemy object
		- Set the position and size of the enemy
		- Set the color of the enemy
	*/

	this->m_enemy.setPosition(10.f, 10.f);
	this->m_enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->m_enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->m_enemy.setFillColor(sf::Color::Cyan);
	/*this->m_enemy.setOutlineColor(sf::Color::Green);
	this->m_enemy.setOutlineThickness(1.f);*/


}

void Game::initFont() {
	if (!this->m_font.loadFromFile("Fonts/Dosis-Light.ttf"))
		std::cout << "ERROR::GAME::initFont::Failed to load font" << std::endl;
}

void Game::initText() {
	this->m_uiText.setFont(this->m_font);
	this->m_uiText.setCharacterSize(24);
	this->m_uiText.setFillColor(sf::Color::White);
	// this->m_uiText.setPosition(0, 0);
	this->m_uiText.setString("NONE");
}

// Constructor
Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initFont();
	this->initText();
	this->initEnemies();
}

// Destructor
Game::~Game() {
	/*
	* Freeing up memory on the heap to prevent memory leaks
		- Delete anything that was created with the "new" keyword
	*/

	delete this->m_window;
}

// Accessors
const bool Game::running() const
{
	/*
	* @return bool
	* Helps the main class determine when to stop the game
		- Determine whether or not the window is still open
	*/
	return this->m_window->isOpen();
}

const bool Game::isGameEnded() const
{
	return this->m_isGameEnded;
}

void Game::pollEvents() {

	// Event Polling
	while (this->m_window->pollEvent(this->m_ev)) {
		switch (this->m_ev.type) {
		case sf::Event::Closed:
			this->m_window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->m_ev.key.code == sf::Keyboard::Escape)
				this->m_window->close();
			break;
		}
	}
}

void Game::updateMousePositions() {
	/*
	* @return void
	* Updates the mouse positions
		- Mouse position relative to the window (Vector2i)
	*/

	this->m_mousePosWindow = sf::Mouse::getPosition(*this->m_window);
	this->m_mousePosView = this->m_window->mapPixelToCoords(this->m_mousePosWindow);
}

void Game::spawnEnemy() {
	/*
	* @return void
	* Spawns enemies and sets their colors and positions
		- Set a random position
		- Sets a random color
		- Adds enemy to the vector
	*/

	this->m_enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->m_window->getSize().x - this->m_enemy.getSize().x)),
		0.f

	);

	this->m_enemy.setFillColor(sf::Color::Green);

	// Spawn the enemy
	this->m_enemies.push_back(this->m_enemy);

	// Remove the enemy at the end of the screen
	
}

void Game::updateText() {
	std::stringstream ss;

	ss << "Points: " << this->m_points << "\n"
		<< "Health: " << this->m_health << std::endl;
	this->m_uiText.setString(ss.str());
}

void Game::updateEnemies() {
	/*
	* @return void
	* Updates the enemy spawn timer and spawns enemies
		- When the total amount of enemies is smaller than the maximum
		- Moves the enemies downward
		- Removes the enemies at the edge of the screen
	*/

	// Updating the timer for enemy spawning
	if (this->m_enemies.size() < this->m_maxEnemies) {

		if (this->m_enemySpawnTimer >= this->m_enemySpawnTimerMax) {

			// Spawn the enemy and reset the timer 
			this->spawnEnemy();
			this->m_enemySpawnTimer = 0.f;
		}

		else
			this->m_enemySpawnTimer += 1.f;
	}

	// Moving and updating the enemies
	for (int i = 0; i < this->m_enemies.size(); i++) {

		bool deleted = false;
		this->m_enemies[i].move(0.f, 3.f); 

		// If the enemy is past the bottom of the screen
		if (this->m_enemies[i].getPosition().y > this->m_window->getSize().y) {

			this->m_enemies.erase(this->m_enemies.begin() + i);
			this->m_health -= 1;
			std::cout << "Health: " << this->m_health << std::endl;
		}
			

	}

	// Check if clicked upon

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (!this->m_isMouseHeld) {

			this->m_isMouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->m_enemies.size() && deleted == false; i++) {
				if (this->m_enemies[i].getGlobalBounds().contains(this->m_mousePosView)) {
					// Delete the enemy
					this->m_enemies.erase(this->m_enemies.begin() + i);
					deleted = true;

					// Gain points
					this->m_points += 10;
					std::cout << "Points: " << this->m_points << std::endl;
				}


			}
		}
		
	}
	else {
		this->m_isMouseHeld = false;
	}
		
}

// Functions
void Game::update() {

	this->pollEvents();

	if (!this->m_isGameEnded) {
		this->updateMousePositions();
		this->updateText();
		this->updateEnemies();
	}

	if (this->m_health <= 0)
		this->m_isGameEnded = true;
	

	// Update mouse position
	// Relative to the screen 
	/*std::cout << "mouse pos: " << sf::Mouse::getPosition().x << ", "
							   << sf::Mouse::getPosition().y << std::endl;*/

	// Relative to the window
	// std::cout << "Mouse Pos: " << sf::Mouse::getPosition(*this->m_window).x << ", " 
	//						   << sf::Mouse::getPosition(*this->m_window).y << std::endl;

	// Moving enemy with the mouse
	/*int posX = sf::Mouse::getPosition(*this->m_window).x;
	int posY = sf::Mouse::getPosition(*this->m_window).y;
	this->m_enemy.setPosition(posX, posY);*/

}

void Game::renderText(sf::RenderTarget& target) {
	target.draw(this->m_uiText);
}

void Game::renderEnemies(sf::RenderTarget& target) {
	for (auto& e : this->m_enemies) {
		target.draw(e);
	}
}

void Game::render(){
	/*
	* @return void
	* Renders the game objects
			- clear old frame
			- render objects
			- display frame in window
	*/

	this->m_window->clear();
	this->renderEnemies(*this->m_window);
	this->renderText(*this->m_window);
	this->m_window->display();
}
