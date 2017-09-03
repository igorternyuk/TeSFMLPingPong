#include "mainmenustate.h"
#include "playstate.h"
#include "game.h"
#include <iostream>

MainMenuState::MainMenuState()
{
    font_.loadFromFile("resources/fonts/BRLNSR.TTF");
    loadMenuItems();
}

MainMenuState::~MainMenuState()
{}

void MainMenuState::handleEvents(Game &game)
{
    sf::Event event;

    while(game.window().pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                game.stop();
                game.window().close();
                break;
            case sf::Event::KeyReleased:
                switch(event.key.code)
                    {
                        case sf::Keyboard::Q:
                        case sf::Keyboard::Escape:
                            game.stop();
                            game.window().close();
                            break;
                        case sf::Keyboard::Up:
                            --selectedMenuItem_;
                            if(selectedMenuItem_ < 0)
                                selectedMenuItem_ = menuItems_.size() - 1;
                            break;
                        case sf::Keyboard::Down:
                            ++selectedMenuItem_;
                            if(selectedMenuItem_ > int(menuItems_.size() - 1))
                                selectedMenuItem_ = 0;
                            break;
                        case sf::Keyboard::Return:
                            activateSelectedItem(game, selectedMenuItem_);
                            break;
                        default:
                            break;
                    }
                break;
            case sf::Event::MouseButtonReleased:
                if(event.mouseButton.button == sf::Mouse::Left)
                    activateSelectedItem(game, selectedMenuItem_);
                break;
            default:
                break;
        }
    }
}

void MainMenuState::update(Game&)
{
    for(unsigned short int i{0}; i < menuItems_.size(); ++i)
    {
        if(menuItems_.at(i).getGlobalBounds().
           contains(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))
        {
            selectedMenuItem_ = i;
            break;
        }
    }
}

void MainMenuState::draw(Game &game)
{
    game.window().clear(sf::Color{0, 134,194,255});
    game.window().draw(menuTitle_);
    for(unsigned short int i{0}; i < menuItems_.size(); ++i)
    {
        menuItems_.at(i).setColor(i == selectedMenuItem_ ? sf::Color::Yellow : sf::Color::White);
        game.window().draw(menuItems_.at(i));
    }
    game.window().display();
}

void MainMenuState::loadMenuItems()
{
    menuTitle_.setString(std::string("MENU"));
    menuTitle_.setColor(sf::Color::Red);
    menuTitle_.setFont(font_);
    menuTitle_.setCharacterSize(120);
    menuTitle_.setPosition((Game::WINDOW_WIDTH -
                            menuTitle_.getGlobalBounds().width) / 2, 50);
    for(unsigned short int i{0}; i < menuItemsParams_.size(); ++i)
    {
        sf::Text text(menuItemsParams_.at(i).first, font_, menuItemsParams_.at(i).second);
        float w{ text.getGlobalBounds().width };
        float top{i == 0 ?  (Game::WINDOW_HEIGHT - text.getGlobalBounds().height) / 2
                             - text.getGlobalBounds().height
                         :   menuItems_.at(i - 1).getGlobalBounds().top +
                             menuItems_.at(i - 1).getGlobalBounds().height + 5};
        text.setPosition((Game::WINDOW_WIDTH - w) / 2, top);
        text.setColor(i == 0 ? sf::Color::Red : sf::Color::White);
        menuItems_.push_back(text);
    }
}

void MainMenuState::activateSelectedItem(Game &game, int intemIndex)
{
    switch(intemIndex)
    {
        case 0:
            game.getPlayState()->prepareNewGame(PlayState::GameMode::HUMAN_HUMAN);
            game.switchScreen();
            break;
        case 1:
            game.getPlayState()->prepareNewGame(PlayState::GameMode::HUMAN_AI);
            game.switchScreen();
            break;
        case 2:
            game.stop();
            game.window().close();
        default:
            break;
    }
}
