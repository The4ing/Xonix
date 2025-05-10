class Game {
public:
    Game();
    void run();

private:
    void loadLevel();
    void processEvents();
    void update(sf::Time dt);
    void render();

    Level currentLevel;
    Player player;
    std::vector<Enemy> enemies;
    HUD hud;

    int currentLevelNumber;
    int lives;
    float closedAreaPercent;

    sf::RenderWindow window;
};
