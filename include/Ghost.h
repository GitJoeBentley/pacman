#ifndef GHOST_H
#define GHOST_H

#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Constants.h"
#include "Externs.h"
#include "Maze.h"

class Pacman;  // forward declaration

class Ghost : public sf::RectangleShape
{

public:
    enum State {InTheBox, Alive, Blue, Dead};
    static const std::string names[4];
    Ghost();
    virtual ~Ghost();
    void setLocation(Location loc)
    {
        location = loc;
    }
    Location getLocation() const
    {
        return location;
    }
    Direction getDirection() const
    {
        return direction;
    }
    void setDirection(Direction dir)
    {
        direction = dir;
    }
    float getSpeed() const
    {
        return speed;
    }
    bool move(Maze& maze, const Pacman& pacman);
    void blueMove(Maze& maze);
    unsigned getPossibleMoveDirections(const Maze& maze);
    void loseDirection()
    {
        direction = NoDirection;
    }
    void determineNewDirection(const Maze& maze, const Pacman& pacman);
    void moveToStart();
    bool isInTheBox() const;
    void moveInTheBox();
    Direction setRandomDirection();
    State getState() const
    {
        return state;
    }
    void setState(State state_)
    {
        state = state_;
    }
    Location getNextDeadGhostLocation() const;
    void goHomeDeadGhost();
    void turnGhostsBlue();

    bool isBlue() const
    {
        return state == Blue;
    }
    void kill();

    static void resetGhostNumberInitializer();
    static unsigned getBlueCount()
    {
        return blueCount;
    }
    static void resetBlueGhosts()
    {
        blueCount = 0;
    }

protected:

private:
    std::string name;
    sf::Texture& ghostTexture;

    State state = InTheBox;
    Location location;
    float speed;
    Direction direction = NoDirection;
    unsigned number;    // ghost number
    Cell::Content previousLocationCellContent;

    void setTextureImage();

    static unsigned GhostNumberInitializer;
    //static std::map<unsigned short,unsigned short> DeadGhostLocation;
    static unsigned blueCount;
    //static void getDeadGhostReturnMap();
};

#endif // GHOST_H
