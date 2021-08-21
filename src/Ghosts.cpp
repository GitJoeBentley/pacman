#include <fstream>

#include "Ghost.h"
#include "Ghosts.h"
#include "Pacman.h"
#include "Constants.h"

const std::string Ghosts::names[4] = {"Blinky","Pinky","Inky","Clyde"};
std::map<unsigned short,unsigned short> Ghosts::DeadGhostLocation;

Ghosts::Ghosts()
{
    ghostTexture.loadFromFile(GhostImageFile);
    createNewGhosts();
    getDeadGhostReturnMap();
}

void Ghosts::getDeadGhostReturnMap()
{
    unsigned temp;
    std::ifstream fin;
    short current,next;
    fin.open(DeadGhostsReturnFile,std::ios_base::binary);
    if (!fin)
    {
        std::cerr << "unable to open file " << DeadGhostsReturnFile << std::endl;
        std::exit(17);
    }
    while (fin.read(reinterpret_cast<char*>(&temp),sizeof(temp)))
    {
        current = temp >> 16;
        next = temp & 0x0000ffff;
        DeadGhostLocation[current] = next;
    }
}

void Ghosts::createNewGhosts(const Location* loc)
{
    Ghost::resetGhostNumberInitializer();
    ghost[0] = new Ghost(names[0], loc[0], 0.9f, Right);
    ghost[1] = new Ghost(names[1], loc[1], 0.8f);
    ghost[2] = new Ghost(names[2], loc[2], 0.7f);
    ghost[3] = new Ghost(names[3], loc[3], 0.6f);
    setTextureImage();
}

void Ghosts::setTextureImage()
{
    sf::IntRect ghostImageLocationInTexture;
    ghostImageLocationInTexture.height = static_cast<int>(GhostSize.y);
    ghostImageLocationInTexture.width = static_cast<int>(GhostSize.x);

    unsigned horizontalTextureOffset = 0;
    static unsigned verticalTextureOffset = 0;
    // animate
    ++verticalTextureOffset %= 2;

    for (auto num = 0u; num < NumGhosts; ++num)
    {
        // set eye direction
        if (ghost[num]->getDirection() == Down)
            horizontalTextureOffset = 0;
        else if (ghost[num]->getDirection() == Up)
            horizontalTextureOffset = 1;
        else if (ghost[num]->getDirection() == Right)
            horizontalTextureOffset = 2;
        else if (ghost[num]->getDirection() == Left)
            horizontalTextureOffset = 3;
        else
            horizontalTextureOffset = 0;
        ghostImageLocationInTexture.left = horizontalTextureOffset * GhostSize.x;

        // set texture for Ghost (sf::RectangleShape)
        ghostImageLocationInTexture.top = static_cast<int>((2*num+verticalTextureOffset) * GhostSize.y);
        ghost[num]->setTextureRect(ghostImageLocationInTexture);
        ghost[num]->setTexture(&ghostTexture);
        ghost[num]->setOrigin(ghost[num]->getLocalBounds().width/2.0f,ghost[num]->getLocalBounds().height / 2.0f);
    }
}

void Ghosts::turnGhostsBlue()
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
    ghostImageLocationInTexture.left = horizontalTextureOffset * GhostSize.x;
    for (auto num = 0u; num < NumGhosts; ++num)
    {
        ghost[num]->setState(Ghost::Blue);
        if (ghost[num])
        {
            ghost[num]->setTextureRect(ghostImageLocationInTexture);
            ghost[num]->setTexture(&ghostTexture);
            ghost[num]->setOrigin(ghost[num]->getLocalBounds().width/2.0f,ghost[num]->getLocalBounds().height / 2.0f);
        }
    }
}

Ghosts::~Ghosts()
{
    //dtor
}

void Ghosts::draw(sf::RenderWindow& window)
{
    for (auto num = 0u; num < NumGhosts; ++num)
    {
        if (ghost[num])
            window.draw(*(ghost[num]));
    }
}

Location Ghosts::move(const Maze& maze, const Pacman* pacman)
{
    setTextureImage();
    sf::Vector2f nextPosition;
    sf::Vector2u nextLocation;
    std::vector<Location> ghostLocations;

    for (auto num = 0u; num < NumGhosts; ++num)
    {
        // don't move a ghost to a location of another ghost
        ghostLocations = getGhostsLocations();

        ghost[num]->move(maze, ghostLocations);

        if (ghost[num]->getLocation() == pacman ->getLocation())
        {
            // std::cout << "Chost gets pacman" << std::endl;
            return pacman ->getLocation();
        }

        ghost[num]->determineNewDirection(maze, pacman);
    }
    return DummyLocation;
}

// Do I need to return a Location and do I need to track pacman
Location Ghosts::moveBlueGhosts(const Maze& maze)
{
    turnGhostsBlue();
    for (auto num = 0u; num < NumGhosts; ++num)
    {
        if (ghost[num])
            ghost[num]->blueMove(maze);
    }
    return DummyLocation;
}

Location Ghosts::moveDeadGhost(unsigned ghostNum)
{
    setDeadGhostTextureImage(ghostNum);

}


std::vector<Location> Ghosts::getGhostsLocations() const
{
    std::vector<Location> locations(NumGhosts);
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        locations[i] = ghost[i]->getLocation();
    }
    return locations;
}

void Ghosts::killGhost(int number)
{
    ghost[number]->setState(Ghost::Dead);
}

void Ghosts::setDeadGhostTextureImage(unsigned ghostNum)
{
    sf::IntRect deadGhostImageLocationInTexture;
    deadGhostImageLocationInTexture.height = static_cast<int>(GhostSize.y);
    deadGhostImageLocationInTexture.width = static_cast<int>(GhostSize.x);

    unsigned horizontalTextureOffset = 0;

    // set eye direction
    if (ghost[ghostNum]->getDirection() == Down)
        horizontalTextureOffset = 0;
    else if (ghost[ghostNum]->getDirection() == Up)
        horizontalTextureOffset = 1;
    else if (ghost[ghostNum]->getDirection() == Right)
        horizontalTextureOffset = 2;
    else if (ghost[ghostNum]->getDirection() == Left)
        horizontalTextureOffset = 3;
    else
        horizontalTextureOffset = 0;
    deadGhostImageLocationInTexture.left = horizontalTextureOffset * GhostSize.x;

    // set texture for Ghost (sf::RectangleShape)
    deadGhostImageLocationInTexture.top = static_cast<int>(10 * GhostSize.y);
    ghost[ghostNum]->setTextureRect(deadGhostImageLocationInTexture);
    ghost[ghostNum]->setTexture(&ghostTexture);
    ghost[ghostNum]->setSize(GhostSize);
    ghost[ghostNum]->setOrigin(ghost[ghostNum]->getLocalBounds().width/2.0f,ghost[ghostNum]->getLocalBounds().height / 2.0f);
    ghost[ghostNum]->setPosition(LeftSideOfMaze+20.0f*ghost[ghostNum]->getLocation().x,TopOfMaze+ghost[ghostNum]->getLocation().y*20.0f);
}

Location Ghosts::getNextDeadGhostLocation(Location loc)
{
    unsigned short key = (loc.y << 8) + loc.x;
    unsigned short value = DeadGhostLocation[key];
    return Location(value%256,value/256);
}
