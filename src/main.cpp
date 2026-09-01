#include "sgg/graphics.h"
#include "GlobalState.h"
/*
ΕΡΓΑΣΙΑ CPP 2025-2026
από τον Παναγιώτη Μπουραζάνα με ΑΜ: 3200268
και τη Νεφέλη Πίτσου με ΑΜ: 3240253
*/

// Callback functions for SGG
void drawCallback() {
    GlobalState* state = GlobalState::getInstance();
    state->draw();
}

void updateCallback(float ms) {
    float dt = ms / 1000.0f;  // Convert to seconds
    GlobalState* state = GlobalState::getInstance();
    state->update(dt);
}

int main() {
    GlobalState* gameState = GlobalState::getInstance();

    //δημιουργούμε το παράθυρο για να αρχικοποιηθεί η SGG
    graphics::createWindow(900, 820, "Pac-Man with Graph Structure");

    //κάνουμε init (τώρα το graphics::getGlobalTime() θα δουλέψει)
    gameState->init();

    graphics::setDrawFunction(drawCallback);
    graphics::setUpdateFunction(updateCallback);
    graphics::setCanvasSize(gameState->getCanvasWidth(), gameState->getCanvasHeight());
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();

    GlobalState::cleanup();
    return 0;
}