#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include <random>
#include <ctime>
using namespace sf;

const int CELL_SIZE = 40;
const int MAZE_WIDTH = 61;
const int MAZE_HEIGHT = 41;

struct Node {
    int x, y, cost;
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

vector<vector<char>> maze(MAZE_HEIGHT, vector<char>(MAZE_WIDTH,
'#'));

int dx[] = {0, 0, -2, 2};
int dy[] = {-2, 2, 0, 0};
int ddx[] = {0, 0, -1, 1};
int ddy[] = {-1, 1, 0, 0};


bool isValid(int x, int y) {
    return x > 0 && y > 0 && x < MAZE_WIDTH - 1 && y < MAZE_HEIGHT -
1;
}

void generateMaze(int startX, int startY) {
    stack<pair<int, int>> s;
    s.push({startX, startY});
    maze[startY][startX] = ' ';

    random_device rd;
    mt19937 g(rd());

    while (!s.empty()) {
        auto [x, y] = s.top();
        vector<int> dirs = {0, 1, 2, 3};
        shuffle(dirs.begin(), dirs.end(), g);

        bool moved = false;
        for (int dir : dirs) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (isValid(nx, ny) && maze[ny][nx] == '#') {
                maze[ny][nx] = ' ';
                maze[y + dy[dir] / 2][x + dx[dir] / 2] = ' ';
                s.push({nx, ny});
                moved = true;
                break;
            }
        }
        if (!moved) s.pop();
    }




    int extraPassages = (MAZE_WIDTH * MAZE_HEIGHT) / 20;
    for (int i = 0; i < extraPassages; ++i) {
        int x = g() % (MAZE_WIDTH - 2) + 1;
        int y = g() % (MAZE_HEIGHT - 2) + 1;
        if (maze[y][x] == '#') {
            int wallNeighbors = 0;
            for (int d = 0; d < 4; ++d) {
                int nx = x + ddx[d];
                int ny = y + ddy[d];
                if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny <
MAZE_HEIGHT) {
                    if (maze[ny][nx] == ' ') wallNeighbors++;
                }
            }
            if (wallNeighbors >= 2)
                maze[y][x] = ' ';
        }
    }
}

vector<pair<int, int>> foodPellets;

void generateFoodPellets(int numPellets) {
    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < numPellets; ++i) {
        int x = g() % (MAZE_WIDTH - 2) + 1;
        int y = g() % (MAZE_HEIGHT - 2) + 1;
        if (maze[y][x] == ' ') {
            foodPellets.push_back({x, y});
        }
    }
}


vector<pair<int, int>> floodFillPath(pair<int, int> start, pair<int, int> goal) {
    queue<pair<int, int>> q;
    map<pair<int, int>, pair<int, int>> cameFrom;
    q.push(start);
    cameFrom[start] = start;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (make_pair(x, y) == goal) break;

        for (int i = 0; i < 4; i++) {
            int nx = x + ddx[i];
            int ny = y + ddy[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny <
MAZE_HEIGHT && maze[ny][nx] == ' ' && !cameFrom.count({nx, ny})) {
                cameFrom[{nx, ny}] = {x, y};
                q.push({nx, ny});
            }
        }
    }

    vector<pair<int, int>> path;
    pair<int, int> curr = goal;
    if (!cameFrom.count(goal)) return path;
    while (curr != start) {
        path.push_back(curr);
        curr = cameFrom[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int, int>> aStar(pair<int, int> start, pair<int, int> goal) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    map<pair<int, int>, pair<int, int>> cameFrom;
    map<pair<int, int>, int> costSoFar;
    pq.push({start.first, start.second, 0});
    cameFrom[start] = start;
    costSoFar[start] = 0;

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop();
        pair<int, int> curr = {current.x, current.y};
        if (curr == goal) break;

        for (int i = 0; i < 4; i++) {
            int nx = curr.first + ddx[i], ny = curr.second + ddy[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny <
MAZE_HEIGHT && maze[ny][nx] == ' ') {
                int newCost = costSoFar[curr] + 1;
                pair<int, int> next = {nx, ny};
                if (!costSoFar.count(next) || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    int priority = newCost + abs(goal.first - nx) + abs(goal.second - ny);
                    pq.push({nx, ny, priority});
                    cameFrom[next] = curr;
                }
            }
        }
    }

    vector<pair<int, int>> path;
    pair<int, int> curr = goal;
    if (!cameFrom.count(goal)) return path;
    while (curr != start) {


        path.push_back(curr);
        curr = cameFrom[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int, int>> dijkstra(pair<int, int> start, pair<int, int> goal) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    map<pair<int, int>, pair<int, int>> cameFrom;
    map<pair<int, int>, int> costSoFar;
    pq.push({start.first, start.second, 0});
    cameFrom[start] = start;
    costSoFar[start] = 0;

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop();
        pair<int, int> curr = {current.x, current.y};
        if (curr == goal) break;

        for (int i = 0; i < 4; i++) {
            int nx = curr.first + ddx[i], ny = curr.second + ddy[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny <
MAZE_HEIGHT && maze[ny][nx] == ' ') {
                int newCost = costSoFar[curr] + 1;
                pair<int, int> next = {nx, ny};
                if (!costSoFar.count(next) || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    pq.push({nx, ny, newCost});
                    cameFrom[next] = curr;
                }
            }
        }
    }



    vector<pair<int, int>> path;
    pair<int, int> curr = goal;
    if (!cameFrom.count(goal)) return path;
    while (curr != start) {
        path.push_back(curr);
        curr = cameFrom[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}


int main() {
    RenderWindow window(VideoMode(MAZE_WIDTH * CELL_SIZE,
MAZE_HEIGHT * CELL_SIZE), "Pac-Man Ghost Squad");

    Texture wallTexture, redGhostTexture, pinkGhostTexture, cyanGhostTexture,
orangeGhostTexture;
    wallTexture.loadFromFile("wall.png");
    redGhostTexture.loadFromFile("red.png");
    pinkGhostTexture.loadFromFile("pink.png");
    cyanGhostTexture.loadFromFile("cyan.png");
    orangeGhostTexture.loadFromFile("orange.png");

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font\n";
        return -1;
    }

    int score = 0;  // Initialize score


// Create score text
Text scoreText;
scoreText.setFont(font);
scoreText.setCharacterSize(24);
scoreText.setFillColor(Color::White);


    generateMaze(1, 1);
    generateFoodPellets(500);

    pair<int, int> pacmanPos = {1, 1};
    pair<int, int> red = {MAZE_WIDTH - 2, MAZE_HEIGHT - 2};
    pair<int, int> pink = {1, MAZE_HEIGHT - 2};
    pair<int, int> cyan = {MAZE_WIDTH - 2, 1};
    pair<int, int> orange = {MAZE_WIDTH / 2, MAZE_HEIGHT / 2};

    Clock clock;
    float moveTimer = 0;
    const float moveDelay = 0.10f;
    float ghostMoveTimer = 0;
    const float ghostMoveDelay = 0.22f;

    bool gameOver = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        moveTimer += dt;
        ghostMoveTimer += dt;


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)


                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
{
                pacmanPos = {1, 1};
                red = {MAZE_WIDTH - 2, MAZE_HEIGHT - 2};
                pink = {1, MAZE_HEIGHT - 2};
                cyan = {MAZE_WIDTH - 2, 1};
                orange = {MAZE_WIDTH / 2, MAZE_HEIGHT / 2};
                generateMaze(1, 1);
                 generateFoodPellets(500);
                  score = 0;
                gameOver = false;
            }
        }

        if (!gameOver && moveTimer >= moveDelay) {
            if (Keyboard::isKeyPressed(Keyboard::Up) && maze[pacmanPos.second - 1][pacmanPos.first] == ' ')
                pacmanPos.second--;
            else if (Keyboard::isKeyPressed(Keyboard::Down) &&
maze[pacmanPos.second + 1][pacmanPos.first] == ' ')
                pacmanPos.second++;
            else if (Keyboard::isKeyPressed(Keyboard::Left) &&
maze[pacmanPos.second][pacmanPos.first - 1] == ' ')
                pacmanPos.first--;
            else if (Keyboard::isKeyPressed(Keyboard::Right) &&
maze[pacmanPos.second][pacmanPos.first + 1] == ' ')
                pacmanPos.first++;

            moveTimer = 0;
        }

          for (auto it = foodPellets.begin(); it != foodPellets.end(); ) {
        if (pacmanPos == *it) {  // Pac-Man eats the food


            it = foodPellets.erase(it);  // Remove the food pellet
            score += 1;  // Increase score by 10 for each pellet
        } else {
            ++it;
        }
    }

        if (!gameOver && ghostMoveTimer >= ghostMoveDelay) {


            auto redPath = dijkstra(red, pacmanPos);
            if (!redPath.empty()) red = redPath[0];

            pair<int, int> target = pacmanPos;
            if (Keyboard::isKeyPressed(Keyboard::Up)) target.second -= 2;
            if (Keyboard::isKeyPressed(Keyboard::Down)) target.second += 2;
            if (Keyboard::isKeyPressed(Keyboard::Left)) target.first -= 2;
            if (Keyboard::isKeyPressed(Keyboard::Right)) target.first += 2;

            auto pinkPath = aStar(pink, target);
            if (!pinkPath.empty()) pink = pinkPath[0];

            auto cyanPath = floodFillPath(cyan, pacmanPos);
            if (!cyanPath.empty()) cyan = cyanPath[0];

            auto orangePath = dijkstra(orange, pacmanPos);
            if (!orangePath.empty()) orange = orangePath[0];

             ghostMoveTimer = 0;
        }

         if (pacmanPos == red || pacmanPos == pink || pacmanPos == cyan ||
pacmanPos == orange)
                gameOver = true;


        window.clear(Color::Black);

        for (int y = 0; y < MAZE_HEIGHT; y++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                RectangleShape cell(Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                if (maze[y][x] == '#') {
                    cell.setFillColor(Color::Blue);
                    window.draw(cell);
                }
            }
        }

         for (const auto& pellet : foodPellets) {
            CircleShape food(CELL_SIZE / 4);  // Small food pellet
            food.setPosition(pellet.first * CELL_SIZE + CELL_SIZE / 2 -
food.getRadius(),
                             pellet.second * CELL_SIZE + CELL_SIZE / 2 -
food.getRadius());
            food.setFillColor(Color::White);  // White food pellets
            window.draw(food);
        }

        if (!gameOver) {
            CircleShape pacman(CELL_SIZE / 2);
            pacman.setPosition(pacmanPos.first * CELL_SIZE, pacmanPos.second *
CELL_SIZE);
            pacman.setFillColor(Color::Yellow);
            window.draw(pacman);
        }

        Sprite redGhost(redGhostTexture); redGhost.setPosition(red.first *
CELL_SIZE, red.second * CELL_SIZE); window.draw(redGhost);
        Sprite pinkGhost(pinkGhostTexture); pinkGhost.setPosition(pink.first *


CELL_SIZE, pink.second * CELL_SIZE); window.draw(pinkGhost);
        Sprite cyanGhost(cyanGhostTexture); cyanGhost.setPosition(cyan.first *
CELL_SIZE, cyan.second * CELL_SIZE); window.draw(cyanGhost);
        Sprite orangeGhost(orangeGhostTexture);
orangeGhost.setPosition(orange.first * CELL_SIZE, orange.second *
CELL_SIZE); window.draw(orangeGhost);

         scoreText.setString("Score: " + to_string(score));
    scoreText.setPosition(10.f, 10.f);  // Position score at the top-left
    window.draw(scoreText);

        if (gameOver) {
            Text gameOverText("Game Over!", font, 40);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setStyle(Text::Bold);
            gameOverText.setPosition(MAZE_WIDTH * CELL_SIZE / 2 - 120,
MAZE_HEIGHT * CELL_SIZE / 2 - 50);
            window.draw(gameOverText);

            Text restartText("Press R to Restart", font, 25);
            restartText.setFillColor(Color::White);
            restartText.setPosition(MAZE_WIDTH * CELL_SIZE / 2 - 130,
MAZE_HEIGHT * CELL_SIZE / 2 + 10);
            window.draw(restartText);
        }

        window.display();
    }

    return 0;
}

