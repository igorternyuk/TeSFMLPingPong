#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Paddle
{
public:
    enum class Direction {UP, DOWN};
    explicit Paddle(float x, float y, sf::Color color, const std::string &imgPath);
    void move(sf::RenderWindow &playScreen, sf::Time delta, Direction dir);
    sf::FloatRect boundingRect() const;
    void update(sf::Time delta);
    void draw(sf::RenderWindow &playScreen) const;
    void reset();
    static constexpr int getWidth() { return FRAME_WIDTH; }
    static constexpr int getHeight() { return FRAME_HEIGHT; }
private:
    sf::RectangleShape rect_;
    sf::Vector2f initPos_;
    //Animation
    sf::Texture texture_;
    sf::Time frameTime_{sf::Time::Zero};
    const float animDelay_{0.04f};
    int currentFrame_{0};
    enum
    {
        VELOCITY = 500,
        NUM_FRAMES = 20,
        FRAME_WIDTH = 53,
        FRAME_HEIGHT = 192,
        PADDLE_SHADOW = 8
    };
};
