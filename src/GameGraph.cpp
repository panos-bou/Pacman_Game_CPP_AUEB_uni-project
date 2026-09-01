#include "GameGraph.h"
#include "sgg/graphics.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <GameNode.h>

GameGraph::GameGraph() {}

int GameGraph::addNode(float x, float y) {
    int id = static_cast<int>(nodes.size());
    nodes.push_back(std::make_shared<GameNode>(id, x, y));
    return id;
}

std::shared_ptr<Node> GameGraph::getNode(int id) {
    if (id >= 0 && id < static_cast<int>(nodes.size())) {
        return nodes[id];
    }
    return nullptr;
}

const std::shared_ptr<Node> GameGraph::getNode(int id) const {
    if (id >= 0 && id < static_cast<int>(nodes.size())) {
        return nodes[id];
    }
    return nullptr;
}

void GameGraph::addEdge(int nodeA, int nodeB) {
    if (nodeA >= 0 && nodeA < static_cast<int>(nodes.size()) &&
        nodeB >= 0 && nodeB < static_cast<int>(nodes.size())) {
        
        // δώσε άκρη (διάδρομο μόνο μια φορά)
        if (!hasEdge(nodeA, nodeB)) {
            nodes[nodeA]->addNeighbor(nodeB);
            nodes[nodeB]->addNeighbor(nodeA);
            edges.push_back({nodeA, nodeB});
        }
    }
}

bool GameGraph::hasEdge(int nodeA, int nodeB) const {
    return std::any_of(edges.begin(), edges.end(),
        [nodeA, nodeB](const std::pair<int, int>& e) {
            return (e.first == nodeA && e.second == nodeB) ||
                   (e.first == nodeB && e.second == nodeA);
        });
}

std::vector<int> GameGraph::getNeighbors(int nodeId) const {
    auto node = getNode(nodeId);
    if (node) {
        return node->getNeighbors();
    }
    return {};
}

std::shared_ptr<Node> GameGraph::findClosestNode(float x, float y) {
    std::shared_ptr<Node> closest;
    double minDist = 1e9;

    for (auto& node : nodes) {
        double dist = node->distanceTo(x, y);
        if (dist < minDist) {
            minDist = dist;
            closest = node;
        }
    }
    return closest;
}

std::vector<int> GameGraph::bfs(int start, int target) {
    if (start < 0 || start >= static_cast<int>(nodes.size()) ||
        target < 0 || target >= static_cast<int>(nodes.size())) {
        return {};
    }

    std::vector<int> parent(nodes.size(), -1);
    std::queue<int> q;
    q.push(start);
	parent[start] = -2;  // visited marker 

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        if (cur == target) break;

        auto neighbors = getNeighbors(cur);
        for (int next : neighbors) {
            if (parent[next] == -1) {
                parent[next] = cur;
                q.push(next);
            }
        }
    }

    // ανακατασκευή path
    std::vector<int> path;
    for (int at = target; at != -2; at = parent[at]) {
        if (at == -1) return {};  // δεν υπάρχει path
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void GameGraph::draw() const {
    graphics::Brush br;

    // ζωγράφισμα ακμών
    br.outline_color[0] = 0.2f; br.outline_color[1] = 0.2f; br.outline_color[2] = 0.5f;
    br.outline_width = 1.0f;
    for (const auto& edge : edges) {
        auto nodeA = getNode(edge.first);
        auto nodeB = getNode(edge.second);
        if (nodeA && nodeB) {
            graphics::drawLine(nodeA->getX(), nodeA->getY(),
                              nodeB->getX(), nodeB->getY(), br);
        }
    }

	// ζωγράφισμα κόμβων
    for (const auto& node : nodes) {
        node->draw();
    }
}

void GameGraph::update(float dt) {
    for (auto& node : nodes) {
        node->update(dt);
    }
}

void GameGraph::addNode(std::shared_ptr<Node> node) {
    nodes.push_back(node);
}
