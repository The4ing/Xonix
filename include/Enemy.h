class Enemy {
public:
    Enemy(const sf::Vector2f& startPos);
    void update(sf::Time dt, const Board& board);
    bool touchesTrail(const Board& board) const;
    sf::Vector2f getPosition() const;

private:
    sf::Vector2f position;
    sf::Vector2f direction; 
};
