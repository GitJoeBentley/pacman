#ifndef MAZE_H
#define MAZE_H

#include "Constants.h"
#include "Cell.h"

class Maze
{
    public:
        Maze();
        virtual ~Maze();
        void draw(sf::RenderWindow& window);
        const Cell& getCell(sf::Vector2u vec) const { return cell[vec.y][vec.x]; }
        Cell::Content getCellContent(sf::Vector2u vec) const { return cell[vec.y][vec.x].getContent(); }
        void setCellContent(sf::Vector2u vec, Cell::Content content) { cell[vec.y][vec.x].setContent(content); }
        void defineCellContents(unsigned row, unsigned col, unsigned value);
        void clearCellContent(sf::Vector2u vec) { cell[vec.y][vec.x].setContent(Cell::Empty); }
    protected:

    private:
        Cell cell[Rows][Cols];
        sf::Texture cellTexture;
};

#endif // MAZE_H
