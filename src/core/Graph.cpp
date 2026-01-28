#include "../../include/core/Graph.hpp" 
#include "raymath.h"
#include <string>

int Graph::AddNode(Vector2 position) {
    int id = nextId++;
    nodes[id] = { id, position, std::to_string(id), COLOR_DEFAULT };
    return id;
}

void Graph::AddEgde(int fromId, int toId, float weight, bool biDirection) {
    if (nodes.find(fromId) != nodes.end() && nodes.find(toId) != nodes.end()) {
        adjacencyList[fromId].push_back({ toId, weight });
        
        if (biDirection) {
            adjacencyList[toId].push_back({ fromId, weight });
        }
    }
}

void Graph::ResetGraphVisuals() {
    for (auto& [id, node] : nodes) {
        node.ResetVisuals();
    }
}

void Graph::ClearAll() {
    nodes.clear();
    adjacencyList.clear();
    nextId = 0;
}

Node* Graph::GetNode(int id) {
    if (nodes.find(id) != nodes.end()) {
        return &nodes[id];
    }
    return nullptr;
}

const std::vector<Edge>& Graph::GetNeighbors(int id) const {
    static const std::vector<Edge> empty;
    
    auto it = adjacencyList.find(id);
    if (it != adjacencyList.end()) {
        return it->second;
    }
    return empty;
}

std::vector<int> Graph::GetAllNodeIds() const {
    std::vector<int> ids;
    ids.reserve(nodes.size());
    for (const auto& [id, node] : nodes) {
        ids.push_back(id);
    }
    return ids;
}

void Graph::Draw(Font font) {
    for (const auto& [sourceId, sourceNode] : nodes) {
        if (adjacencyList.find(sourceId) != adjacencyList.end()) {
            for (const auto& edge : adjacencyList.at(sourceId)) {
                if (nodes.find(edge.targetNodeId) != nodes.end()) {
                    Vector2 targetPos = nodes.at(edge.targetNodeId).position;

                    DrawLineEx(sourceNode.position, targetPos, 2.0f, BLACK);

                    Vector2 mid = { 
                        (sourceNode.position.x + targetPos.x) / 2.0f,
                        (sourceNode.position.y + targetPos.y) / 2.0f 
                    };

                    char buff[16];
                    snprintf(buff, sizeof(buff), "%.1f", edge.weight);
                    DrawTextEx(font, buff, mid, 12, 1, DARKGRAY);
                }
            }
        }
    }

    for (const auto& [id, node] : nodes) {
        DrawCircleV(node.position, NODE_RADIUS, node.color);
        DrawCircleLines(node.position.x, node.position.y, NODE_RADIUS, BLACK);
        DrawTextEx(font, node.label.c_str(), 
                  { node.position.x - 5, node.position.y - 10 }, 
                  20, 1, WHITE);
    }
}

int Graph::GetNodeAtPosition(Vector2 pos) const {
    for (const auto& [id, node] : nodes) {
        if (CheckCollisionPointCircle(pos, node.position, NODE_RADIUS)) {
            return id;
        }
    }
    return -1;
}