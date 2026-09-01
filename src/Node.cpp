#include "Node.h"
#include <algorithm>

Node::Node(int id, float x, float y)
    : id(id), x(x), y(y), hasPellet(true) {}

void Node::addNeighbor(int neighborId) {
    if (!hasNeighbor(neighborId)) {
        neighbors.push_back(neighborId);
    }
}

bool Node::hasNeighbor(int neighborId) const {
    return std::find(neighbors.begin(), neighbors.end(), neighborId) != neighbors.end();
}

double Node::distanceTo(float px, float py) const {
    float dx = x - px;
    float dy = y - py;
    return std::sqrt(dx * dx + dy * dy);
}
