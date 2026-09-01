#pragma once

#include <vector>
#include <memory>
#include "Node.h"


class GameNode : public Node {
private:
    float radius;  // Ακτίνα για σχεδίαση

public:
    GameNode(int id, float x, float y, float r = 8.0f);
    
    void draw() const override;
    void update(float dt) override;

    float getRadius() const { return radius; }
};
