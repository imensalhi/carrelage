#include "Game.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/system.hpp>
#include <SFML/window.hpp>
#include <SFML/network.hpp>
using namespace sf;
using namespace std;
//les fonctions privées
void Game::initvariables()
{
	this->window = nullptr;
    //game logic
    this->endGame = false;
    this->points = 0;
    this->health = 20;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 8;
    this->mouseheld = false;
    initMusic();
   

}
void Game::initText()
{
    this->uitext.setFont(this->font);
    this->uitext.setCharacterSize(30);
    this->uitext.setFillColor(sf::Color::White);
    this->uitext.setString("NONE");
}
void Game::initwindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 900;
    this->window = new RenderWindow(this->videoMode, "Carrelage", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}
void Game::initFonts()
{
    if (this->font.loadFromFile("font/RoughenCornerRegular.ttf"))
    {
        cout << "ERROR::GAME::INITFONTS::Failed to load font !" << "\n";
    }
}
void Game::initEnemies()
{
    this->enemy.setPosition(10.f,10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(Color::Green);
    //this->enemy.setOutlineColor(Color::Green);
    //this->enemy.setOutlineThickness(1.f);
}
void Game::updateMousePositions()
{
    this->mousePosWindow = Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::updateText()
{
    std::ostringstream ss;
    ss << "points: " << this->points <<"\n" <<"health: "<<this->health <<"\n";

    this->uitext.setString(ss.str());
}
//constructeur/deconstructeur
Game::Game()
{
	this->initvariables();
	this->initwindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}
Game::~Game()
{
	delete this->window;
    music.stop();
    
}
//accessoires

const bool Game::running() const
{
    return this->window->isOpen();
}
const bool Game::getEndGame() const
{
    return this->endGame;
}
//les fonctions
void Game::spawnEnemy()
{
    this->enemy.setPosition(
static_cast<float>(rand()% static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
1.f
    );
    //randomize enemy type
    int type = rand() % 7;
    switch (type)
    {
    case 0:
        this->enemy.setSize(sf::Vector2f(65.f, 65.f)); //sagher 7ajem enemies jdod walo 10
        this->enemy.setFillColor(sf::Color::Magenta);
        break;
    case 1:
        this->enemy.setSize(sf::Vector2f(70.f, 70.f)); //sagher 7ajem enemies jdod walo 30
        this->enemy.setFillColor(sf::Color::Blue);
        break;
    case 2:
        this->enemy.setSize(sf::Vector2f(90.f, 90.f)); 
        this->enemy.setFillColor(sf::Color::Red);
        break;
    case 3:
        this->enemy.setSize(sf::Vector2f(70.f, 70.f)); 
        this->enemy.setFillColor(sf::Color::Cyan);
        break;
    case 4:
        this->enemy.setSize(sf::Vector2f(60.f, 60.f)); 
        this->enemy.setFillColor(sf::Color::Green);
        break;
    case 5:
        this->enemy.setSize(sf::Vector2f(80.f, 80.f));
        this->enemy.setFillColor(sf::Color::Yellow);
        break;
    case 6:
        this->enemy.setSize(sf::Vector2f(80.f, 80.f));
        this->enemy.setFillColor(sf::Color::White);
        break;
    default:
        this->enemy.setSize(sf::Vector2f(100.f, 100.f)); 
        this->enemy.setFillColor(sf::Color::Yellow);
        break;
    }

    this->enemies.push_back(this->enemy);

}
void Game::pollEvents()
{
    // Gestion des événements
    while (this->window->pollEvent(this->ev))
    {
        if (this->ev.type == sf::Event::Closed)
        {
           this->window->close();
        }
    }
}
void Game::updateEnemies()
{//updating the timer for enemy
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 5.f;//mta3 nombre des enemy
    }
    //moving and updateing the enemies 
    for (int i = 0; i< this->enemies.size(); i++)
    {
        bool deleted = false;
        this->enemies[i].move(0.f, 15.f); //mta3 vitesse des enemies
        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
           if(this->enemies[i].getFillColor() != sf::Color::White)
            {
                this->health -= 2;
                std::cout << "health: " << this->health << "\n";
           }
           else 
               this->health = this->health;
           std::cout << "health: " << this->health << "\n";
            
        }

    }
    //chek if clicked upon
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseheld == false)
        {
            this->mouseheld = true;
            bool deleted = false;
            for (SIZE_T i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                   
                    //gain points
                    if (this->enemies[i].getFillColor() == sf::Color::Magenta)
                    this->points += 10;
                    else if (this->enemies[i].getFillColor() == sf::Color::Blue)
                    
                        this->points += 7;
                        
                    
                    else if(this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == sf::Color::Red)
                    {
                        //this->points += 3;
                        this->health += 2;
                    }
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 1;
                    else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
                        this->points += 4;
                    else if (this->enemies[i].getFillColor() == sf::Color::White)
                        this->points -= 2;

                    std::cout << "points: " << this->points << "\n";
                    //delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
              

            }
        }

    }
    else
    {
        this->mouseheld = false;
    }
}
void Game::update()
{
    this->pollEvents();
    if (this->endGame == false)
    {
        this->updateMousePositions();
        this->updateText();
        this->updateEnemies();
           
        
    }
    // fin condition de jeu
    if (this->health <= 0)//le jeueur est lose 
    {
        this->endGame = true;
       // this->window->clear();
       
    }
    if (this->points >= 300)//en cas ou le jeueur est win
        this->endGame = true;
}
void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uitext);
}
void Game::renderEnemies(sf::RenderTarget& target)
{
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}
void Game::render()
{
    // Effacement de la fenêtre avec une couleur de fond blanc

   //this->window->clear();
   if (this->endGame == false)
   {
       setBackground("images/background.png");
   }// Appeler la fonction pour charger l'image d'arrière-plan.
   //lose
   else 
   {
    if (this->health <= 0)
    {  //this->window->clear();
       music.stop();
       window->clear();
       setlose("images/lose.png");
       window->display();
       sf::sleep(sf::seconds(4));
       window->close();
       
       
    }
    else
    {
        music.stop();
       this->window->clear();
       setwin("images/win.png");
       window->display();
       sf::sleep(sf::seconds(4));
       window->close();
    }
   }
   //draw

       this->renderEnemies(*this->window);
       this->renderText(*this->window);
  
  

   // Mise à jour de la fenêtre
   this->window->display();
}

//backgound 
void Game::setBackground(const std::string& filename)
{
    sf::Texture backgroundTexture;
    if (backgroundTexture.loadFromFile(filename)) {
        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundTexture);
        window->draw(backgroundSprite);
    }
    else {
        std::cerr << "Failed to load background image: " << filename << std::endl;
    }
}
//music
void Game::initMusic()
{
    if (!music.openFromFile("audio/music.ogg"))
    {
        std::cerr << "Impossible de charger la musique !" << std::endl;      
        return;
    }
    music.setLoop(true); // boucle la musique en continu
    music.play(); // lance la musique
}
//lose
void Game::setlose(const std::string& filename)
{
    // Game Over Sprite -----------------------------------------------------------
    Texture gameoverTexture;
    gameoverTexture.loadFromFile(filename);
    Sprite gameoverSprite(gameoverTexture);
    //gameoverSprite.setPosition(50.0f, 150.0f);
    window->draw(gameoverSprite);
}
void Game::setwin(const std::string& filename)
{
    // win--------------------------------------------------------------------------
    Texture winTexture;
    winTexture.loadFromFile("images/win.png");
    Sprite winSprite(winTexture);
    //winSprite.setPosition(50.0f, 150.0f);
    window->draw(winSprite);

}
bool Game:: waitForStart() {

    sf::Texture texture;
    if (!texture.loadFromFile("images/start_button.png")) {
        // Erreur: impossible de charger l'image
        return false;
    }
    sf::Sprite sprite(texture);

    while (this->window->isOpen()) {
        sf::Event event;
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window->close();
                return false;
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Vérifier si le clic est sur l'image
                sf::Vector2i mousePos = sf::Mouse::getPosition(*this->window);
                sf::FloatRect bounds = sprite.getGlobalBounds();
                if (bounds.contains(mousePos.x, mousePos.y)) {
                    return true;
                }
            }
        }

        this->window->clear();
        this->window->draw(sprite);
        window->display();
    }

    return false;
}


