#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Cell : public sf::RectangleShape
{
    public:

        enum Content {Empty, Dot, Energizer, HW, VH, ULW, LLW, URW, LRW, Door, Pacman, Ghost, BlueGhost};
        Cell(unsigned value = 0);
        virtual ~Cell();
        void setContent(unsigned value);
        void setContent(Content value);
        Content getContent() const { return content; }
        bool isAWall() const { return content >= HW && content <= Pacman; /* && content <= LRW; */ }
        bool isNotAWall() const { return content < HW || content > Door; }

    protected:

    private:
        Content content;
};

#endif // CELL_H

/* Content values
   Empty = Empty Cell
   Dot = Dot
   Energizer = Energizer (pill)
   HW = Horizontal Wall
   VW = Vertical Wall
   ULW = Upper Left (curved) Wall
   URW = Upper Right (curved) Wall
   LLW = Lower Left (curved) Wall
   LRW = Lower Right (curved) Wall
  Door = Door
*/
