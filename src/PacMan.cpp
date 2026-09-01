#include "PacMan.h"
#include "PowerNode.h" 
#include "GameGraph.h"
#include "GlobalState.h"
#include "sgg/graphics.h"
#include "sgg/scancodes.h"
#include <cstdlib>
#include <ctime>
#include <string>


PacMan::PacMan(int startNodeId, float r, float spd, GameGraph* graph)
    : Character(startNodeId, r, spd, graph),
    lives(3), score(0), pelletsCollected(0), totalPellets(0),
    animationTimer(0.0f), animationFrame(0),
    currentDirection(-1), nextDirection(-1) { 
}



int PacMan::chooseNextNode() {
    if (!gameGraph) return currentNodeId;
    auto currentNode = gameGraph->getNode(currentNodeId);
    if (!currentNode) return currentNodeId;

    float x = currentNode->getX();
    float y = currentNode->getY();

    // Λογική για nextDirection (επιθυμητή στροφή)
    if (nextDirection != -1) {
        for (int neighborId : currentNode->getNeighbors()) {
            auto neighbor = gameGraph->getNode(neighborId);
            if (!neighbor) continue;

            float dx = neighbor->getX() - x;
            float dy = neighbor->getY() - y;
            bool isWrap = std::abs(dx) > 400.0f; // Ανίχνευση τούνελ

            if ((nextDirection == graphics::SCANCODE_LEFT && (dx < -10.0f || (isWrap && dx > 0))) ||
                (nextDirection == graphics::SCANCODE_RIGHT && (dx > 10.0f || (isWrap && dx < 0))) ||
                (nextDirection == graphics::SCANCODE_UP && dy < -10.0f) ||
                (nextDirection == graphics::SCANCODE_DOWN && dy > 10.0f)) {
                currentDirection = nextDirection;
                return neighborId;
            }
        }
    }

    // Λογική για currentDirection (συνέχεια πορείας)
    if (currentDirection != -1) {
        for (int neighborId : currentNode->getNeighbors()) {
            auto neighbor = gameGraph->getNode(neighborId);
            if (!neighbor) continue;

            float dx = neighbor->getX() - x;
            float dy = neighbor->getY() - y;
            bool isWrap = std::abs(dx) > 400.0f;

            if ((currentDirection == graphics::SCANCODE_LEFT && (dx < -10.0f || (isWrap && dx > 0))) ||
                (currentDirection == graphics::SCANCODE_RIGHT && (dx > 10.0f || (isWrap && dx < 0))) ||
                (currentDirection == graphics::SCANCODE_UP && dy < -10.0f) ||
                (currentDirection == graphics::SCANCODE_DOWN && dy > 10.0f)) {
                return neighborId;
            }
        }
    }

    return currentNodeId;
}


void PacMan::draw() const {
    graphics::Brush br;
    br.outline_opacity = 0.0f;

    std::string dir = "right";
    if (currentDirection == graphics::SCANCODE_LEFT) dir = "left";
    else if (currentDirection == graphics::SCANCODE_UP) dir = "up";
    else if (currentDirection == graphics::SCANCODE_DOWN) dir = "down";

    // Σύνθεση ονόματος: π.χ. assets/pacman_up_1.png
    br.texture = "assets/pacman_" + dir + "_" + std::to_string(animationFrame) + ".png";

    graphics::drawRect(getX(), getY(), radius * 2.5f, radius * 2.5f, br);
}

void PacMan::update(float dt) {
    Character::update(dt); // Εκτελεί την κίνηση και το interpolation

    // Animation Logic
    animationTimer += dt;
    if (animationTimer >= ANIMATION_SPEED) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 3; // Κύκλος 0, 1, 2
    }

    // Παίρνουμε τον κόμβο στον οποίο κατευθυνόμαστε (destination node)
    auto node = gameGraph->getNode(currentNodeId);

    if (node && node->isPelletPresent()) {
        // Υπολογισμός απόστασης του Pac-Man από το κέντρο του κόμβου
        float dx = getX() - node->getX();
        float dy = getY() - node->getY();
        float distSq = dx * dx + dy * dy; // Απόσταση στο τετράγωνο

        // Αν η απόσταση είναι μικρότερη από 5 pixels (25 στο τετράγωνο), τότε τρώει το pellet
        if (distSq < 25.0f) {
            eatPellet();
        }
    }
}



void PacMan::eatPellet() {
    auto node = gameGraph->getNode(currentNodeId);
    if (node && node->isPelletPresent()) {
        // Έλεγχος αν ο κόμβος είναι PowerNode
        bool wasPower = (std::dynamic_pointer_cast<PowerNode>(node) != nullptr);
        graphics::playSound("assets/pacman_chomp.wav", 0.4f, false);
        node->setPellet(false);
        pelletsCollected++;
        score += wasPower ? 50 : 10;

        if (wasPower) {
            graphics::playSound("assets/pacman_eatfruit.wav", 0.4f, false);
            // Κάνε όλα τα φαντάσματα FRIGHTENED
            auto gs = GlobalState::getInstance();
            for (auto& actor : gs->getActors()) {
                auto ghost = std::dynamic_pointer_cast<Ghost>(actor);
                if (ghost) {
                    ghost->setBehavior(GhostBehavior::FRIGHTENED, 7.0f); // 7 δευτερόλεπτα
                }
            }
        }
    }
}

void PacMan::loseLife() {
    lives--;
    graphics::playSound("assets/pacman_death.wav", 0.6f, false);
    if (lives > 0) {
        // Επιστροφή στον κόμβο 93
        auto startNode = gameGraph->getNode(93);
        if (startNode) {
            currentNodeId = 93;
            targetNodeId = 93;
            startX = endX = startNode->getX();
            startY = endY = startNode->getY();
            interpolationTime = interpolationDuration;
        }
    }
}

void PacMan::reset() {
    auto startNode = gameGraph->getNode(0);
    if (startNode) {
        currentNodeId = 0;
        targetNodeId = 0;
        startX = endX = startNode->getX();
        startY = endY = startNode->getY();
        interpolationTime = 0.0f;
        nextDirection = -1;
        currentDirection = -1;
    }
}

void PacMan::eatGhost() {
    score += 200; // Πόντοι για κάθε φάντασμα
    graphics::playSound("assets/pacman_eatghost.wav", 0.6f, false);
}
