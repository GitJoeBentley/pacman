#include <iostream>
#include <fstream>
#include "Maze.h"

using namespace std;

Maze::Maze()
{
    cellTexture.loadFromFile(MazeImagesFile);
    sf::IntRect invaderImageLocationInTexture;
    sf::IntRect cellImageLocationInTexture;
    cellImageLocationInTexture.height = static_cast<int>(CellSize.y);
    cellImageLocationInTexture.width = static_cast<int>(CellSize.x);
    cellImageLocationInTexture.left = 0;

    ifstream fin(CellContentsFile);
    if (!fin)
    {
        cerr << "Unable to open file " << CellContentsFile << endl;
        exit(1);
    }

    unsigned value;

    for (auto row = 0u; row < Rows; ++row)
    {
        for (auto col = 0u; col < Cols; ++col)
        {
            fin >> value;

            cell[row][col].setContent(value);
            cellImageLocationInTexture.top = static_cast<int>(value * CellSize.y);
            cell[row][col].setTextureRect(cellImageLocationInTexture);
            cell[row][col].setTexture(&cellTexture);

            cell[row][col].setSize(CellSize);
            cell[row][col].setOrigin(cell[row][col].getLocalBounds().width/2.0f,cell[row][col].getLocalBounds().height / 2.0f);
            cell[row][col].setPosition(LeftSideOfMaze+col*20.0f,TopOfMaze+row*20.0f);
        }
    }
}

void Maze::defineCellContents(unsigned row, unsigned col, unsigned value)
{
    cell[row][col].setContent(value);
}


Maze::~Maze()
{
    //dtor
}

void Maze::draw(sf::RenderWindow& window)
{
    for (auto row = 0u; row < Rows; ++row)
    {
        for (auto col = 0u; col < Cols; ++col)
        {
            if (cell[row][col].getContent() != Cell::Empty)  window.draw(cell[row][col]);
        }
    }
}
