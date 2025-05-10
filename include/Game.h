#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
//#include "Player.h"
//#include "Enemy.h"
//#include "Level.h"
//#include "HUD.h"

class Game {
public:
    Game();
    void run();

private:
    void loadLevel();
    void processEvents();
    void update(sf::Time dt);
    void render();

    //Level currentLevel;
    //Player player;
   // std::vector<Enemy> enemies;
    //HUD hud;

    int currentLevelNumber ;
    int lives ;
    float closedAreaPercent;

    sf::RenderWindow window;
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
