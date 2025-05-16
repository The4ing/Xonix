#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "LevelGrid.h"
#include "Wall.h"
#include "HUD.h"
#include "Enemy.h"
#include "SmartEnemy.h"
#include "LevelLoader.h"


class Game {
public:
    Game();
    void loadFirstLevel();
    void showSplashScreen();
    void run();
    

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    std::vector<sf::Vector2f> getEnemyPositions() const;
    sf::RenderWindow window;
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

    int currentLevelNumber;
    int lives;
    float closedAreaPercent;

    Player player;
    LevelGrid grid;

    std::vector<Wall> walls;               // holds actual wall objects
    std::vector<Enemy> enemies;
    std::vector<SmartEnemy> smartEnemies;
    std::vector<GameObject*> gameObjects;


    sf::Font font;
    HUD hud;
    sf::Clock gameClock;
    int score = 0;
    
    LevelLoader levelLoader;
    std::vector<LevelData> levelDataList;
    GameSettings settings;

    void loadLevel(int index);  // הכרזה לפונקציה שאתה מגדיר בקובץ cpp
};
