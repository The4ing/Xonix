// Game.cpp
#include "Game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Xonix Game"),
    currentLevelNumber(0),
    lives(3),
    closedAreaPercent(0.0f)
{

}


void Game::run() {
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

}

void Game::render() {
    window.clear(sf::Color::Black);

    window.display();
}
