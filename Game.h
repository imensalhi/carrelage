#pragma once
#include <iostream>
#include<vector>
#include<ctime>
#include<sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/system.hpp>
#include <SFML/window.hpp>
#include <SFML/network.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
using namespace sf;
class Game
{
private:
	
	sf::Music music;
	//variables
	//pour win et lose 
	RenderWindow* window;
	Event ev;
	VideoMode videoMode;
	//mouse position 
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	//resources
	sf::Font font;
	//text
	sf::Text uitext;
	//game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseheld;
	//game objectifs
	std::vector<RectangleShape> enemies;
	RectangleShape enemy;
	//fonctions privées
	void initwindow();
	void initFonts();
	void initvariables();
	void initText();
	void initEnemies();
	void initMusic();
	
public:
	//constructeur /deconstructeur
	Game();
	virtual ~Game();
	//accessors 
	const bool running() const;
	const bool getEndGame() const;

	//les fonctions
	void spawnEnemy();
	void pollEvents();
	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
	void setBackground(const std::string& filename);
	//lose
	void setlose(const std::string& filename);
	//win
	void setwin(const std::string& filename);
	bool waitForStart();
};

