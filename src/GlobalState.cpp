#include "GlobalState.h"
#include "sgg/graphics.h"
#include <cmath>
#include <algorithm>
#include <string>
#include "PowerNode.h"
#include "GameNode.h"
#include "PacMan.h"

GlobalState* GlobalState::instance = nullptr;

GlobalState::GlobalState()
    : currentState(GameState::PLAYING), currentLevel(1), maxLevels(1),
    canvasWidth(900.0f), canvasHeight(820.0f),
    levelStartTime(0.0f), gameTime(0.0f) {
}

GlobalState* GlobalState::getInstance() {
    if (!instance) instance = new GlobalState();
    return instance;
}

void GlobalState::cleanup() {
    if (instance) { delete instance; instance = nullptr; }
}

void GlobalState::init() { 
    graphics::setFont("assets/font.ttf");
    initLevel(1); }

void GlobalState::initLevel(int level) {
    currentLevel = level;
    gameGraph = std::make_unique<GameGraph>();
    float startX = 50.0f, startY = 50.0f, spacing = 80.0f;
    // Intro Μουσική (παίζει μία φορά στην αρχή)
    graphics::playMusic("assets/pacman_beginning.wav", 0.5f, false);


    // Δημιουργία Κόμβων με PowerNodes στις γωνίες
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 11; ++col) {
            float nx = startX + col * spacing;
            float ny = startY + row * spacing;
            int id = row * 11 + col;

            // Αν είναι γωνία, βάλε PowerNode
            if ((row == 0 && col == 0) || (row == 0 && col == 10) ||
                (row == 9 && col == 0) || (row == 9 && col == 10)) {
                gameGraph->addNode(std::make_shared<PowerNode>(id, nx, ny));
            }
            else {
                gameGraph->addNode(std::make_shared<GameNode>(id, nx, ny));
            }
        }
    }

    // Δημιουργία Λαβυρίνθου 
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 11; ++col) {
            int cur = row * 11 + col;

            // Οριζόντιες συνδέσεις
            if (col < 10) {
                bool isWall = (row == 1 && (col == 1 || col == 3 || col == 6 || col == 8)) ||
                    (row == 3 && (col == 2 || col == 7)) ||
                    (row == 5 && (col == 2 || col == 7));

                //εξαίρεση: Μέσα στο Ghost House (Row 4-5, Col 4-5) επιτρέπουμε κίνηση
                if ((row == 4 || row == 5) && (col == 4)) isWall = false;

                if (!isWall) gameGraph->addEdge(cur, cur + 1);
            }

            // Κάθετες συνδέσεις
            if (row < 9) {
                bool isWall = (col == 2 && (row == 1 || row == 4 || row == 7)) ||
                    (col == 8 && (row == 1 || row == 4 || row == 7)) ||
                    (col == 5 && (row == 1 || row == 6));

                // επιτρέπουμε τη σύνδεση από το σπίτι (Row 4) προς τα πάνω (Row 3)
                if (col == 5 && row == 3) isWall = false;

                if (!isWall) gameGraph->addEdge(cur, cur + 11);
            }
        }
    }

    // προσθήκη tunnel στη σειρά 4 (Row 4, Col 0 <-> Col 10)
    gameGraph->addEdge(44, 54);


    // αφαίρεση Pellet από το σημείο spawn του Pac-Man (Κόμβος 93)
    auto spawnNode = gameGraph->getNode(93);
    if (spawnNode) {
        spawnNode->setPellet(false);
    }

    // καταμέτρηση των Pellets που όντως υπάρχουν στον χάρτη
    int totalCount = 0;
    for (int i = 0; i < gameGraph->getNodeCount(); ++i) {
        auto node = gameGraph->getNode(i);
        // Μετράμε μόνο αν ο κόμβος έχει pellet ΚΑΙ είναι προσβάσιμος (έχει γείτονες)
        if (node && node->isPelletPresent() && !node->getNeighbors().empty()) {
            totalCount++;
        }
    }


    // Spawn Pac-Man και χαρακτήρες
    actors.clear();
    pacman = std::make_shared<PacMan>(93, 20.0f, 150.0f, gameGraph.get());
    actors.push_back(pacman);

    // ΤΟΠΟΘΕΤΗΣΗ ΦΑΝΤΑΣΜΑΤΩΝ ΣΤΟ ΚΕΝΤΡΟ 
    actors.push_back(std::make_shared<Ghost>(0, 48, 20.0f, 120.0f, gameGraph.get()));
    actors.push_back(std::make_shared<Ghost>(1, 49, 20.0f, 120.0f, gameGraph.get()));
    actors.push_back(std::make_shared<Ghost>(2, 59, 20.0f, 120.0f, gameGraph.get()));
    actors.push_back(std::make_shared<Ghost>(3, 60, 20.0f, 120.0f, gameGraph.get()));

    pacman->setTotalPellets(totalCount); // Ενημέρωση με τον σωστό αριθμό
    levelStartTime = graphics::getGlobalTime();

}

void GlobalState::update(float dt) {
    
    handleInput();
    if (currentState != GameState::PLAYING || !pacman) return;

    gameTime += dt;
    levelStartTime += dt;

    // ενημέρωση όλων των actors (Pacman & Φαντάσματα)
    for (auto& actor : actors) {
        if (actor) actor->update(dt);
    }

    checkCollisions();

    if (allPelletsEaten()) {
        graphics::stopMusic();
        graphics::playSound("assets/pacman_intermission.wav", 0.5f, false);
        if (currentLevel < maxLevels) nextLevel();
        else currentState = GameState::WON;
    }
}

void GlobalState::draw() const {
    if (!gameGraph || !pacman) return;
    graphics::Brush br;
    br.outline_opacity = 0.0f;
    br.fill_color[0] = 0.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
    graphics::drawRect(canvasWidth / 2, canvasHeight / 2, canvasWidth, canvasHeight, br);
    gameGraph->draw();
    // σχεδίαση Χαρακτήρων (Pacman & Ghosts)
    for (const auto& actor : actors) {
        if (actor) actor->draw();
    }

    drawUI();
}

void GlobalState::checkCollisions() {
    if (!pacman) return;

    for (auto& actor : actors) {
        if (actor != pacman) { // έλεγχος μόνο με τα φαντάσματα
            auto ghost = std::dynamic_pointer_cast<Ghost>(actor);
            if (ghost && pacman->collidedWith(*ghost)) {

                // αν το φάντασμα είναι μπλε (FRIGHTENED)
                if (ghost->getBehavior() == GhostBehavior::FRIGHTENED) {
                    pacman->eatGhost(); // Πάρε πόντους
                    ghost->setBehavior(GhostBehavior::EATEN, 0.0f); // Γίνε "μάτια"
                }
                // αν το φάντασμα είναι κανονικό (και όχι ήδη φαγωμένο)
                else if (ghost->getBehavior() != GhostBehavior::EATEN) {
                    pacman->loseLife();
                    if (pacman->isGameOver()) currentState = GameState::GAME_OVER;
                    return;
                }
            }
        }
    }
}

void GlobalState::handleInput() {
    if (!pacman) return;

    // έλεγχος για Επανεκκίνηση 
    if (graphics::getKeyState(graphics::SCANCODE_R)) {
        resetGame();
        return; // Επιστρέφουμε αμέσως για να ξεκινήσει το νέο επίπεδο καθαρό
    }

    // έλεγχος κίνησης Pac-Man
    if (graphics::getKeyState(graphics::SCANCODE_LEFT)) pacman->setNextDirection(graphics::SCANCODE_LEFT);
    else if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) pacman->setNextDirection(graphics::SCANCODE_RIGHT);
    else if (graphics::getKeyState(graphics::SCANCODE_UP)) pacman->setNextDirection(graphics::SCANCODE_UP);
    else if (graphics::getKeyState(graphics::SCANCODE_DOWN)) pacman->setNextDirection(graphics::SCANCODE_DOWN);

    // έλεγχος για Pause 
    static bool spacePressed = false;
    if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
        if (!spacePressed) {
            spacePressed = true;
            if (currentState == GameState::PLAYING) currentState = GameState::PAUSED;
            else if (currentState == GameState::PAUSED) currentState = GameState::PLAYING;
        }
    }
    else spacePressed = false;
}



void GlobalState::resetGame() { 
    currentState = GameState::PLAYING;
    initLevel(currentLevel);
}
void GlobalState::nextLevel() { currentLevel++; initLevel(currentLevel); }
bool GlobalState::allPelletsEaten() const { return pacman && pacman->getPelletsCollected() >= pacman->getTotalPellets(); }

void GlobalState::drawUI() const {
    if (!pacman) return;

    graphics::Brush br;
    br.fill_opacity = 1.0f; 

    // ρύθμιση για το HUD 
    br.fill_color[0] = 1.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f;

    // εμφάνιση Score 
    std::string scoreStr = "SCORE: " + std::to_string(pacman->getScore());
    graphics::drawText(30, 30, 30, scoreStr, br);

    // εμφάνιση Lives 
    std::string livesStr = "LIVES: " + std::to_string(pacman->getLives());
    graphics::drawText(canvasWidth - 180, 30, 30, livesStr, br);



    // GAME OVER 
    if (currentState == GameState::GAME_OVER) {
        br.fill_color[0] = 1.0f; br.fill_color[1] = 0.0f; br.fill_color[2] = 0.0f;
        graphics::drawText(canvasWidth / 2 - 180, canvasHeight / 2, 70, "GAME OVER", br);
        graphics::drawText(canvasWidth / 2 - 160, canvasHeight / 2 + 60, 20, "PRESS 'R' TO RESTART", br);
    }

    // YOU WON!
    if (currentState == GameState::WON) {
        br.fill_color[0] = 0.0f; br.fill_color[1] = 1.0f; br.fill_color[2] = 0.0f; // Πράσινο
        graphics::drawText(canvasWidth / 2 - 150, canvasHeight / 2, 60, "YOU WON!", br);
        graphics::drawText(canvasWidth / 2 - 140, canvasHeight / 2 + 50, 20, "CONGRATULATIONS! PRESS 'R' TO RESTART", br);
    }

    // Μήνυμα PAUSE
    if (currentState == GameState::PAUSED) {
        br.fill_opacity = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 200.0f);
        graphics::drawText(canvasWidth / 2 - 80, canvasHeight / 2, 40, "PAUSED", br);
    }
}