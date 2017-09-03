#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "screenstate.h"
#include "paddle.h"
#include "consts.h"

#include <memory>

//class Paddle;
class Ball;

class PlayState: public ScreenState
{
public:
    enum class GameMode
    {
        HUMAN_HUMAN,
        HUMAN_AI
    };
    explicit PlayState();
    ~PlayState();

    // GameState interface

    virtual void handleEvents(Game &game);
    virtual void update(Game &game);
    virtual void draw(Game &game);

    void prepareNewGame(GameMode mode);
private:

    enum class GameState
    {
        PLAY,
        PAUSE,
        LEFT_PLAYER_WON,
        RIGHT_PLAYER_WON
    };
    enum {
        BALL_X = pong::WINDOW_WIDTH / 2,
        BALL_Y = pong::WINDOW_HEIGHT / 2,
        LEFT_PADDLE_X = 0,
        RIGHT_PADDLE_X = pong::WINDOW_WIDTH - Paddle::getWidth(),
        PADDLES_Y = (pong::WINDOW_HEIGHT - Paddle::getHeight()) / 2,
        WIN_SCORE = 5
    };
    GameMode gameMode_{GameMode::HUMAN_HUMAN};
    GameState gameState_{GameState::PLAY};
    int leftPlayerScore_{0};
    int rightPlayerScore_{0};

    std::unique_ptr<Ball> upBall_;
    std::unique_ptr<Paddle> upPaddleLeft_, upPaddleRight_;
    sf::Font font_;
    sf::Text txtScore_;
    sf::Texture tBackground_;
    sf::Sprite sBackground_;
    //Sounds
    sf::SoundBuffer sbCollision_;
    sf::Sound soundCollision_;
    sf::SoundBuffer sbGoal_;
    sf::Sound soundGoal_;
    void togglePause();
    void updateScore();
    void checkWin();
    void drawGameStatus(Game &game) const;
};
