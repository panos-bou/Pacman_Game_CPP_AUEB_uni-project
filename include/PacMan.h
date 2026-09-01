#pragma once

#include "Character.h"
#include <queue>


class PacMan : public Character {
private:
    int score;
    int lives;
    int pelletsCollected;
    int totalPellets;

    // Input για την κίνηση
    int nextDirection;  // SCANCODE_* από SGG
    int currentDirection;

    float animationTimer = 0.0f;       // Χρονομέτρης για την εναλλαγή εικόνων
    int animationFrame =0;         // 0 = κλειστό στόμα, 1 = ανοιχτό στόμα
    const float ANIMATION_SPEED = 0.1f; // Πόσο γρήγορα ανοιγοκλείνει (σε δευτερόλεπτα)

public:
    PacMan(int startNodeId, float r, float spd, GameGraph* graph);

    // Inherited
    int chooseNextNode() override;
    void draw() const override;
    void update(float dt) override;

    // PacMan-specific
    void setNextDirection(int scancode) { nextDirection = scancode; }
    void eatPellet();

    void loseLife();
    void reset();
    void eatGhost();

    // Getters
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getPelletsCollected() const { return pelletsCollected; }
    int getTotalPellets() const { return totalPellets; }
    void setTotalPellets(int total) { totalPellets = total; }

    bool isGameOver() const { return lives <= 0; }
};
