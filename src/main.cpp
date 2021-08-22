#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <bitset>
#include <cstdlib>
#include <vector>
#include <map>

#include "Constants.h"
#include "Externs.h"
#include "Maze.h"
#include "Pacman.h"
#include "DyingPacman.h"
#include "Ghosts.h"
#include "DeadGhost.h"

using namespace std;

void startSound(sf::Sound& sound);
void stopSound(sf::Sound& sound);
void getDeadGhostReturn(map<Location,Location,bool(*)(const Location&, const Location&)> deadGhostReturn);
bool compLoc(const Location& loc1, const Location& loc2);
void createGhostTextute();
void getDeadGhostReturn();
void turnGhostsBlue(Ghost* ghost);
unsigned getGhostNumberAtLocation(Ghost* ghost, const Location& location);
bool ghostsAreBlue(Ghost* ghost);
void moveBlueGhosts(Ghost* ghost, Maze&);
bool moveGhosts(Ghost*, Maze&, Pacman&);
void drawPacman(sf::RenderWindow& window,Pacman** pacman);
void drawGhosts(sf::RenderWindow& window, Ghost* ghost);


sf::Texture GhostTexture;
std::map<unsigned short,unsigned short> DeadGhostReturn;

int main()
{
    sf::RenderWindow window(sf::VideoMode(MainWindowWidth, MainWindowHeight), "Joe's Pacman", sf::Style::Close);
    bool gameOverFlag = false;
    bool pauseFlag = false;
    Location tempLocation;
    int score = 0;
    unsigned loopCounter = 0;
    int pacmanActive = 0;
    int pacmanMoveReturn = 0;
    unsigned tempPoints = 0;
    unsigned tempGhostScoreDisplayCount = 0;

    srand(static_cast<unsigned>(time(0)));

    ///////////// Text ////////////////
    sf::Font font;
    font.loadFromFile("resources/arial.ttf");
    sf::String scoreString = "SCORE  0";
    sf::Text text(scoreString, font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(210,220,250));
    text.setPosition(60.0f,15.0f);

    sf::String ghostPointString;
    sf::Text ghostPointText(ghostPointString, font);
    ghostPointText.setCharacterSize(18);
    ghostPointText.setStyle(sf::Text::Bold);
    ghostPointText.setFillColor(sf::Color(100,240,250));

    //////////// Sound effects /////////////
    sf::SoundBuffer munchBuffer;
    munchBuffer.loadFromFile(MunchSoundFile);
    sf::Sound munchSound;
    munchSound.setBuffer(munchBuffer);
    munchSound.setLoop(true);
    munchSound.setVolume(25.0f);

    sf::SoundBuffer pacmanDeathBuffer;
    pacmanDeathBuffer.loadFromFile(PacmanDeathSoundFile);
    sf::Sound pacmanDeathSound;
    pacmanDeathSound.setBuffer(pacmanDeathBuffer);
    pacmanDeathSound.setLoop(false);
    pacmanDeathSound.setVolume(20.0f);

    sf::SoundBuffer pacmanEatBallBuffer;
    pacmanEatBallBuffer.loadFromFile(PacmanEatBallSoundFile);
    sf::Sound pacmanEatBallSound;
    pacmanEatBallSound.setBuffer(pacmanEatBallBuffer);
    pacmanEatBallSound.setLoop(false);
    pacmanEatBallSound.setVolume(20.0f);

    sf::SoundBuffer sirenBuffer;
    sirenBuffer.loadFromFile(SirenSoundFile);
    sf::Sound sirenSound;
    sirenSound.setBuffer(sirenBuffer);
    sirenSound.setLoop(true);
    sirenSound.setVolume(20.0f);

    sf::SoundBuffer eatGhostBuffer;
    eatGhostBuffer.loadFromFile(EatGhostSoundFile);
    sf::Sound eatGhostSound;
    eatGhostSound.setBuffer(eatGhostBuffer);
    eatGhostSound.setLoop(false);
    eatGhostSound.setVolume(20.0f);

    Maze maze;
    Pacman* pacman[3];
    pacman[0] = new Pacman(sf::Vector2u(14u,23u));
    pacman[1] = new Pacman(sf::Vector2u(1u, 31u));
    pacman[2] = new Pacman(sf::Vector2u(3u, 31u));

    {
        DeadGhost initializer;
    }

    DyingPacman* dyingPacmanPtr = nullptr;
    DeadGhost* deadGhostPtr = nullptr;

    createGhostTextute();
    getDeadGhostReturn();
    Ghost ghost[4];

    // Game Speed controls
    float deltaTime = .2f;
    float totalTime = 0.0f;
    sf::Clock clock;
    sf::Event evnt;
    unsigned ghostCount = 1;

    while(window.isOpen())
    {
        if (ghostCount < 4 && loopCounter == ghostCount*100  && ghost[ghostCount].isInTheBox())
        {
            ghost[ghostCount].moveToStart();
            ghostCount++;
        }
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))  // quit the game
                    gameOverFlag = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  // move to the right
                    if (pacman[pacmanActive])
                        pacman[pacmanActive]->setDirection(Right);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  // move to the left
                    if (pacman[pacmanActive])
                        pacman[pacmanActive]->setDirection(Left);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  // move to the up
                    if (pacman[pacmanActive])
                        pacman[pacmanActive]->setDirection(Up);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  // move to the down
                    if (pacman[pacmanActive])
                        pacman[pacmanActive]->setDirection(Down);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))    // pause
                {
                    if (pauseFlag)
                    {
                        startSound(munchSound);
                    }
                    else
                    {
                        stopSound(munchSound);
                    }
                    pauseFlag = !pauseFlag;
                }
                break;
            default:
                ;
            }
        }

        if (!pauseFlag)
        {
            startSound(munchSound);
            unsigned deadGhost;

            deltaTime = clock.restart().asSeconds();
            totalTime += deltaTime;

            if(totalTime >= SwitchTime)
            {
                totalTime -= SwitchTime;
                loopCounter++;

                /////////////////// move Pacman /////////////////////
                if (!dyingPacmanPtr)
                {
                    // ONly move pacman if he's not dying
                    pacmanMoveReturn = pacman[pacmanActive]->move(maze);
                    switch (pacmanMoveReturn)
                    {
                    case DotValue:
                        score += pacmanMoveReturn;
                        break;
                    case EnergizerValue:
                        score += pacmanMoveReturn;
                        pacmanEatBallSound.play();
                        turnGhostsBlue(ghost);
                        stopSound(munchSound);
                        startSound(sirenSound);
                        break;
                    case GhostValue:
                        // which ghost was killed
                        deadGhost = getGhostNumberAtLocation(ghost, pacman[pacmanActive]->getLocation());
                        ghost[deadGhost].kill();
                        stopSound(munchSound);
                        startSound(eatGhostSound);
                        tempPoints = static_cast<unsigned int>(pow(2, pacman[pacmanActive]->ghostsEaten()) * 100);
                        score += tempPoints;
                        ghostPointText.setPosition(LeftSideOfMaze + tempLocation.x * 20.0f - 15, TopOfMaze + tempLocation.y * 20.0f - 10);
                        ghostPointText.setString(sf::String(to_string(tempPoints)));
                        deadGhostPtr = new DeadGhost(tempLocation);
                        break;
                    default:
                        ;
                    }
                }
                if (deadGhostPtr)
                {
                    deadGhostPtr->goHome();
                }
                if (ghostsAreBlue(ghost))
                {
                    if (++tempGhostScoreDisplayCount == 20)
                        ghostPointText.setString("");
                    if (tempGhostScoreDisplayCount == 200)
                    {
                        Ghost::resetBlueGhosts();
                        pacman[pacmanActive]->resetGhostEaten();
                        tempGhostScoreDisplayCount = 0;
                        delete deadGhostPtr;

                        stopSound(sirenSound);
                        startSound(munchSound);
                        if (deadGhostPtr)
                            deadGhostPtr->goHome();

                        continue;
                    }
                    moveBlueGhosts(ghost,maze);
                }

                else
                {
                    if (pacman[pacmanActive] && moveGhosts(ghost, maze, *(pacman[pacmanActive])))
                    {
                        // The ghost kills Pacman
                        stopSound(munchSound);
                        startSound(pacmanDeathSound);
                        dyingPacmanPtr = new DyingPacman(pacman[pacmanActive]->getLocation());

                        delete pacman[pacmanActive];
                        pacman[pacmanActive] = nullptr;

                        if (pacmanActive > 2)
                        {
                            gameOverFlag = true;
                            break;
                        }
                    }
                }

                text.setString("Score  "+to_string(score));
                window.clear();
                window.draw(text);
                maze.draw(window);

                drawPacman(window,pacman);
                drawGhosts(window,ghost);

                if (ghostPointText.getString().getSize()>1)
                    window.draw(ghostPointText);
                if (deadGhostPtr)
                    window.draw(*deadGhostPtr);

                if (dyingPacmanPtr)
                {
                    if (dyingPacmanPtr->suffer())
                        window.draw(*dyingPacmanPtr);
                    else    // pacman is now dead
                    {
                        stopSound(pacmanDeathSound);
                        delete dyingPacmanPtr;
                        dyingPacmanPtr = nullptr;
                        pacmanActive++;

                        if (pacmanActive > 2)
                        {
                            gameOverFlag = true;
                            break;
                        }
                        pacman[pacmanActive]->moveToStart(maze);
                    }
                }

                window.display();
                if (gameOverFlag)
                    break;
            }
        }
    }
}
void startSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::SoundSource::Playing)
        sound.play();
}

void stopSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::SoundSource::Stopped)
        sound.stop();
}

bool compLoc(const Location& loc1, const Location& loc2)
{
    return loc1.x*30 + loc1.y < loc2.x*30 + loc2.y;
}

void getDeadGhostReturn(map<Location,Location,bool(*)(const Location&, const Location&)> deadGhostReturn)
{
    ifstream fin(DeadGhostsReturnFile);
    if (!fin)
    {
        cerr << "Unable to open file " << DeadGhostsReturnFile << endl;
        exit(3);
    }
    Location current, next;
    while (!fin.eof())
    {
        fin >> current.x >> current.y >> next.x >> next.y;
        deadGhostReturn[current] = next;
    }
}

void createGhostTextute()
{
    GhostTexture.loadFromFile(GhostImageFile);
}

void getDeadGhostReturn()
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
        DeadGhostReturn[current] = next;
    }
}

void turnGhostsBlue(Ghost* ghost)
{
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        ghost[i].turnGhostsBlue();
    }
}

bool ghostsAreBlue(Ghost* ghost)
{
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        if (ghost[i].isBlue())
            return true;
    }
    return false;
}

unsigned getGhostNumberAtLocation(Ghost* ghost, const Location& location)
{
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        if (ghost[i].getLocation() == location)
            return i;
    }
    return NumGhosts;
}

void moveBlueGhosts(Ghost* ghost, Maze& maze)
{
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        ghost[i].blueMove(maze);
    }
}

bool moveGhosts(Ghost* ghost, Maze& maze, Pacman& pacman)
{
    for (auto i = 0u; i < NumGhosts; ++i)
    {
        if (ghost[i].move(maze,pacman))
            return true;
    }
    return false;
}

void drawPacman(sf::RenderWindow& window,Pacman** pacman)
{
    for (auto i = 0u; i < 3; i++)
        if (pacman[i])
            window.draw(*pacman[i]);
}

void drawGhosts(sf::RenderWindow& window, Ghost* ghost)
{
    for (auto i = 0u; i < NumGhosts; ++i)
        window.draw(ghost[i]);
}
