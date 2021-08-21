#ifndef DEADGHOST_H
#define DEADGHOST_H

#include <map>
#include "Constants.h"

class DeadGhost : public sf::RectangleShape
{
public:
    DeadGhost(Location = DummyLocation);
    virtual ~DeadGhost();
    Location getLocation() const { return location; }
    void setLocation(Location nextLoc) { location = nextLoc; }
    Location getNextLocation();
    void goHome();
protected:

private:
    static std::map<unsigned short,unsigned short> DeadGhostLocation;
    Location location;
    Direction direction;
    float speed;
    sf::Texture deadGhostTexture;
    void setTextureImage();
};

#endif // DEADGHOST_H
