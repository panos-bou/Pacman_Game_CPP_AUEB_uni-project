#include "Character.h"
#include "GameGraph.h"
#include <cmath>
#include <algorithm>

Character::Character(int startNodeId, float r, float spd, GameGraph* graph)
    : currentNodeId(startNodeId), targetNodeId(startNodeId), radius(r),
    speed(spd), gameGraph(graph), interpolationTime(0.0f),
    interpolationDuration(0.5f),
    startX(0.0f), startY(0.0f), endX(0.0f), endY(0.0f) { 

    auto node = gameGraph->getNode(startNodeId);
    if (node) {
        startX = endX = node->getX();
        startY = endY = node->getY();
    }
}

float Character::getX() const {
    if (interpolationDuration > 0) {
        float t = interpolationTime / interpolationDuration;
        t = std::max(0.0f, std::min(1.0f, t));
        return startX + (endX - startX) * t;
    }
    return endX;
}

float Character::getY() const {
    if (interpolationDuration > 0) {
        float t = interpolationTime / interpolationDuration;
        t = std::max(0.0f, std::min(1.0f, t));
        return startY + (endY - startY) * t;
    }
    return endY;
}

void Character::update(float dt) {
    interpolationTime += dt;
    if (interpolationTime >= interpolationDuration) {
        interpolationTime = interpolationDuration;
        int nextNode = chooseNextNode();
        if (nextNode != -1 && canMoveTo(nextNode)) {
            moveToNode(nextNode);
        }
    }
}

void Character::moveToNode(int nextNodeId) {
    auto nextNode = gameGraph->getNode(nextNodeId);
    if (nextNode) {
        currentNodeId = nextNodeId;
        targetNodeId = nextNodeId;
        startX = endX;
        startY = endY;
        endX = nextNode->getX();
        endY = nextNode->getY();

        float dx = endX - startX;
        float dy = endY - startY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // αν η απόσταση είναι τεράστια (> 400 pixels), είναι το τούνελ
        if (distance > 400.0f) {
            interpolationDuration = 0.001f; // ακαριαία μετακίνηση
        }
        else {
            interpolationDuration = distance / speed;
        }
        interpolationTime = 0.0f;
    }
}

bool Character::canMoveTo(int nodeId) const {
    auto node = gameGraph->getNode(currentNodeId);
    if (node) {
        const auto& neighbors = node->getNeighbors();
        return std::find(neighbors.begin(), neighbors.end(), nodeId) != neighbors.end();
    }
    return false;
}

bool Character::collidedWith(const Character& other) const {
    float dx = getX() - other.getX();
    float dy = getY() - other.getY();
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < (radius + other.radius);
}
