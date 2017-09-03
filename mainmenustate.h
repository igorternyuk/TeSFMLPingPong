#pragma once

#include <SFML/Graphics.hpp>
#include "screenstate.h"

#include <vector>
#include <string>

class MainMenuState: public ScreenState
{
public:
    explicit MainMenuState();
    ~MainMenuState();
    // GameState interface
public:
    virtual void handleEvents(Game &game);
    virtual void update(Game&);
    virtual void draw(Game &game);
private:
    int selectedMenuItem_{0};
    sf::Font font_;
    sf::Text menuTitle_;
    const std::vector<std::pair<std::string, unsigned int>> menuItemsParams_
    {
        {"Start human-human game", 60u},
        {"Start human-cpu", 60u},
        {"Quit game", 60u}
    };
    std::vector<sf::Text> menuItems_;
    void loadMenuItems();
    void activateSelectedItem(Game &game, int intemIndex);
};
