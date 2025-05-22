#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include <stdexcept>
#include <iostream>
#include "utilities.h"


Game::Game()
    : levelLoader("resources/levels.txt"),  
    font(),
    hud(sf::Vector2f(0, 960), sf::Vector2f(1920, 120), font),
    currentLevelNumber(0),
    requiredAreaPercent(0.0f),
    closedAreaPercent(0.0f),
    lives(1),
    player(0, 29, 32.0f),
    grid(30, 60, 32.0f)
{
    remainingTime = 120.f;  
    gameClock.restart(); 

    if (!font.loadFromFile("resources/game.otf")) {
        throw std::runtime_error("Failed to load font.");
    }
    if (!lostFont.loadFromFile("resources/PressStart2P.ttf")) {
        throw std::runtime_error("Failed to load LostFont.ttf");
    }
}

void Game::loadFirstLevel() {
    loadLevel(FIRST_LEVEL);
}

void Game::loadLevel(int levelNumber) {
    levelLoader.load();

    currentLevelNumber = levelNumber;

   
    enemies.clear();
    smartEnemies.clear();
    walls.clear();
    gameObjects.clear();


    settings = levelLoader.getSettings(); 
    levelDataList = levelLoader.getLevelData();
    window.create(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Xonix Game");
    lives = settings.initialLives;
    requiredAreaPercent = levelDataList[levelNumber].initialAreaPercent;

    levelLoader.loadLevel(currentLevelNumber, grid, enemies, smartEnemies);


    player.setWindowSize(sf::Vector2f(window.getSize()));
    player.setGrid(&grid);

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

    remainingTime = 120.f;
    gameClock.restart();
    closedAreaPercent = 0.f;
    hud.setAreaPercent(0.f);

}

void Game::showSplashScreen() {
    sf::RenderWindow splash(sf::VideoMode(800, 600), "Welcome");
    const auto winSize = splash.getSize();

    // Title
    sf::Text title("XONIX", font, 160);
    title.setFillColor(sf::Color::White);
    {
        auto bounds = title.getLocalBounds();
        title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        title.setPosition(winSize.x / 2.f, winSize.y / 2.f - 80.f);
    }

    // Prompt
    sf::Text prompt("Press any key to start", font, 67);
    prompt.setFillColor(sf::Color::White);
    {
        auto bounds = prompt.getLocalBounds();
        prompt.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        prompt.setPosition(winSize.x / 2.f, winSize.y / 2.f + 40.f);
    }

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
    // עדכון הזמן הנותר
    remainingTime -= dt.asSeconds();
    if (remainingTime < 0.f) {
        remainingTime = 0.f;
        restart();
        return;
    }

    // קריאת קלט ועדכון השחקן
    player.handleInput();
    sf::Vector2i dir = player.getDirection();
    player.update(dt);

    sf::Vector2f pPos = player.getPosition();
    float tileSize = grid.getTileSize();
    int row = static_cast<int>(pPos.y / tileSize);
    int col = static_cast<int>(pPos.x / tileSize);
    float fracY = (pPos.y / tileSize) - row;
    float fracX = (pPos.x / tileSize) - col;

    int nextRow = row;
    int nextCol = col;
    if (dir.x > 0 && fracX >= 0.5f)      nextCol = col + 1;
    else if (dir.x < 0 && fracX <= 0.5f) nextCol = col - 1;
    if (dir.y > 0 && fracY >= 0.5f)      nextRow = row + 1;
    else if (dir.y < 0 && fracY <= 0.5f) nextRow = row - 1;

    nextRow = std::clamp(nextRow, 0, grid.getRows() - 1);
    nextCol = std::clamp(nextCol, 0, grid.getCols() - 1);

    TileType nextTile = grid.get(nextRow, nextCol);

    if ((nextTile == TileType::Wall || nextTile == TileType::Filled)
        && player.getIsDrawingPath()) {
        player.setIsDrawingPath(false);
        grid.fillEnclosedArea(getEnemyPositions());

        updateClosedAreaPercent();

        // בדיקת מעבר שלב
        if (closedAreaPercent >= requiredAreaPercent) {
            std::cout << "Level complete!" << std::endl;
            currentLevelNumber++;

            if (currentLevelNumber >= levelDataList.size()) {
                std::cout << "All levels completed. You win!" << std::endl;
                window.close(); 
            }
            else {
                loadLevel(currentLevelNumber);
            }
        }
    }

    // עדכון אויבים רגילים
    for (auto& enemy : enemies) {
        sf::Vector2f pos = enemy.getPosition();
        int row = static_cast<int>(pos.y / grid.getTileSize());
        int col = static_cast<int>(pos.x / grid.getTileSize());

        if (grid.get(row, col) == TileType::PlayerPath) {
            loseLife();
            // הסר את הנתיב של השחקן
            for (int y = 0; y < grid.getRows(); ++y) {
                for (int x = 0; x < grid.getCols(); ++x) {
                    if (grid.get(y, x) == TileType::PlayerPath) {
                        grid.set(y, x, TileType::Open);
                    }
                }
            }
            // אפס את השחקן
            player.resetToStart();

            // אפס את השומרים החכמים בלבד
            for (auto& smart : smartEnemies) {
                smart.resetToStart();
            }

            break; // מספיק שומר אחד שנגע
        }
        enemy.update(dt, grid);
    }

    // עדכון אויבים חכמים
    for (auto& smart : smartEnemies) {
        smart.update(dt, grid, player);

        if (smart.getBounds().intersects(player.getBounds())) {
            loseLife();                    
            player.resetToStart();        
            for (auto& s : smartEnemies)  
                s.resetToStart();
            break;                        
        }
    }

    hud.setTime(remainingTime);
    hud.setLives(lives);
    hud.setAreaProgress(closedAreaPercent, requiredAreaPercent);
}

void Game::render() {
    window.clear();
    grid.draw(window);

    for (const auto& wall : walls) {
        wall.draw(window);
    }
       
    for (const auto& enemy : enemies) {
        enemy.draw(window);
    }

    for (auto& smartEnemy : smartEnemies) {
        smartEnemy.draw(window);
    }

    player.draw(window);
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

void Game::updateClosedAreaPercent() {
    int openCount = 0;
    int filledCount = 0;
    int rows = grid.getRows();
    int cols = grid.getCols();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            TileType t = grid.get(y, x);
            if (t == TileType::Open)   ++openCount;
            else if (t == TileType::Filled) ++filledCount;
        }
    }

    int totalOriginalOpen = openCount + filledCount;
    if (totalOriginalOpen > 0) {
        closedAreaPercent = 100.f * filledCount / totalOriginalOpen;
    }
    else {
        closedAreaPercent = 0.f;
    }
}

void Game::loseLife() {
    lives--;
    hud.setLives(lives);
    if (lives <= 0) {
        showGameOverScreen();
        restart();
    }
}
void Game::restart() {
    lives = settings.initialLives;
    hud.setLives(lives);

    // reset the countdown
    remainingTime = 120.f;
    gameClock.restart();
    closedAreaPercent = 0.f;
    hud.setAreaPercent(0.f);

    hud.draw(window);

    window.display();
    loadFirstLevel();
}

void Game::showGameOverScreen() {
    gameOverText.setFont(lostFont);
    gameOverText.setString("       YOU LOST!\nPress any key to restart");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    // center it
    sf::FloatRect bb = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bb.width / 2, bb.height / 2);
    gameOverText.setPosition(window.getSize().x / 2.f,
        window.getSize().y / 2.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                return window.close();
            if (event.type == sf::Event::KeyPressed)
                return;  // any key to continue
        }
        window.clear(sf::Color::Black);
        window.draw(gameOverText);
        window.display();
    }
}
