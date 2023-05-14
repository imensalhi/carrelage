// SFMLMiniGameTuto.cpp : This file contains the 'main' function. Program execution begins andends there.
//
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/system.hpp>
#include <SFML/window.hpp>
#include <SFML/network.hpp>
#include <iostream>
using namespace sf;
int main()
{
    std::srand(static_cast<unsigned>(time(NULL)));
    Game game;
    game.waitForStart();
    // Boucle principale du programme
    if (game.waitForStart() == true)
    {
        while (game.running() && !game.getEndGame())
        {
            // Lancement de la musique d'arrière-plan
           // game.playBackgroundMusic();
            //update
            game.update();
            //render
            game.render();

        }
    }
    
    //game.setlose("images/lose.png");
    //************************fin de l'application***************************************
	return 0;
}