#pragma once
#include "GameNode.h"
#include "sgg/graphics.h"
class PowerNode : public GameNode {
public:
    PowerNode(int id, float x, float y) : GameNode(id, x, y, 12.0f) {} // μεγαλύτερη ακτίνα

    void draw() const override {
        GameNode::draw(); // σχεδιάζει τη βάση
        if (isPelletPresent()) {
            graphics::Brush br;
            // εφέ που αναβοσβήνει 
            br.fill_opacity = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 200.0f);
            br.fill_color[0] = 1.0f; br.fill_color[1] = 0.8f; br.fill_color[2] = 0.0f;
            graphics::drawDisk(x, y, 10.0f, br);
        }
    }
};