#include <iostream>
using namespace std;
#include "Constants.h"
#include "Pacman.h"

Pacman::Pacman(Location locationInMaze)
    : location(locationInMaze), direction(Right), speed(1.0f)
{
    pacmanTexture.loadFromFile(PacmanImageFile);
    setTextureImage();
}

void Pacman::setTextureImage(int imageCount)
{
    sf::IntRect pacmanImageLocationInTexture;
    pacmanImageLocationInTexture.height = static_cast<int>(CellSize.y);
    pacmanImageLocationInTexture.width = static_cast<int>(CellSize.x);
    pacmanImageLocationInTexture.left = imageCount*20;
    setTextureRect(pacmanImageLocationInTexture);
    setTexture(&pacmanTexture);
    setSize(PacmanSize);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition(LeftSideOfMaze+20.0f*location.x,TopOfMaze+location.y*20.0f);
}

Pacman::~Pacman()
{
    //dtor
}

void Pacman::moveToStart(Maze& maze)
{
    cout << "move pacman to start" << endl;
    location = Location(14u,23u);
    setPosition(LeftSideOfMaze+20.0f*location.x,TopOfMaze+location.y*20.0f);
    maze.setCellContent(location,Cell::Pacman);
}

int Pacman::move(Maze& maze)
{
    int returnValue = 0;
    static int counter = 1;
    if (counter != 3)
    {
        setTextureImage(counter);
        counter++;
        return returnValue;
    }
    else
    {
        sf::Vector2f nextPosition = getPosition();
        sf::Vector2u nextLocation = location;

        switch (direction)
        {
        case Up:
            nextPosition.y -= CellSize.y;
            nextLocation.y -= 1;
            break;
        case Down:
            nextPosition.y += CellSize.y;
            nextLocation.y += 1;
            break;
        case Left:
            nextPosition.x -= CellSize.x;
            nextLocation.x -= 1;
            // wrap around
            if (location.x == 0)
            {
                nextLocation.x = 27;
                nextPosition.x = LeftSideOfMaze+20.0f*nextLocation.x;
            }
            break;
        case Right:
            nextPosition.x += CellSize.x;
            nextLocation.x += 1;

            // wrap around
            if (nextLocation.x == 28)
            {
                nextLocation.x = 0;
                nextPosition.x = LeftSideOfMaze;
            }
            break;
        default:
            ;
        }

        // if the new location contains a dot, eat it
        if (maze.getCellContent(nextLocation) == Cell::Dot)
        {
            maze.setCellContent(nextLocation, Cell::Empty);
            returnValue = 10;
        }

        // if the new location contains a Energizer, eat it
        if (maze.getCellContent(nextLocation) == Cell::Energizer)
        {
            maze.setCellContent(nextLocation, Cell::Empty);
            returnValue = EnergizerValue;
        }

        // if the new location contains a Ghost, don't move there - it would be suicide
        if (maze.getCellContent(nextLocation) == Cell::Ghost)
        {
            return 0;
        }

        // if the new location contains a Blue Ghost, eat it
        if (maze.getCellContent(nextLocation) == Cell::BlueGhost)
        {
            std::cout << "*** Pacman ate a blue ghost at " << nextLocation.x << ',' << nextLocation.y << std::endl;
            location = nextLocation;
            setPosition(nextPosition);
            maze.setCellContent(location, Cell::Pacman);
            ghostEaten++;
            returnValue = 200;
        }

        else if (!maze.getCell(nextLocation).isAWall())
        {
            maze.setCellContent(location,Cell::Empty);
            location = nextLocation;
            setPosition(nextPosition);
            maze.setCellContent(location,Cell::Pacman);
            // std::cout << "Pacman located at " << location.x << ',' << location.y << std::endl;
        }
        else
        {

        }
    }
    counter = 1;
    setTextureImage();
    return returnValue;
}

void Pacman::setDirection(Direction dir)
{
    direction = dir;
    rotate();
}

void Pacman::rotate()
{
    if (direction == Up)
        setRotation(-90);
    if (direction == Down)
        setRotation(90);
    if (direction == Right)
        setRotation(0);
    if (direction == Left)
        setRotation(180);
}
/*
Location Pacman::eatGhost(Ghost& ghost)
{
        if (ghosts[i] && location == ghosts[i]->getLocation())
        {
            ghostEaten++;
            ghosts.killGhost(i);
            return location;
        }
    return DummyLocation;
}
*/
/*
Location Pacman::killGhost(Ghost& ghost)
{
    if (location == ghost.getLocation() && ghost.getState() == Ghost::Blue)
    {
        ghostEaten++;
        ghost.kill();
        return location;
    }
    return DummyLocation;  // ghost not killed
}
*/
