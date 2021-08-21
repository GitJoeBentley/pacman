#ifndef GHOSTS_H
#define GHOSTS_H

#include <string>
#include <vector>

#include "Ghost.h"

class Ghosts
{
public:
    Ghosts();
    virtual ~Ghosts();
    void draw(sf::RenderWindow& window);
    Location move(const Maze& maze, const Pacman* pacman);
    Ghost* operator[](int i)
    {
        return ghost[i];
    }
    bool areBlue() const { return blueCount; }
    void createNewGhosts(const Location* = GhostStartLocations);
    std::vector<Location> getGhostsLocations() const;
    void turnGhostsBlue();
    Location moveBlueGhosts(const Maze& maze);
    Location moveDeadGhost(unsigned ghostNum);
    unsigned getBlueCount() const { return blueCount; }
    void resetBlueGhosts() { blueCount = 0;}
    void killGhost(int number);
    static Location getNextDeadGhostLocation(Location);

protected:

private:
    static const std::string names[4];
    static std::map<unsigned short,unsigned short> DeadGhostLocation;
    Ghost* ghost[4] = {nullptr};
    unsigned blueCount = 0;
    void setTextureImage();
    void setDeadGhostTextureImage(unsigned ghostNum);
    void getDeadGhostReturnMap();
};

#endif // GHOSTS_H
