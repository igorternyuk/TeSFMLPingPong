#include "ball.h"

#include "game.h"

#include <cmath>
#include <ctime>
#include <iostream>

Ball::Ball(float x, float y, const std::string &imgPath):
    circle_{RADIUS}, initPos_{x, y}
{
    circle_.setOrigin(RADIUS, RADIUS);
    circle_.setPosition(x, y);
    texture_.loadFromFile(imgPath);
    texture_.setSmooth(true);
    circle_.setTexture(&texture_);
    srand(time(0));
    init();
}

void Ball::reflect(float overlapX, float overlapY,
                   bool isVerticalSurface)
{
    /*std::cout << "До компенсации "<< std::endl;
    std::cout << "bx = " << x() << " by = " << y() << std::endl;
    std::cout << "overlapX = " << overlapX << " overlapY = " << overlapY << std::endl;*/
    if(isVerticalSurface)
    {

        circle_.move((vx() < 0 ? 1 : -1) * (overlapX + 2), 0.f);
        velocity_.x = -velocity_.x;
    }
    else
    {
        circle_.move(0.f, (vy() < 0 ? 1 : -1)  * (overlapY + 2));
        velocity_.y = -velocity_.y;
    }
    //std::cout << "После компенсации" << std::endl;
    //std::cout << "bx = " << x() << " by = " << y() << std::endl;
}

void Ball::update(sf::Time delta)
{
    circle_.move(velocity_.x * delta.asSeconds(), velocity_.y * delta.asSeconds());
}

void Ball::draw(sf::RenderWindow &playScreen) const
{
    playScreen.draw(circle_);
}

void Ball::init()
{
    int randAngle = rand() % (MAX_ANGLE - MIN_ANGLE) + MIN_ANGLE + 1;
    int randDir = rand() % 2 ? 1 : -1;
    circle_.setFillColor(randDir == -1 ? sf::Color::Blue : sf::Color::Red);
    velocity_.x = randDir * VELOCITY * cos(randAngle * M_PI / 180);
    velocity_.y = VELOCITY * sin(randAngle * M_PI / 180);
    circle_.setRotation(0.f);
}

void Ball::reset()
{
    circle_.setPosition(initPos_);
    init();
}
