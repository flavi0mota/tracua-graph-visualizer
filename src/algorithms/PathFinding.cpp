#include "../../include/algorithms/Algorithms.hpp"
#include <algorithm>
#include <cstdio>

// BFS IMPLEMENTATIONS

void BfsSolver::Initialize(Graph* g, int start, int end) {
    this->graph = g;
    this->startNodeId = start;
    this->endNodeId = end;

    frontier.clear();
    visited.clear();
    parentMap.clear();

    frontier.push_back(start);
    visited.insert(start);

    if (Node* n = graph->GetNode(start)) {
        n->color = ORANGE;
    }
}

SolverState BfsSolver::Step() {
    if (frontier.empty()) {
        return SolverState::FINISHED_NO_PATH;
    }

    int currentId = frontier.front();
    frontier.pop_front();

    Node* currentNode = graph->GetNode(currentId);

    if (currentId == endNodeId) {
        return SolverState::FINISHED_FOUND_PATH;
    }

    if (currentId != startNodeId) {
        currentNode->color = RED;
    }

    const std::vector<Edge>& neighbors = graph->GetNeighbors(currentId);

    for (const Edge& edge : neighbors) {
        int neighborId = edge.targetNodeId;

        if (visited.find(neighborId) == visited.end()) {
            visited.insert(neighborId);
            parentMap[neighborId] = currentId;
            frontier.push_back(neighborId);

            Node* neighborNode = graph->GetNode(neighborId);
            if (neighborId != endNodeId) {
                neighborNode->color = YELLOW;
            }
        }
    }

    return SolverState::RUNNING;
}

std::vector<int> BfsSolver::GetPath() const {
    std::vector<int> path;

    int current = endNodeId;

    while (current != startNodeId && parentMap.count(current)) {
        path.push_back(current);
        current = parentMap.at(current);
    }

    path.push_back(startNodeId);

    std::reverse(path.begin(), path.end());

    return path;
}


// END OF BFS

// START OF DFS

void DfsSolver::Initialize(Graph* g, int start, int end) {
    this->graph = g;
    this->startNodeId = start;
    this->endNodeId = end;

    stack.clear();
    visited.clear();
    parentMap.clear();

    stack.push_back(start);
    visited.insert(start);

    if (Node* n = graph->GetNode(start)) {
        n->color = ORANGE;
    }
}

SolverState DfsSolver::Step() {
    if (stack.empty()) {
        return SolverState::FINISHED_NO_PATH;
    }

    int currentId = stack.back();
    stack.pop_back();

    Node* currentNode = graph->GetNode(currentId);

    if (currentId == endNodeId) {
        return SolverState::FINISHED_FOUND_PATH;
    }

    if (currentId != startNodeId) {
        currentNode->color = RED;
    }

    const std::vector<Edge>& neighbors = graph->GetNeighbors(currentId);

    for (const Edge& edge : neighbors) {
        int neighbordId = edge.targetNodeId;

        if (visited.find(neighbordId) == visited.end()) {
            visited.insert(neighbordId);
            parentMap[neighbordId] = currentId;
            stack.push_back(neighbordId);

            Node* neighbordNode = graph->GetNode(neighbordId);

            if (neighbordId != endNodeId) {
                neighbordNode->color = YELLOW;
            }
        }
    }

    return SolverState::RUNNING;
}

std::vector<int> DfsSolver::GetPath() const {
    std::vector<int> path;
    int curr = endNodeId;
    while (curr != startNodeId && parentMap.count(curr)) {
        path.push_back(curr);
        curr = parentMap.at(curr);
    }
    path.push_back(startNodeId);
    std::reverse(path.begin(), path.end());
    return path;
}

// END OF DFS

//START OF DIJKSTRA

void DijkstraSolver::Initialize(Graph* g, int start, int end) {
    this->graph = g;
    this->startNodeId = start;
    this->endNodeId = end;

    pq = {};
    dist.clear();
    parentMap.clear();

    dist[start] = 0.0f;
    pq.push({0.0f, start});

    if (Node* n = graph->GetNode(start)) {
        n->color = ORANGE;
    }
}

SolverState DijkstraSolver::Step() {
    if (pq.empty()) {
        return SolverState::FINISHED_NO_PATH;
    }

    PQElement top = pq.top();
    pq.pop();

    float currentDist = top.first;
    int currentId = top.second;

    if (currentDist > dist[currentId]) {
        return SolverState::RUNNING;
    }

    Node* currentNode = graph->GetNode(currentId);

    if (currentId == endNodeId) {
        return SolverState::FINISHED_FOUND_PATH;
    }

    if (currentId != startNodeId) {
        currentNode->color = RED;
    }

    const std::vector<Edge>& neighbors = graph->GetNeighbors(currentId);

    for (const Edge& edge : neighbors) {
        int neighborId = edge.targetNodeId;
        float weight = edge.weight;

        float newDist = currentDist + weight;

        bool isFirstDiscovery = (dist.find(neighborId) == dist.end());
        bool isBetterPath = isFirstDiscovery || (newDist < dist[neighborId]);

        if (isBetterPath) {
            dist[neighborId] = newDist;
            parentMap[neighborId] = currentId;
            pq.push({newDist, neighborId});

            Node* neighborNode = graph->GetNode(neighborId);

            if (neighborId != endNodeId) {
                neighborNode->color = YELLOW;
            }
        }
    }

    return SolverState::RUNNING;
}

std::vector<int> DijkstraSolver::GetPath() const {
    std::vector<int> path;
    int curr = endNodeId;
    while (curr != startNodeId && parentMap.count(curr)) {
        path.push_back(curr);
        curr = parentMap.at(curr);
    }
    path.push_back(startNodeId);
    std::reverse(path.begin(), path.end());
    return path;
}

void DijkstraSolver::DrawDebug(Font font) {
    for (auto const& [id, d] : dist) {
        Node* n = graph->GetNode(id);   
        if (!n) continue;

        char buffer[16];

        std::snprintf(buffer, sizeof(buffer), ".1f", d);

        Vector2 textPos = { n->position.x - 10, n->position.y - 35};
        DrawTextEx(font, buffer, textPos, 20, 1, BLACK);
    }
}

// END OF DIJKSTRA

// START OF A*

float AStarSolver::Heuristic(int nodeId) {
    if (!graph) return 0.0f;

    Node* a = graph->GetNode(nodeId);
    Node* b = graph->GetNode(endNodeId);

    if (!a || !b) return 0.0f;

    return Vector2Distance(a->position, b->position);
}

void AStarSolver::Initialize(Graph* g, int start, int end) {
    graph = g;
    startNodeId = start;
    endNodeId = end;

    pq = {};
    gScore.clear();
    fScore.clear();
    parentMap.clear();

    gScore[start] = 0.0f;
    fScore[start] = Heuristic(start);

    pq.push({fScore[start], start});

    if (Node* n = graph->GetNode(start)) {
        n->color = ORANGE;
    }
}

SolverState AStarSolver::Step() {
    if (pq.empty()) {
        return SolverState::FINISHED_NO_PATH;
    }

    PQElement top = pq.top();
    pq.pop();

    float currentF = top.first;
    int currentId = top.second;

    if (currentF > fScore[currentId]) {
        return SolverState::RUNNING;
    }

    if (currentId == endNodeId) {
        return SolverState::FINISHED_FOUND_PATH;
    }

    Node* currentNode = graph->GetNode(currentId);
    if (currentId != startNodeId) {
        currentNode->color = RED;
    }

    const std::vector<Edge>& neighbors = graph->GetNeighbors(currentId);

    for (const Edge& edge : neighbors) {
        int neighborId = edge.targetNodeId;
        float weight = edge.weight;

        float tentativeG = gScore[currentId] + weight;

        bool isBetter = false;
        if (gScore.find(neighborId) == gScore.end()) {
            isBetter = true;
        } else if (tentativeG < gScore[neighborId]) {
            isBetter = true;
        }

        if (isBetter) {
            parentMap[neighborId] = currentId;
            gScore[neighborId] = tentativeG;

            float h = Heuristic(neighborId);
            float f = tentativeG + h;
            fScore[neighborId] = f;

            pq.push({f, neighborId});

            Node* neighborNode = graph->GetNode(neighborId);
            if (neighborId != endNodeId) {
                neighborNode->color = YELLOW;
            }
        }
    }

    return SolverState::RUNNING;
}

std::vector<int> AStarSolver::GetPath() const {
    std::vector<int> path;
    int curr = endNodeId;

    // Backtrack from End -> Start using the parentMap
    // Safety check: parentMap.count(curr) prevents infinite loops if path is broken
    while (curr != startNodeId && parentMap.count(curr)) {
        path.push_back(curr);
        curr = parentMap.at(curr);
    }

    path.push_back(startNodeId);
    
    // Reverse the vector to get Start -> End order
    std::reverse(path.begin(), path.end());
    
    return path;
}

void AStarSolver::DrawDebug(Font font) {
    for (auto const& [id, g] : gScore) {
        Node* n = graph->GetNode(id);
        if (!n) continue;

        float h = Heuristic(id);
        float f = fScore[id];

        char textG[10], textH[10], textF[10];
        std::snprintf(textG, 10, "g:%.0f", g);
        std::snprintf(textG, 10, "h:%.0f", h);
        std::snprintf(textG, 10, "F:%.0f", f);

        Vector2 pos = n->position;
        DrawTextEx(font, textF, {pos.x - 5, pos.y - 5}, 20, 1, BLACK);
        DrawTextEx(font, textG, {pos.x - 25, pos.y - 25}, 10, 1, DARKGRAY);
        DrawTextEx(font, textH, {pos.x + 10, pos.y - 25}, 10, 1, DARKGRAY);
    }
}

// END OF AStar
