#pragma once
#include "Character.h"
#include <vector>

enum class GhostBehavior {
    CHASE,
    SCATTER,
    FRIGHTENED,
    EATEN 
};

class Ghost : public Character {
private:
    int ghostId;
    int spawnNodeId; // Η βάση του φαντάσματος
    GhostBehavior behavior;
    float behaviorTimer;
    float behaviorDuration;

public:
    Ghost(int ghostId, int startNodeId, float r, float spd, GameGraph* graph);
    bool canMoveTo(int nodeId) const override;
    int chooseNextNode() override;
    void draw() const override;
    void update(float dt) override;

    void setBehavior(GhostBehavior b, float duration);
    GhostBehavior getBehavior() const { return behavior; }
    int getGhostId() const { return ghostId; }

private:
    int chasePacMan(int pacmanNodeId);
    int scatterBehavior();
    int randomMovement();
};