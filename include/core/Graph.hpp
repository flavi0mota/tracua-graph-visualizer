#pragma once
#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <optional>

constexpr float NODE_RADIUS = 20.0f;
constexpr Color COLOR_DEFAULT = DARKBLUE;
constexpr Color COLOR_HIGHLIGHT = RED;

struct Edge {
    int targetNodeId;
    float weight;
};

struct Node {
    int id;
    Vector2 position;
    std::string label;
    Color color;

    void ResetVisuals() {
        color = COLOR_DEFAULT;
    }
};

class Graph {
private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<Edge>> adjacencyList;

    int nextId = 0;

public:
    Graph() = default;

    int AddNode(Vector2 position);
    void AddEgde(int fromId, int toId, float weight = 1.0f, bool biDirection = true);

    void ResetGraphVisuals();
    void ClearAll();

    Node* GetNode(int id);

    const std::vector<Edge>& GetNeighbors(int id) const;
    std::vector<int> GetAllNodeIds() const;

    void Draw(Font font);

    int GetNodeAtPosition(Vector2 pos) const;
};
