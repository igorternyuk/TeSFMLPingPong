#include "paddle.h"

#include <iostream>

Paddle::Paddle(float x, float y, sf::Color color, const std::string &imgPath):
    rect_{sf::Vector2f{FRAME_WIDTH, FRAME_HEIGHT}}, initPos_{x, y}
{
   rect_.setPosition(x, y);
   rect_.setFillColor(color);
   texture_.loadFromFile(imgPath);
   texture_.setSmooth(true);
   rect_.setTexture(&texture_);
   rect_.setTextureRect(sf::IntRect{0,0,FRAME_WIDTH, FRAME_HEIGHT});
}

void Paddle::reset()
{
    rect_.setPosition(initPos_);
}

void Paddle::move(sf::RenderWindow &playScreen, sf::Time delta, Direction dir)
{
    switch(dir)
    {
        case Direction::UP:
            if(rect_.getGlobalBounds().top > 0)
            {
                rect_.move(0.f, -delta.asSeconds() * VELOCITY);
            }
            break;
        case Direction::DOWN:
            if(rect_.getGlobalBounds().top + rect_.getGlobalBounds().height < playScreen.getSize().y)
            {
                rect_.move(0.f, +delta.asSeconds() * VELOCITY);
            }
            break;
        default:
            break;
    }
}

sf::FloatRect Paddle::boundingRect() const
{
    sf::FloatRect boundingRect;
    boundingRect.left = rect_.getGlobalBounds().left + PADDLE_SHADOW;
    boundingRect.top = rect_.getGlobalBounds().top + PADDLE_SHADOW;
    boundingRect.width = rect_.getGlobalBounds().width - 2 * PADDLE_SHADOW;
    boundingRect.height = rect_.getGlobalBounds().height - 2 * PADDLE_SHADOW;
    return boundingRect;
}

void Paddle::update(sf::Time delta)
{
    frameTime_ += delta;
    if(frameTime_.asSeconds() >= animDelay_)
    {
        if(++currentFrame_ >= NUM_FRAMES)
            currentFrame_ = 0;
        rect_.setTextureRect(sf::IntRect{currentFrame_ * FRAME_WIDTH,0,FRAME_WIDTH,
                                         FRAME_HEIGHT});
        frameTime_ = sf::Time::Zero;
    }
}

void Paddle::draw(sf::RenderWindow &playScreen) const
{
    playScreen.draw(rect_);
}
