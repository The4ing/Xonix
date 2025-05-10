class Level {
public:
    bool loadFromFile(const std::string& filename, int levelNumber);
    const Board& getBoard() const;
    void updateBoard(const Player& player, const std::vector<Enemy>& enemies);

private:
    Board board;
    int requiredAreaPercent;
    int enemyCount;
    std::vector<sf::Vector2f> initialEnemyPositions;
};
