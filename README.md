# Graph Visualizer (C++ & Raylib)

A real-time, interactive visualization engine for graph pathfinding algorithms.

Built from scratch using **C++17** and **Raylib**.

## Features

* **Interactive Graph Editor:** Point-and-click interface to draw nodes and create edges.
* **Weights:** Edge weights are calculated based on distance for visual and logical consistency.
* **Algorithm Suite:**
* **BFS (Breadth-First Search):** Unweighted shortest path.
* **DFS (Depth-First Search):** Topological exploration (non-optimal).
* **Dijkstra:** Weighted shortest path using a Min-Heap.
* **A* (A-Star):** Heuristic-based search.


* **Visuals:** Real-time rendering of the "Frontier" (Yellow), "Processed" (Red), and path costs (, ,  scores).

## 🛠️ Prerequisites

* **C++ Compiler:** GCC, Clang, or MSVC (C++17 standard).
* **CMake:** Version 3.10 or higher.
* **Raylib:** The graphics library.
* *Linux (Debian/Ubuntu):* `sudo apt install libraylib-dev`



## Build & Execution

This project uses CMake.

```bash
cd GraphVisualizer

mkdir build
cd build

cmake ..

make

./GraphViz

```

## 🎮 Controls

| Key / Mouse | Action | Context |
| --- | --- | --- |
| **Left Click (Empty)** | Create a new Node | Editor Mode |
| **Left Click (Node)** | Select **Start** (Green) or **End** (Red) | Editor Mode |
| **Right Drag** | Create an Edge between two nodes | Editor Mode |
| **Spacebar** | **Run** / **Pause** the algorithm | All Modes |
| **R** | **Hard Reset** (Clear Graph & Colors) | All Modes |
| **C** | **Soft Reset** (Clear Colors/Path only) | All Modes |
| **1** | **BFS** | Editor / Finished |
| **2** | **DFS** | Editor / Finished |
| **3** | **Dijkstra** | Editor / Finished |
| **4** | **A*** | Editor / Finished |

## Inner Workings & Architecture

Unlike standard algorithms that run to completion in a single `while` loop, and thus blocking the UI, this visualizer uses a **State Machine** pattern.

### 1. The Core Pattern: `ISolver::Step()`

Standard algorithms are transformed into classes that maintain their own memory (stack, queue, visited set) between frames.

* **Usual approach:**
```cpp
while(!queue.empty()) { ... }

```


* **My approach:**
```cpp
SolverState Step() {
    if (queue.empty()) return FINISHED;
    return RUNNING;
}

```

The Main Loop calls `Step()` once every `STEP_DELAY` seconds, allowing the Renderer to draw the intermediate states (Yellow/Red colors) to the screen.

### 2. Separation of Concerns (MVC)

The codebase is strictly layered to prevent spaghetti code:

* **Model (`src/core/`)**:
* `Graph`: A data container. It holds `std::unordered_map` of Nodes and Edges. It knows nothing about pathfinding.


* **Controller (`src/algorithms/`)**:
* `ISolver`: An abstract interface defining the contract (`Initialize`, `Step`, `GetPath`).
* Concrete Solvers (`BfsSolver`, `AStarSolver`, etc.) manipulate the Model's colors based on their specific logic.


* **View (`src/ui/`)**:
* `Renderer`: Handles all Raylib draw calls. It reads the Model and paints circles/lines.
* `Input`: Maps raw mouse coordinates to Node IDs.

### 3. Key Data Structures

* **Adjacency List:** Used for graph topology (`std::unordered_map<int, std::vector<Edge>>`). This allows  node access and efficient neighbor iteration.
* **Priority Queue:** Used by Dijkstra and A* to order nodes by cost.
* **Parent Map:** A `std::map<int, int>` that tracks the "breadcrumb trail." When the target is found, we backtrack through this map to reconstruct the final green path.

## 📁 Directory Structure

```text
GraphVisualizer/
├── CMakeLists.txt       
├── src/
│   ├── main.cpp         # State Machine
│   ├── core/            # Graph & Node data structures
│   ├── algorithms/      # BFS, DFS, Dijkstra, A*
│   └── ui/              # Raylib rendering & Input handling (TO-DO)
└── include/             # Header files

```