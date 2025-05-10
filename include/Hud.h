class HUD {
public:
    void update(int level, int lives, float closedPercent);
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text statusText;
};
