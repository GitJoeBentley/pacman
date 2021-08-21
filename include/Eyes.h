#ifndef EYES_H
#define EYES_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Maze.h"


class Eyes : public sf::RectangleShape
{
public:
    Eyes(Location locationInMaze);
    virtual ~Eyes();

protected:

private:
    Location location;
    Direction direction;
    float speed;
    sf::Texture eyesTexture;
    void rotate();
    void setTextureImage();
    Location getNextLocation() const;
};

#endif // EYES_H
