#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

const unsigned MainWindowWidth = 640;
const unsigned MainWindowHeight = 700;

const unsigned Rows = 31;
const unsigned Cols = 28;
const sf::Vector2f CellSize{20.0f,20.0f};
const sf::Vector2f PacmanSize{24.0f,24.0f};
const sf::Vector2f GhostSize{24.0f,24.0f};
const float TopOfMaze = 60.0f;
const float LeftSideOfMaze = 50.0f;
const unsigned NumberOfPacman = 3;
const unsigned NumGhosts = 4;
constexpr unsigned NotAGhost = NumGhosts + 1;
const float SwitchTime = .1f;

// Point values for Pacman eating ...
const int DotValue = 10;
const int EnergizerValue = 50;
const int GhostValue = 200;


// files
const std::string CellContentsFile = "resources/cell_contents.txt";
const std::string DeadGhostsReturnFile = "resources/DeadGhostsReturnFile.bin";

const std::string BackgroundFile = "resources/gameboard2.png";
const std::string dotFile = "resources/dot.png";
const std::string MazeImagesFile = "resources/maze_images.png";
const std::string PacmanImageFile = "resources/pacman.png";
const std::string DyingPacmanImageFile = "resources/pacmandying.png";
const std::string GhostImageFile = "resources/ghosts.bmp";
const std::string DeadGhostsImageFile = "resources/eyes.bmp";

const std::string MunchSoundFile = "resources/munch.wav";
const std::string PacmanDeathSoundFile = "resources/pacman_death.wav";
const std::string PacmanEatBallSoundFile = "resources/eatball.wav";
const std::string SirenSoundFile = "resources/siren.wav";
const std::string EatGhostSoundFile = "resources/eatghost.wav";

enum Direction { Up = 2, Left = 3, Down = 5, Right = 7, NoDirection = 11};
using Location = sf::Vector2u;
const Location GhostStartLocation = sf::Vector2u(13u,11u);
const Location DummyLocation = sf::Vector2u(0u,0u);
const Location GhostStartLocations[4] {GhostStartLocation,Location(11u,14u),Location(13u,14u),Location(15u,14u)};

#endif
