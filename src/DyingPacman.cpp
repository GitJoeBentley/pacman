#include <iostream>
#include "DyingPacman.h"

DyingPacman::DyingPacman(Location loc)
{
    imageCount = 9;
    dyingPacmanTexture.loadFromFile(DyingPacmanImageFile);
    /*
    sf::IntRect dyingPacmanImageLocationInTexture;
    dyingPacmanImageLocationInTexture.height = static_cast<int>(CellSize.y);
    dyingPacmanImageLocationInTexture.width = static_cast<int>(CellSize.x);
    dyingPacmanImageLocationInTexture.left = imageCount*20;
    setTextureRect(dyingPacmanImageLocationInTexture);
    setTexture(&dyingPacmanTexture);
    */
    setTextureImage();
    setSize(PacmanSize);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition(LeftSideOfMaze+20.0f*loc.x,TopOfMaze+loc.y*20.0f);
}

void DyingPacman::setTextureImage()
{
    sf::IntRect dyingPacmanImageLocationInTexture;
    dyingPacmanImageLocationInTexture.height = static_cast<int>(CellSize.y);
    dyingPacmanImageLocationInTexture.width = static_cast<int>(CellSize.x);
    dyingPacmanImageLocationInTexture.left = imageCount*20;
    setTextureRect(dyingPacmanImageLocationInTexture);
    setTexture(&dyingPacmanTexture);
}

DyingPacman::~DyingPacman()
{
    //dtor
}

bool DyingPacman::suffer()
{
    //    static int counter = 0;

    //    counter++;

    //  if (counter == 2)
    //   {
    imageCount--;
    //      counter = 0;
    if (imageCount == -1)
    {
        imageCount = 9;
        return false;
    }
    //}
    //std::cout << "imageCount = " << imageCount << std::endl;
    setTextureImage();
    return true;
}
