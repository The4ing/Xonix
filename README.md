# Xonix
XONIX
program by: Or-Ram Atar(325004851) and Romi Sinizkey (322311457)

Xonix is a classic 2D arcade game where the player controls a marker that moves around a rectangular field. The goal is to claim territory by drawing lines and enclosing areas, avoiding enemies that roam inside the unclaimed space. Once an area is enclosed and no enemies are inside, it gets filled, reducing the open space. The game ends if the player is hit by an enemy or if an enemy touches an incomplete line.
there are 2 types of enemies regular enemies inside the unfilled area and smart enemies in the filed area the follows the player


the project contains the folowing files: 
Enemy.h, Game.h, GameObject.h, Hud.h, LevelGrid.h, LevelLoader.h, Player.h, smartEnemy.h, TileType.h, utilities.h, Wall.h, Enemy.cpp, Game.cpp, HUD.cpp, LevelGrid.cpp, LevelLoader.cpp, main.cpp, Player.cpp, smartEnemy.cpp, Wall.cpp

all the headers files contains the template of the class / namespace / struct 
in the CPP we have the implementation so we will elaborate more about the CPP files

main.CPP - we creating an object of game and calling the run function

Game: Manages the overall game lifecycle, including splash and game-over screens, level loading, event processing, entity updates (player, enemies, grid), rendering, and restart logic. 

Player: Handles user input to move the player tile, tracks and paints the player’s path on the grid, updates position within screen bounds, and renders the player shape. 

SmartEnemy: Implements a BFS-based pathfinding AI that recalculates a chase path every second, moves smoothly along that path toward the player, and draws the enemy. 

Wall: Represents static wall tiles placed around the level perimeter, encapsulating their size, color, position, and drawing logic. 

Enemy: A basic adversary that moves in a fixed/random direction, updates its position each frame, and reverses direction upon colliding with walls. 

HUD: Renders the heads-up display showing remaining lives, countdown timer, and area-capture progress with dynamic text and progress bar visuals. 

LevelGrid: Maintains the 2D tile matrix (Wall, Open, PlayerPath, Filled), handles drawing tiles, setting/getting tile types, filling enclosed areas excluding enemies, and collision checks. 

LevelLoader: Reads game settings (window size, initial lives) and level data (area percent, enemy positions) from a file, and initializes/reset the grid and enemy lists for each level. 

Main data structures we used: 
std::vector<std::vector<TileType>> grid;
representing each cell’s state (Open, Wall, PlayerPath, Filled)

std::vector<Enemy> enemies; and std::vector<SmartEnemy> smartEnemies; hold all adversaries in the level. They’re populated by LevelLoader::loadLevel(...) from the per-level std::vector<LevelData> levels; 

A unified std::vector<GameObject*> gameObjects; collects pointers to player, walls, and enemies for bulk update/collision handling. 
Pathfinding & flood-fill helpers:

BFS queue: std::queue<sf::Vector2i> for exploring connected tiles in both LevelGrid::fillEnclosedArea(...) and SmartEnemy::findPath(...).

Visited sets/maps: std::unordered_set<sf::Vector2i> (and std::unordered_map<sf::Vector2i, sf::Vector2i> cameFrom in the smart enemy) track seen tiles and reconstruct paths.

Enemy path buffer:
std::queue<sf::Vector2i> path; inside SmartEnemy stores the sequence of grid‐coords to follow on its next moves.

special algorithms - 
Flood-Fill for Capturing Enclosed Areas: Converts any contiguous region of “Open” tiles that is fully surrounded (doesn’t touch the border) into “Filled” tiles, unless an enemy occupies that region. It also converts the player’s trail (PlayerPath) into Filled when the area is closed.

it works by Iterate over every unvisited Open tile.
Run a BFS (queue-based) from that tile, marking visited and collecting all reachable Open tiles into a set, while tracking if the search ever reaches the map’s border.
If border was not reached, that set is an enclosed area—then check for any enemy positions in it; if none, mark all those tiles Filled.
Finally, sweep the grid to turn all PlayerPath tiles into Filled as part of the capture.

BFS Pathfinding for SmartEnemy: Every second, the smart enemy computes a shortest-path toward the player on the tile grid (ignoring walls/Filled tiles) and then steps along that route smoothly.
it works by Converting the enemy’s and player’s world positions into grid coordinates.
Initialize a queue with the enemy’s tile, a visited set, and a cameFrom map.
Dequeue and explore four‐directional neighbors; for each valid, non‐wall, non‐filled, unvisited neighbor, enqueue it and record its predecessor in cameFrom.
Once the player’s tile is dequeued (or queue exhausted), backtrack via cameFrom to build the path vector, reverse it, and push into a std::queue<sf::Vector2i> path. During update, the enemy moves toward the front of this path until exhausted, then waits for the next recalculation.

Wall-Collision Reflection for Basic Enemy: Moves in a constant velocity vector; when that move would collide with a wall tile, reverses the corresponding component of its direction (bounce effect).
it works by computeing a candidate next position by adding direction * tileSize.
Convert both current and next positions to grid indices.
If the next row at current column is collidable, invert the y component; if next column at current row collides, invert the x component of direction.
Continue moving along the (potentially reflected) vector each frame.

design - 
main: Entry point; creates the Game instance and calls run().

Game: Core orchestrator; loads levels, owns the grid, player, enemies and HUD, runs the update/draw loop.

LevelLoader: Parses level files to set up the grid size, initial lives and enemy spawn data.

LevelGrid: 2D tile map; answers collision queries and runs flood-fill to claim enclosed areas.

Player: Handles keyboard input, moves within bounds and stamps its path onto the grid.

Enemy: Simple “bounce” AI; moves in a direction and reflects off walls.

SmartEnemy: Every second runs BFS on the grid to compute and follow the shortest path to the player.

Wall: Static boundary tile; stores position/size and participates in collision checks.

HUD: Reads game state each frame and renders lives, timer and capture percentage.
