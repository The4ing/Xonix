enum class CellState { Closed, Open, Trail };

class Board {
public:
    Board(int width, int height);
    CellState getCell(int x, int y) const;
    void setCell(int x, int y, CellState state);
    float calculateClosedAreaPercent() const;
    bool closeAreaIfPossible(const Player& playerTrail, const std::vector<Enemy>& enemies);

private:
    std::vector<std::vector<CellState>> grid;
    bool isInside(int x, int y) const;
    void floodFill(...);  // אלגוריתם לסגירת שטחים
};
