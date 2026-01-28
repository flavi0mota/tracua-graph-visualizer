#pragma once
#include "ISolver.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include "raymath.h"

using PQElement = std::pair<float, int>;

class BfsSolver : public ISolver {
private:
    std::deque<int> frontier;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parentMap;

    Graph* graph = nullptr;
    int startNodeId = -1;
    int endNodeId = -1;

public:
    void Initialize(Graph* graph, int start, int end) override;
    SolverState Step() override;
    std::vector<int> GetPath() const override;

    void DrawDebug(Font font) override {};
};

class DfsSolver : public ISolver {
private:
    std::vector<int> stack;

    std::unordered_set<int> visited;
    std::unordered_map<int, int> parentMap;

    Graph* graph = nullptr;
    int startNodeId = -1;
    int endNodeId = -1;

public:
    void Initialize(Graph* graph, int start, int end) override;
    SolverState Step() override;
    std::vector<int> GetPath() const override;

    void DrawDebug(Font font) override {}
};

class DijkstraSolver : public ISolver {
private:
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    std::unordered_map<int, float> dist;
    std::unordered_map<int, int> parentMap;

    Graph* graph = nullptr;
    int startNodeId = -1;
    int endNodeId = -1;

public:
    void Initialize(Graph* graph, int start, int end) override;
    SolverState Step() override;
    std::vector<int> GetPath() const override;

    void DrawDebug(Font font) override;
};

class AStarSolver : public ISolver {
private:
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    std::unordered_map<int, float> gScore;
    std::unordered_map<int, float> fScore;
    std::unordered_map<int, int> parentMap;

    Graph* graph = nullptr;
    int startNodeId = -1;
    int endNodeId = -1;

    float Heuristic(int nodeId);

public:
    void Initialize(Graph* graph, int start, int end) override;
    SolverState Step() override;
    std::vector<int> GetPath() const override;

    void DrawDebug(Font font) override;
};