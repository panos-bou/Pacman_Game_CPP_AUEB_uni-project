#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class GameGraph;  

/**
 * Character.h - κλάση για κινούμενα αντικείμενα
 */

class Character {
protected:
    int currentNodeId;           // ID του κόμβου όπου βρίσκεται
    int targetNodeId;            // ID του κόμβου προορισμού
    float radius;                // ακτίνα για σχεδίαση και collision detection
    float speed;                 // pixels/sec
    GameGraph* gameGraph;        // pointer στη γράφο

    // Interpolation μεταξύ κόμβων
    float interpolationTime;
    float interpolationDuration;
    float startX, startY;
    float endX, endY;

public:
    Character(int startNodeId, float r, float spd, GameGraph* graph);
    virtual ~Character() = default;
    virtual bool canMoveTo(int nodeId) const;

    // Getters
    int getCurrentNodeId() const { return currentNodeId; }
    int getTargetNodeId() const { return targetNodeId; }
    float getRadius() const { return radius; }
    float getX() const;
    float getY() const;

    // Movement
    virtual void update(float dt);
    void moveToNode(int nextNodeId);
    virtual int chooseNextNode() = 0;  
  

    // Collision detection
    bool collidedWith(const Character& other) const;

    // Visualization
    virtual void draw() const = 0;
};
