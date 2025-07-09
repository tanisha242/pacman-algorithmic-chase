# PACMAN Algorithmic Chase

An interactive 2D PACMAN-inspired maze chase game built in C++ using the **SFML** graphics library. In this game, Pac-Man navigates through a procedurally generated maze collecting food pellets while being chased by four intelligent ghosts, each using a different algorithmic strategy to pursue the player in real-time.

---

## Game Concept

- The player controls **Pac-Man** using arrow keys in a 61x41 grid-based maze.
- The goal is to collect all food pellets (500 total) scattered across the maze.
- Four unique ghosts dynamically chase Pac-Man using different **pathfinding algorithms**.
- The game ends when Pac-Man is caught by any ghost.

---

## AI & Algorithms Used

Each ghost exhibits distinct AI behavior using the following search algorithms:

| Ghost  | Color     | Algorithm Used          | Behavior Description                           |
|--------|-----------|-------------------------|------------------------------------------------|
| Red    |   Red     | Dijkstra's Algorithm    | Pursues Pac-Man using shortest guaranteed path |
| Pink   |   Pink    | A* (A-Star)             | Predicts and intercepts Pac-Man's position     |
| Cyan   |   Cyan    | Flood Fill (BFS)        | Covers distance via breadth-first spreading    |
| Orange |   Orange  | Dijkstra's Algorithm    | Same as Red (for now)                          |

---

## Maze Generation

- Maze is generated using a **randomized depth-first search (DFS)** algorithm.
- Guarantees a connected, solvable layout with complex paths and loops.
- Layout changes every time the game is restarted.

---

## Game Components

- Maze: 61×41 grid where `'#'` = wall, `' '` = path
- Pac-Man: Player-controlled via arrow keys, moves 1 cell per frame
- Ghosts: Chasing AI agents
- Pellets: 500 randomly placed food items (score +1 each)

---

## Screenshot

![Gameplay](https://github.com/tanisha242/pacman-algorithmic-chase/blob/main/screenshots/game%20screenshot%201.png)
           (https://github.com/tanisha242/pacman-algorithmic-chase/blob/main/screenshots/game%20screenshot%202.png)

## Requirements

- C++ compiler (e.g. `g++`, `clang++`)
- [SFML 2.5+](https://www.sfml-dev.org/) installed and linked

### Install SFML

#### On Ubuntu/Debian
```bash
sudo apt-get install libsfml-dev
