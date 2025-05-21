#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include <stdexcept>
#include <iostream>



Game::Game()
    : levelLoader("C:/Users/User/source/repos/The4ing/Xonix/resources/levels.txt"),  
    font(),
    hud(sf::Vector2f(0, 960), sf::Vector2f(1920, 120), font),
    currentLevelNumber(0),
    lives(3),
    closedAreaPercent(0.0f),
    player(0, 29, 32.0f),
    grid(30, 60, 32.0f)
{

    if (!font.loadFromFile("resources/arial.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }
}


void Game::loadFirstLevel() {
    loadLevel(0);  
}


void Game::loadLevel(int levelNumber) {
    levelLoader.load();

    currentLevelNumber = levelNumber;

   
    enemies.clear();
    smartEnemies.clear();
    walls.clear();
    gameObjects.clear();


    settings = levelLoader.getSettings(); 
    window.create(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Xonix Game");
    lives = settings.initialLives;

    levelLoader.loadLevel(currentLevelNumber, grid, enemies, smartEnemies);


    player.setWindowSize(sf::Vector2f(window.getSize()));
    player.setGrid(&grid);

    std::cout << "Window size: " << settings.windowSize.x << "x" << settings.windowSize.y << std::endl;

    gameObjects.push_back(&player);

  
    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            if (grid.get(row, col) == TileType::Wall) {
                sf::Vector2f pos(col * grid.getTileSize(), row * grid.getTileSize());
                walls.emplace_back(pos, grid.getTileSize());
            }
        }
    }

   
    for (auto& wall : walls)
        gameObjects.push_back(&wall);

    for (auto& enemy : enemies)
        gameObjects.push_back(&enemy);

    for (auto& smart : smartEnemies)
        gameObjects.push_back(&smart);
}




void Game::showSplashScreen() {
    sf::RenderWindow splash(sf::VideoMode(800, 600), "Welcome");

    sf::Text title("XONIX", font, 80);
    title.setPosition(220, 200);
    title.setFillColor(sf::Color::White);

    sf::Text prompt("Press any key to start", font, 30);
    prompt.setPosition(240, 350);
    prompt.setFillColor(sf::Color::White);

    while (splash.isOpen()) {
        sf::Event event;
        while (splash.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                splash.close();
            else if (event.type == sf::Event::KeyPressed)
                splash.close();
        }

        splash.clear(sf::Color::Black);
        splash.draw(title);
        splash.draw(prompt);
        splash.display();
    }
}


void Game::run() {
    showSplashScreen();   
    loadFirstLevel();     

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }

        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time dt) {
    player.handleInput();
    player.update(dt);

    int currentRow = static_cast<int>(player.getPosition().y / grid.getTileSize());
    int currentCol = static_cast<int>(player.getPosition().x / grid.getTileSize());

    if (grid.get(currentRow, currentCol) == TileType::Wall && player.getIsDrawingPath()) {
        player.setIsDrawingPath(false);
        grid.fillEnclosedArea(getEnemyPositions());
    }

    for (auto& enemy : enemies) {
        enemy.update(dt, grid);
    }

    for (auto& e : smartEnemies) {
        e.update(dt, grid, player);
    }

    float elapsed = gameClock.getElapsedTime().asSeconds();
    hud.setTime(elapsed);
    hud.setScore(score);
    hud.setLives(lives);

    for (size_t i = 0; i < gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < gameObjects.size(); ++j) {
            if (gameObjects[i]->getBounds().intersects(gameObjects[j]->getBounds())) {
                gameObjects[i]->collideWith(*gameObjects[j]);
                gameObjects[j]->collideWith(*gameObjects[i]);
            }
        }
    }
    closedAreaPercent = grid.calculateClosedAreaPercent(); 
    hud.setAreaPercent(closedAreaPercent);

    
    if (closedAreaPercent >= 75.0f) {
        if (currentLevelNumber + 1 < levelLoader.getLevelCount()) {
            loadLevel(currentLevelNumber + 1);
        }
        else {
            window.close();
        }
    }


}

void Game::render() {
    window.clear();

    // 1. Draw grid
    grid.draw(window);

    // 2. Draw walls
    for (const auto& wall : walls) {
        wall.draw(window);
    }
       

    // 3. Draw enemies
    for (const auto& enemy : enemies) {
        enemy.draw(window);
    }
        

    // 4. Draw smart enemies
    for (auto& smartEnemy : smartEnemies) {
        smartEnemy.draw(window);
    }

    // 5. Draw player
    player.draw(window);

    // 6. Draw HUD
    hud.draw(window);

    window.display();
}


std::vector<sf::Vector2f> Game::getEnemyPositions() const {
    std::vector<sf::Vector2f> positions;
    for (const auto& e : enemies)
        positions.push_back(e.getPosition());

    for (const auto& e : smartEnemies)
        positions.push_back(e.getPosition());

    return positions;
}
