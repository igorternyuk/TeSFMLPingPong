#include "playstate.h"

//#include "paddle.h"
#include "ball.h"
#include "game.h"

#include <cmath>
#include <algorithm>
#include <string>

PlayState::PlayState():
    upBall_{std::make_unique<Ball>(BALL_X, BALL_Y, "resources/gfx/ballSprite.png")},
    upPaddleLeft_{std::make_unique<Paddle>(LEFT_PADDLE_X, PADDLES_Y, sf::Color::Red,
                                           "resources/gfx/redPaddleSprite.png")},
    upPaddleRight_{std::make_unique<Paddle>(RIGHT_PADDLE_X, PADDLES_Y, sf::Color::Blue,
                                            "resources/gfx/bluePaddleSprite.png")}
{
    font_.loadFromFile("resources/fonts/orbitron-light.ttf");
    txtScore_.setString("0 : 0");
    txtScore_.setFont(font_);
    txtScore_.setCharacterSize(140);
    txtScore_.setPosition((Game::WINDOW_WIDTH - txtScore_.getGlobalBounds().width) / 2, 5);

    //Background

    tBackground_.loadFromFile("resources/gfx/background.png");
    tBackground_.setSmooth(true);
    sBackground_.setTexture(tBackground_);

    //Sounds

    sbCollision_.loadFromFile("resources/sounds/paddle_ball.wav");
    soundCollision_.setBuffer(sbCollision_);
    sbGoal_.loadFromFile("resources/sounds/goal.wav");
    soundGoal_.setBuffer(sbGoal_);
}

PlayState::~PlayState()
{}

void PlayState::handleEvents(Game &game)
{
    sf::Event event;

    while (game.window().pollEvent(event))
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
                        case sf::Keyboard::A:
                            prepareNewGame(GameMode::HUMAN_AI);
                            break;
                        case sf::Keyboard::N:
                            prepareNewGame(GameMode::HUMAN_HUMAN);
                            break;
                        case sf::Keyboard::Space:
                            prepareNewGame(gameMode_);
                            break;
                        case sf::Keyboard::M:
                            togglePause();
                            game.switchScreen();
                            break;
                        case sf::Keyboard::P:
                            togglePause();
                            break;
                        case sf::Keyboard::Q:
                        case sf::Keyboard::Escape:
                            game.stop();
                            game.window().close();
                            break;
                        default:
                            break;
                    }
                break;
            case sf::Event::MouseButtonReleased:
                switch(event.mouseButton.button)
                {
                    case sf::Mouse::Button::Left:
                        togglePause();
                        break;
                    case sf::Mouse::Button::Right:
                        togglePause();
                        game.switchScreen();
                        break;
                    case sf::Mouse::Button::Middle:
                        prepareNewGame(gameMode_);
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        upPaddleLeft_->move(game.window(), game.getElapsedTime(), Paddle::Direction::UP);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        upPaddleLeft_->move(game.window(), game.getElapsedTime(), Paddle::Direction::DOWN);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && gameMode_ == GameMode::HUMAN_HUMAN)
    {
        upPaddleRight_->move(game.window(), game.getElapsedTime(), Paddle::Direction::UP);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && gameMode_ == GameMode::HUMAN_HUMAN)
    {
        upPaddleRight_->move(game.window(), game.getElapsedTime(), Paddle::Direction::DOWN);
    }
}

void PlayState::update(Game &game)
{
    //Paddles animation
    upPaddleLeft_->update(game.getElapsedTime());
    upPaddleRight_->update(game.getElapsedTime());

    if(gameState_ == GameState::PLAY)
    {
         upBall_->update(game.getElapsedTime());
        //Let's check if our ball is beyond the side borders

        if(upBall_->left() < 0 || upBall_->right() > Game::WINDOW_WIDTH)
        {
            if(upBall_->left() < 0)
                ++rightPlayerScore_;
            else if(upBall_->right() > Game::WINDOW_WIDTH)
                ++leftPlayerScore_;
            upBall_->reset();
            updateScore();
            soundGoal_.play();
        }

        //May be the ball collides top or bottom borders
        //sf::FloatRect overlapRect;

        if(upBall_->top() <= 0)
        {
            float overlapX = 0.f;
            float overlapY = fabs(upBall_->top());
            upBall_->reflect(overlapX, overlapY, false);
        }
        else if(upBall_->bottom() >= Game::WINDOW_HEIGHT)
        {
            float overlapX = 0.f;
            float overlapY = upBall_->bottom() - Game::WINDOW_HEIGHT;
            upBall_->reflect(overlapX, overlapY, false);
        }

        //And here we check collisions with paddles

        sf::FloatRect overlapRect{0.f,0.f,0.f,0.f};

        if(upPaddleLeft_->boundingRect().intersects(upBall_->boundingRect(),overlapRect))
        {
            upBall_->reflect(overlapRect.width + 1, 0.f, true);
            upBall_->setColor(sf::Color::Red);
            soundCollision_.play();
        }
        else if(upPaddleRight_->boundingRect().intersects(upBall_->boundingRect(),overlapRect))
        {
            upBall_->reflect(overlapRect.width + 1, 0.f, true);
            upBall_->setColor(sf::Color::Blue);
            soundCollision_.play();
        }

        //If we play araings CPU we will move the right paddle correspondingly
        if(gameMode_ == GameMode::HUMAN_AI)
        {
            //y = y0 + vy / vx * (Game::WINDOW_WIDTH - x0)
            float collisionY = upBall_->y() + upBall_->vy() / upBall_->vx() *
                    (Game::WINDOW_WIDTH - upPaddleRight_->boundingRect().width -
                     upBall_->x());
            if(upBall_->x() > Game::WINDOW_WIDTH * 3 / 4 && upBall_->vx() > 0)
            {
                if(upPaddleRight_->boundingRect().top < collisionY)
                {
                    upPaddleRight_->move(game.window(), game.getElapsedTime(),
                                         Paddle::Direction::DOWN);
                }
                else if(upPaddleRight_->boundingRect().top > collisionY)
                {
                    upPaddleRight_->move(game.window(), game.getElapsedTime(),
                                         Paddle::Direction::UP);
                }
            }
        }
        checkWin();
    }
}

void PlayState::draw(Game &game)
{
    game.window().draw(sBackground_);
    game.window().draw(txtScore_);
    upBall_->draw(game.window());
    upPaddleLeft_->draw(game.window());
    upPaddleRight_->draw(game.window());

    drawGameStatus(game);
    game.window().display();
}

void PlayState::prepareNewGame(PlayState::GameMode mode)
{
    leftPlayerScore_ = 0;
    rightPlayerScore_ = 0;
    updateScore();
    upBall_->reset();
    upPaddleLeft_->reset();
    upPaddleRight_->reset();
    gameMode_ = mode;
    gameState_ = GameState::PLAY;
}

void PlayState::togglePause()
{
    if(gameState_ == GameState::PLAY)
        gameState_ = GameState::PAUSE;
    else if(gameState_ == GameState::PAUSE)
            gameState_ = GameState::PLAY;
}

void PlayState::updateScore()
{
    txtScore_.setString(std::to_string(leftPlayerScore_) +
                        std::string(" : ") +
                        std::to_string(rightPlayerScore_));
    txtScore_.setPosition((Game::WINDOW_WIDTH - txtScore_.getGlobalBounds().width) / 2,
                          txtScore_.getPosition().y);
}

void PlayState::checkWin()
{
    if(leftPlayerScore_ >= WIN_SCORE)
        gameState_ = GameState::LEFT_PLAYER_WON;
    else if(rightPlayerScore_ >= WIN_SCORE)
        gameState_ = GameState::RIGHT_PLAYER_WON;
}

void PlayState::drawGameStatus(Game &game) const
{
    std::string msg;
    sf::Color color;
    switch(gameState_)
    {
        case GameState::PAUSE:
            msg = "GAME PAUSED";
            color = sf::Color::Yellow;
            break;
        case GameState::LEFT_PLAYER_WON:
            msg = "LEFT PLAYER WON!";
            color = sf::Color::Red;
            break;
        case GameState::RIGHT_PLAYER_WON:
            msg = "RIGHT PLAYER WON!";
            color = sf::Color::Blue;
            break;
        default:
            msg = "";
            color = sf::Color::White;
            break;
    }
    sf::Text text(msg, font_, 100);
    text.setColor(color);
    text.setPosition((Game::WINDOW_WIDTH - text.getGlobalBounds().width) / 2,
                     (Game::WINDOW_HEIGHT - text.getGlobalBounds().height) / 2);
    game.window().draw(text);
}
