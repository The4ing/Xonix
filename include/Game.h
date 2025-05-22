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
    void showSplashScreen();
    void run();
    

private:
    void loadFirstLevel();
    void loadLevel(int index);
    void processEvents();
    void update(sf::Time dt);
    void render();
    void loseLife();
    void showGameOverScreen();
    void restart();
    void updateClosedAreaPercent();
    std::vector<sf::Vector2f> getEnemyPositions() const;

    Player player;
    LevelGrid grid;
    std::vector<Wall> walls;             
    std::vector<Enemy> enemies;
    std::vector<SmartEnemy> smartEnemies;
    std::vector<GameObject*> gameObjects;


    sf::Font font;
    sf::Font lostFont;
    sf::Text gameOverText;
    sf::Clock gameClock;
    HUD hud;
    
    LevelLoader levelLoader;
    std::vector<LevelData> levelDataList;
    GameSettings settings;
    sf::RenderWindow window;
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
    int currentLevelNumber;
    int lives;
    float closedAreaPercent;
    float requiredAreaPercent;
    float remainingTime;

};
