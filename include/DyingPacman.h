#ifndef DYINGPACMAN_H
#define DYINGPACMAN_H

#include <SFML/Graphics.hpp>

#include "Constants.h"

class DyingPacman : public sf::RectangleShape
{
    public:
        DyingPacman(Location loc);
        virtual ~DyingPacman();
        bool suffer();
        void resetImageCount() { imageCount = 9; }

    protected:

    private:
        sf::Texture dyingPacmanTexture;
        int imageCount;
        void setTextureImage();
};

#endif // DYINGPACMAN_H
