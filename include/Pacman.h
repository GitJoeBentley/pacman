#ifndef PACMAN_H
#define PACMAN_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Maze.h"
#include "Ghosts.h"

using sf::RectangleShape;
using sf::Vector2u;
using sf::Texture;

class Pacman : public RectangleShape
{
public:
    Pacman(Location locationInMaze);
    virtual ~Pacman();
    void setLocationInMaze(Vector2u loc) { location = loc; }
    Vector2u getLocation() const { return location; }
    void setDirection(Direction dir);
    int move(Maze& maze);
    void moveToStart(Maze& maze);
    Location eatGhost(Ghost& ghost);
    unsigned ghostsEaten() const { return ghostEaten; }
    void resetGhostEaten() { ghostEaten = 0; }
    Location killGhost(Ghost& ghost);
protected:
private:
    Location location;
    Direction direction;
    const float speed;
    Texture pacmanTexture;
    void rotate();
    void setTextureImage(int imageCount = 0);
    unsigned ghostEaten = 0;
};

#endif // PACMAN_H
