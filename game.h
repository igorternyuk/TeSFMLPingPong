#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <memory>

#include "mainmenustate.h"
#include "playstate.h"
#include "consts.h"

class Game
{
public:
    enum {
        WINDOW_WIDTH = pong::WINDOW_WIDTH,
        WINDOW_HEIGHT = pong::WINDOW_HEIGHT,
        WINDOW_X = (pong::SCREEN_RESOLUTION_X - WINDOW_WIDTH) / 2 - 40,
        WINDOW_Y = (pong::SCREEN_RESOLUTION_Y - WINDOW_HEIGHT) / 2
    };
    enum class ScreenStates
    {
        MAIN_MENU,
        PLAY
    };

    explicit Game();
    void run();
    inline void stop() { isRunning_ = false; }
    inline auto getElapsedTime() const noexcept { return timeElapsed_; }
    inline auto isRunning() const noexcept { return isRunning_; }
    inline sf::RenderWindow& window() { return window_; }
    void switchScreen();
    inline auto getPlayState() { return upPlayState_.get(); }
private:
    std::string WINDOW_TITLE{"TeSFMLPingPong"};
    bool isRunning_{true};
    sf::RenderWindow window_;
    sf::Time timeElapsed_{sf::Time::Zero};
    std::unique_ptr<PlayState> upPlayState_;
    std::unique_ptr<MainMenuState> upMainMenuState_;
    ScreenState* currentState_;
    //Music
    sf::Music bgmMenu_, bgmPlay_;
};
