#include "GameNode.h"
#include "sgg/graphics.h"

GameNode::GameNode(int id, float x, float y, float r)
    : Node(id, x, y), radius(r) {}

void GameNode::draw() const {
    graphics::Brush br;
    
    // Κύκλος για τον κόμβο
    br.fill_color[0] = 0.3f; br.fill_color[1] = 0.3f; br.fill_color[2] = 0.3f;
    graphics::drawDisk(x, y, radius, br);

    // Pellet αν υπάρχει
    if (hasPellet) {
        br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;
        graphics::drawDisk(x, y, radius * 0.4f, br);
    }
}

void GameNode::update(float dt) {
    // Κόμβοι δεν χρειάζονται update
}
