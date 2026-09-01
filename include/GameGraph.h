#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class GameGraph {
private:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::pair<int, int>> edges;  // Ζεύγη IDs κόμβων

public:
    GameGraph();
    ~GameGraph() = default;

    // Node management
    int addNode(float x, float y);
    // νεα addNode
    void addNode(std::shared_ptr<Node> node);
    std::shared_ptr<Node> getNode(int id);
    const std::shared_ptr<Node> getNode(int id) const;

    // Edge management
    void addEdge(int nodeA, int nodeB);
    bool hasEdge(int nodeA, int nodeB) const;
    std::vector<int> getNeighbors(int nodeId) const;

    // Graph queries
    int getNodeCount() const { return static_cast<int>(nodes.size()); }
    std::shared_ptr<Node> findClosestNode(float x, float y);
    std::vector<int> bfs(int start, int target);

    // Visualization
    void draw() const;
    void update(float dt);
};
