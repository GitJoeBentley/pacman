#include <iostream>
#include <fstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "DeadGhost.h"

std::map<unsigned short,unsigned short> DeadGhost::DeadGhostLocation;

DeadGhost::DeadGhost(Location locationInMaze) : location(locationInMaze), direction(Right), speed(1.0f)
{
    std::cout << "DeadGhostLocation.size()=" << DeadGhostLocation.size() << std::endl;
    if (!DeadGhostLocation.size())
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
            //std::cout << std::hex << current << ' ' << next << std::endl;
            //std::cout << temp << std::endl;
            DeadGhostLocation[current] = next;
        }
    }
    std::cout << "DeadGhostLocation.size()=" << DeadGhostLocation.size() << std::endl;
    deadGhostTexture.loadFromFile(DeadGhostsImageFile);
    setTextureImage();
}

DeadGhost::~DeadGhost()
{
    //dtor
}

void DeadGhost::setTextureImage()
{
    sf::IntRect deadGhostImageLocationInTexture;
    deadGhostImageLocationInTexture.height = static_cast<int>(GhostSize.y);
    deadGhostImageLocationInTexture.width = static_cast<int>(GhostSize.x);

    unsigned horizontalTextureOffset = 0;
    //static unsigned verticalTextureOffset = 0;

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
    deadGhostImageLocationInTexture.left = horizontalTextureOffset * GhostSize.x;

    // set texture for Ghost (sf::RectangleShape)
    //deadGhostImageLocationInTexture.top = static_cast<int>((2*num+verticalTextureOffset) * GhostSize.y);
    deadGhostImageLocationInTexture.top = 0;
    setTextureRect(deadGhostImageLocationInTexture);
    setTexture(&deadGhostTexture);
    setSize(GhostSize);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition(LeftSideOfMaze+20.0f*location.x,TopOfMaze+location.y*20.0f);
}

Location DeadGhost::getNextLocation()
{
    unsigned short key = (location.y << 8) + location.x;
    unsigned short value = DeadGhostLocation[key];
    setTextureImage();
    return Location(value%256,value/256);
}

void DeadGhost::goHome()
{
    setLocation(getNextLocation());
    setPosition(LeftSideOfMaze+20.0f*location.x,TopOfMaze+location.y*20.0f);
}

