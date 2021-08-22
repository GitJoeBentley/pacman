#include <iostream>
#include <fstream>
#include "Ghost.h"
#include "Pacman.h"

// static member initialization
unsigned Ghost::GhostNumberInitializer = 0;
const std::string Ghost::names[4] = {"Blinky","Pinky","Inky","Clyde"};
unsigned Ghost::blueCount = 0;
sf::Texture ghostTexture;


void Ghost::resetGhostNumberInitializer()
{
    GhostNumberInitializer = 0;
}

Ghost::Ghost()
: sf::RectangleShape(GhostSize),
  name(names[GhostNumberInitializer]),
  ghostTexture(GhostTexture),
  location(GhostStartLocations[GhostNumberInitializer]),
  speed(0.9f-0.1f*GhostNumberInitializer),
  number(GhostNumberInitializer),
  previousLocationCellContent(Cell::Empty)
{
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition(LeftSideOfMaze+20.0f*(location.x),TopOfMaze+location.y*20.0f);
    setTextureImage();
    if (GhostNumberInitializer == 0)
        moveToStart();
    GhostNumberInitializer++;
}

void Ghost::setTextureImage()
{
    sf::IntRect ghostImageLocationInTexture;
    ghostImageLocationInTexture.height = static_cast<int>(GhostSize.y);
    ghostImageLocationInTexture.width = static_cast<int>(GhostSize.x);

    unsigned horizontalTextureOffset = 0;
    static unsigned verticalTextureOffset = 0;
    // animate
    ++verticalTextureOffset %= 2;

    // set eye direction
    if (direction == Down)
        horizontalTextureOffset = 0;
    else if (direction == Up)
        horizontalTextureOffset = 1;
    else if (direction == Right)
        horizontalTextureOffset = 2;
    else if (direction == Left)
        horizontalTextureOffset = 3;
    else
        horizontalTextureOffset = 0;
    ghostImageLocationInTexture.left = static_cast<int>(horizontalTextureOffset * GhostSize.x);

    // set texture for Ghost (sf::RectangleShape)
    ghostImageLocationInTexture.top = static_cast<int>((2*number+verticalTextureOffset) * GhostSize.y);
    setTextureRect(ghostImageLocationInTexture);
    setTexture(&ghostTexture);
    // setOrigin(ghost[num]->getLocalBounds().width/2.0f,ghost[num]->getLocalBounds().height / 2.0f);
}

Ghost::~Ghost()
{
    GhostNumberInitializer--;
}

Direction Ghost::setRandomDirection()
{
    switch (rand() % 4)
    {
    case 0:
        return Up;
    case 1:
        return Down;
    case 2:
        return Left;
    default:
        ;
    }
    return Right;
}


bool Ghost::move(Maze& maze, const Pacman& pacman)
{
    if (isInTheBox())
    {
        moveInTheBox();
        return false;
    }
    // skip a move based on the ghost number
    if (rand() % (6 - number) < 3)
        return false;
    // move in a random direction based on the ghost number
    if (rand() % (6 - number) == 0)
    {
        loseDirection();
        return false;
    }

    sf::Vector2f nextPosition = getPosition();
    sf::Vector2u nextLocation = location;

    // Determine the next location of the ghost
    switch (direction)
    {
    case Right:
        nextLocation.x += 1;
        nextPosition.x += CellSize.x;
        break;
    case Left:
        if (!(nextLocation.x == 1 and nextLocation.y == 14))
        {
            nextLocation.x -= 1;
            nextPosition.x -= CellSize.x;
        }
        break;
    case Up:
        nextLocation.y -= 1;
        nextPosition.y -= CellSize.x;
        break;
    case Down:
        nextLocation.y += 1;
        nextPosition.y += CellSize.x;
        break;
    default:
        ;
    }

    //  Don't move to another ghost location
    if (maze.getCellContent(nextLocation) == Cell::Ghost)
    {
        if (direction == Right)
            direction = Left;
        else if (direction == Left)
            direction = Right;
        else
            determineNewDirection(maze,pacman);
        return false;
    }

    if (maze.getCellContent(nextLocation) == Cell::Pacman)
    {
        std::cout << "Ghost " << name << " gets Pacman at " << nextLocation.x << ',' << nextLocation.y << std::endl;
        maze.setCellContent(location,Cell::Empty);
        location = nextLocation;
        setPosition(nextPosition);
        maze.setCellContent(location,Cell::Ghost);
        return true;
    }
    if (maze.getCell(nextLocation).isNotAWall())
    {
        maze.setCellContent(location,previousLocationCellContent);
        previousLocationCellContent = maze.getCellContent(nextLocation);
        maze.setCellContent(nextLocation,Cell::Ghost);
        location = nextLocation;
        setPosition(nextPosition);
    }

    determineNewDirection(maze,pacman);
    return false;
}

void Ghost::blueMove(Maze& maze)
{
    if (rand()%2)
        return; // slow the ghost down
    if (isInTheBox())
    {
        moveInTheBox();
        return;
    }
    unsigned possibleDirections = getPossibleMoveDirections(maze);

    sf::Vector2f nextPosition = getPosition();
    sf::Vector2u nextLocation = location;

    // Determine the next location of the blue ghost
    if (direction == Right)
    {
        if (possibleDirections % Right == 0)
        {
            nextLocation.x += 1;
            nextPosition.x += CellSize.x;
        }
        else // can't move right, change direction
        {
            direction = setRandomDirection();
            return;
        }
    }
    else if (direction == Left && possibleDirections)
    {
        if (possibleDirections % Left == 0)
        {
            nextLocation.x -= 1;
            nextPosition.x -= CellSize.x;
        }
        else // can't move left, change direction
        {
            direction = setRandomDirection();
            return;
        }
    }
    else if (direction == Up)
    {
        if (possibleDirections % Up == 0)
        {
            nextLocation.y -= 1;
            nextPosition.y -= CellSize.x;
        }
        else // can't move up, change direction
        {
            direction = setRandomDirection();
            return;
        }
    }
    else if (direction == Down)
    {
        if (possibleDirections % Down == 0)
        {
            nextLocation.y += 1;
            nextPosition.y += CellSize.x;
        }
        else // can't move down, change direction
        {
            direction = setRandomDirection();
            return;
        }
    }
    else
    {
        // don't move ghost
    }

    if (maze.getCell(nextLocation).isNotAWall())
    {
        maze.setCellContent(location,previousLocationCellContent);
        previousLocationCellContent = maze.getCellContent(nextLocation);
        maze.setCellContent(nextLocation,Cell::BlueGhost);

        location = nextLocation;
        setPosition(nextPosition);
    }
}

void Ghost::moveInTheBox()
{
    float rightSideOfTheBox = LeftSideOfMaze+20.0f*(15.5);
    if (direction == NoDirection || direction == Down)
        direction = Right;
    if (direction == Right)
        setPosition(getPosition().x+10.0f,getPosition().y);
    if (direction == Left)
        setPosition(getPosition().x-10.0f,getPosition().y);
    if (getPosition().x > rightSideOfTheBox)
    {
        direction = Left;
    }
    if (getPosition().x < LeftSideOfMaze+20.0f*(11.5))
    {
        direction = Right;
    }
}

unsigned Ghost::getPossibleMoveDirections(const Maze& maze)
{
    unsigned possibleDirections = 1;

    if (maze.getCell(Location(location.x,location.y-1)).isNotAWall() &&
            maze.getCell(Location(location.x,location.y-1)).getContent() != Cell::Ghost)
        possibleDirections *= Up;
    if (maze.getCell(sf::Vector2u(location.x-1,location.y)).isNotAWall() && location.x != 1 &&
            maze.getCell(Location(location.x-1,location.y)).getContent() != Cell::Ghost)
        possibleDirections *= Left;
    if (maze.getCell(sf::Vector2u(location.x,location.y+1)).isNotAWall() &&
            maze.getCell(Location(location.x,location.y+1)).getContent() != Cell::Ghost)
        possibleDirections *= Down;
    if (maze.getCell(sf::Vector2u(location.x+1,location.y)).isNotAWall() && location.x != 26 &&
            maze.getCell(Location(location.x+1,location.y)).getContent() != Cell::Ghost)
        possibleDirections *= Right;
    return possibleDirections;
}

void Ghost::determineNewDirection(const Maze& maze, const Pacman& pacman)
{
    if (isInTheBox())
        return;

    // Determine possible move directions
    unsigned possibleDirections = getPossibleMoveDirections(maze);
    Location pacLoc = pacman.getLocation();
    Location ghostLoc = getLocation();

    int deltaX = ghostLoc.x - pacLoc.x;
    int deltaY = ghostLoc.y - pacLoc.y;

    // move horizontal?
    if (abs(deltaX) > abs(deltaY) && (possibleDirections % Left == 0 || possibleDirections % Right == 0))
    {
        // move left?
        if (ghostLoc.x > pacLoc.x)
        {
            if (possibleDirections % Left == 0)
            {
                setDirection(Left);
                return;
            }
        }
        // move right?
        if (ghostLoc.x < pacLoc.x)
        {
            if (possibleDirections % Right == 0)
            {
                setDirection(Right);
                return;
            }
        }
    }
    // move vertical
    else if (/*abs(deltaX) < abs(deltaY) && */ (possibleDirections % Up == 0 || possibleDirections % Down == 0))
    {
        // move up?
        if (ghostLoc.y > pacLoc.y)
        {
            if (possibleDirections % Up == 0)
            {
                setDirection(Up);
                return;
            }
        }
        // move down?
        if (ghostLoc.y < pacLoc.y)
        {
            if (possibleDirections % Down == 0)
            {
                setDirection(Down);
                return;
            }
        }
    }
    else if (possibleDirections % Left == 0 || possibleDirections % Right == 0)
    {
        // move left?
        if (ghostLoc.x > pacLoc.x)
        {
            if (possibleDirections % Left == 0)
            {
                setDirection(Left);
                return;
            }
        }
        // move right?
        if (ghostLoc.x < pacLoc.x)
        {
            if (possibleDirections % Right == 0)
            {
                setDirection(Right);
                return;
            }
        }
    }
    else if (possibleDirections % Up == 0 || possibleDirections % Down == 0)
    {
        // move up?
        if (ghostLoc.y > pacLoc.y)
        {
            if (possibleDirections % Up == 0)
            {
                setDirection(Up);
                return;
            }
        }
        // move down?
        if (ghostLoc.y < pacLoc.y)
        {
            if (possibleDirections % Down == 0)
            {
                setDirection(Down);
                return;
            }
        }
        else
        {
            // don't move
        }

    }
}

void Ghost::moveToStart()
{
    location = GhostStartLocation;
    direction = Right;
    setPosition(LeftSideOfMaze + 20.0f * (location.x), TopOfMaze + location.y * 20.0f);
}

bool Ghost::isInTheBox() const
{
    return location.y == 14u && (location.x > 9 && location. x < 17);
}


/*
void Ghost::goHomeDeadGhost()
{
    setLocation(Ghosts::getNextDeadGhostLocation(location));
    setPosition(LeftSideOfMaze+20.0f*location.x,TopOfMaze+location.y*20.0f);
}
*/

void Ghost::turnGhostsBlue()
{
    blueCount++;
    sf::IntRect ghostImageLocationInTexture;
    ghostImageLocationInTexture.height = static_cast<int>(GhostSize.y);
    ghostImageLocationInTexture.width = static_cast<int>(GhostSize.x);

    unsigned horizontalTextureOffset = 0;
    static unsigned verticalTextureOffset = 0;
    // animate
    ++verticalTextureOffset %= 2;
    ghostImageLocationInTexture.top = static_cast<int>((2*4+verticalTextureOffset) * GhostSize.y);
    ghostImageLocationInTexture.left = static_cast<int>(horizontalTextureOffset * GhostSize.x);
    state = Blue;
    setTextureRect(ghostImageLocationInTexture);
    setTexture(&ghostTexture);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
}

void Ghost::kill()
{
    state = Dead;
}
