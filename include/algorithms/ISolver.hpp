#pragma once
#include "../core/Graph.hpp"
#include <vector>

enum class SolverState {
    NOT_STARTED,
    RUNNING,
    FINISHED_FOUND_PATH,
    FINISHED_NO_PATH
};

class ISolver {
public:
    virtual ~ISolver() = default;

    virtual void Initialize(Graph* graph, int startNodeID, int endNodeId) = 0;
    virtual SolverState Step() = 0;

    virtual std::vector<int> GetPath() const = 0;

    virtual void DrawDebug(Font font) = 0;
};
