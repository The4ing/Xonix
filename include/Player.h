class Player {
public:
    Player(const sf::Vector2f& startPos);
    void handleInput();
    void update(sf::Time dt, const Board& board);
    void reset();
    const std::vector<sf::Vector2i>& getTrail() const;

private:
    sf::Vector2f position;
    std::vector<sf::Vector2i> trail;
    Direction cלurrentDirection;
    bool isInTrailMode;
};
