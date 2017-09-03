#pragma once

class Game;

class ScreenState
{
public:
    virtual ~ScreenState() {}
    virtual void handleEvents(Game&) = 0;
    virtual void update(Game&) = 0;
    virtual void draw(Game&) = 0;
};
