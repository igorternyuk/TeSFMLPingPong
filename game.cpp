#include "game.h"

#include <iostream>

Game::Game():
    window_{sf::RenderWindow{sf::VideoMode{WINDOW_WIDTH, WINDOW_HEIGHT}, WINDOW_TITLE}},
    upPlayState_{std::make_unique<PlayState>()},
    upMainMenuState_{std::make_unique<MainMenuState>()},
    currentState_{upMainMenuState_.get()}
{
    window_.setPosition(sf::Vector2i{WINDOW_X, WINDOW_Y});
    bgmMenu_.openFromFile("resources/sounds/bgmMenu.ogg");
    bgmMenu_.setLoop(true);
    bgmPlay_.openFromFile("resources/sounds/bgmPlay.ogg");
    bgmPlay_.setLoop(true);
}

void Game::run()
{
    sf::Clock clock;
    bgmMenu_.play();
    while(window_.isOpen())
    {
        timeElapsed_ = clock.getElapsedTime();
        clock.restart();
        currentState_->handleEvents(*this);
        currentState_->update(*this);
        currentState_->draw(*this);
    }
}

void Game::switchScreen()
{
    if(currentState_ == upMainMenuState_.get())
    {
        currentState_ = upPlayState_.get();
        bgmMenu_.pause();
        bgmPlay_.play();
        //std::cout << "Screen switch: play screen was activated" << std::endl;
    }
    else if(currentState_ == upPlayState_.get())
    {
        currentState_ = upMainMenuState_.get();
        bgmPlay_.pause();
        bgmMenu_.play();
        //std::cout << "Screen switch: menu screen was activated" << std::endl;
    }
}
