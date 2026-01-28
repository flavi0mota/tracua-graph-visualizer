#include "raylib.h"
#include "raymath.h"
#include <memory>  
#include <string>
#include <iostream>

#include "../include/core/Graph.hpp"
#include "../include/algorithms/Algorithms.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const float STEP_DELAY = 0.1f;

enum class AppState {
    EDITOR,
    RUNNING,
    PAUSED,
    FINISHED
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph Visualizer - C++ & Raylib");
    SetTargetFPS(60);

    Graph graph;
    
    std::unique_ptr<ISolver> solver = nullptr;
    
    AppState currentState = AppState::EDITOR;
    int startNodeId = -1;
    int endNodeId = -1;
    
    int dragSourceId = -1;
    Vector2 currentMousePos = {0, 0};

    float stepTimer = 0.0f;

    solver = std::make_unique<BfsSolver>();
    std::string currentAlgoName = "BFS";

    while (!WindowShouldClose()) {
        currentMousePos = GetMousePosition();
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_R)) {
            graph.ClearAll();
            currentState = AppState::EDITOR;
            startNodeId = -1; endNodeId = -1;
            graph.ResetGraphVisuals();
        }

        if (IsKeyPressed(KEY_C)) {
            graph.ResetGraphVisuals();
            currentState = AppState::EDITOR;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (currentState == AppState::EDITOR) {
                if (startNodeId != -1 && endNodeId != -1) {
                    graph.ResetGraphVisuals();

                    currentState = AppState::RUNNING;
                    solver->Initialize(&graph, startNodeId, endNodeId);
                }
            } else if (currentState == AppState::RUNNING) {
                currentState = AppState::PAUSED;
            } else if (currentState == AppState::PAUSED) {
                currentState = AppState::RUNNING;
            }
        }

        if (currentState == AppState::EDITOR || currentState == AppState::FINISHED) {
            if (IsKeyPressed(KEY_ONE))   { solver = std::make_unique<BfsSolver>(); currentAlgoName = "BFS"; }
            if (IsKeyPressed(KEY_TWO))   { solver = std::make_unique<DfsSolver>(); currentAlgoName = "DFS"; }
            if (IsKeyPressed(KEY_THREE)) { solver = std::make_unique<DijkstraSolver>(); currentAlgoName = "Dijkstra"; }
            if (IsKeyPressed(KEY_FOUR))  { solver = std::make_unique<AStarSolver>(); currentAlgoName = "A*"; }
        }

        if (currentState == AppState::EDITOR) {
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int hoveredNode = graph.GetNodeAtPosition(currentMousePos);
                
                if (hoveredNode == -1) {
                    graph.AddNode(currentMousePos);
                } else {
                    if (startNodeId == -1) startNodeId = hoveredNode;
                    else if (endNodeId == -1 && hoveredNode != startNodeId) endNodeId = hoveredNode;
                    else {
                        startNodeId = hoveredNode;
                        endNodeId = -1;
                    }
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                dragSourceId = graph.GetNodeAtPosition(currentMousePos);
            }
            
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                if (dragSourceId != -1) {
                    int dragTargetId = graph.GetNodeAtPosition(currentMousePos);
                    if (dragTargetId != -1 && dragTargetId != dragSourceId) {
                        Node* a = graph.GetNode(dragSourceId);
                        Node* b = graph.GetNode(dragTargetId);
                        float weight = Vector2Distance(a->position, b->position);
                        
                        graph.AddEgde(dragSourceId, dragTargetId, weight);
                    }
                }
                dragSourceId = -1;
            }
        }

        if (currentState == AppState::RUNNING) {
            stepTimer += dt;
            if (stepTimer >= STEP_DELAY) {
                stepTimer = 0.0f;
                
                SolverState result = solver->Step();
                
                if (result == SolverState::FINISHED_FOUND_PATH) {
                    currentState = AppState::FINISHED;
                    
                    std::vector<int> path = solver->GetPath();
                    for (int nodeId : path) {
                        if (Node* n = graph.GetNode(nodeId)) {
                            n->color = GREEN; 
                        }
                    }
                    
                } else if (result == SolverState::FINISHED_NO_PATH) {
                    currentState = AppState::FINISHED;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        graph.Draw(GetFontDefault());

        if (startNodeId != -1) {
            Node* n = graph.GetNode(startNodeId);
            DrawCircleLines(n->position.x, n->position.y, NODE_RADIUS + 5, GREEN);
            DrawText("START", n->position.x - 20, n->position.y - 40, 10, GREEN);
        }
        if (endNodeId != -1) {
            Node* n = graph.GetNode(endNodeId);
            DrawCircleLines(n->position.x, n->position.y, NODE_RADIUS + 5, RED);
            DrawText("END", n->position.x - 15, n->position.y - 40, 10, RED);
        }

        if (dragSourceId != -1) {
            Node* n = graph.GetNode(dragSourceId);
            DrawLineEx(n->position, currentMousePos, 2.0f, GRAY);
        }

        if (currentState != AppState::EDITOR) {
            solver->DrawDebug(GetFontDefault());
        }

        DrawRectangle(0, 0, SCREEN_WIDTH, 50, Fade(LIGHTGRAY, 0.8f));
        DrawText(TextFormat("Mode: %s | Algo: %s", 
            (currentState == AppState::EDITOR ? "EDITOR" : "RUNNING"), 
            currentAlgoName.c_str()), 10, 15, 20, DARKGRAY);
        
        DrawText("L-Click: Adicionar nó | R-Drag: Vértice | Space: Executar | R: Reset | 1-4: Trocar algoritmo", 
            350, 18, 10, DARKGRAY);

        EndDrawing();
    }

    // 4. CLEANUP
    CloseWindow();
    return 0;
}