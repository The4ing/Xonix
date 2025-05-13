#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "LevelGrid.h"
#include "Wall.h"
#include "HUD.h"
#include "Enemy.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    sf::RenderWindow window;
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

    int currentLevelNumber;
    int lives;
    float closedAreaPercent;

    Player player;
    LevelGrid grid;

    std::vector<Wall> walls;               // holds actual wall objects
    std::vector<Enemy> enemies;
    std::vector<GameObject*> gameObjects;


    sf::Font font;
    HUD hud;
    sf::Clock gameClock;
    int score = 0;
};
