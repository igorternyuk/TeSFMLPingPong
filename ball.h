#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Ball
{
public:
    explicit Ball(float x, float y, const std::string &imgPath);
    inline void setColor(sf::Color color) { circle_.setFillColor(color); }
    sf::FloatRect boundingRect() const { return circle_.getGlobalBounds(); }
    inline auto x() const noexcept { return circle_.getPosition().x; }
    inline auto y() const noexcept { return circle_.getPosition().y; }
    inline auto top() const noexcept {return circle_.getPosition().y - RADIUS;}
    inline auto bottom() const noexcept {return circle_.getPosition().y + RADIUS;}
    inline auto left() const noexcept {return circle_.getPosition().x - RADIUS;}
    inline auto right() const noexcept { return circle_.getPosition().x + RADIUS;}
    inline auto vx() const noexcept { return velocity_.x; }
    inline auto vy() const noexcept { return velocity_.y; }
    void reflect(float overlapX, float overlapY,
                 bool isVerticalSurface = true);
    void update(sf::Time delta);
    void draw(sf::RenderWindow &playScreen) const;
    void init();
    void reset();
private:
    enum
    {
        RADIUS = 18,
        VELOCITY = 500,
        MAX_ANGLE = 60,
        MIN_ANGLE = 5
    };
    sf::CircleShape circle_;
    sf::Vector2f initPos_;
    sf::Vector2f velocity_;
    sf::Texture texture_;
};
