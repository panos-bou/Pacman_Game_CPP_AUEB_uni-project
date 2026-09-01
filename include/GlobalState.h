#pragma once

#include <memory>
#include <vector>
#include <string>
#include "GameGraph.h"
#include "PacMan.h"
#include "Ghost.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER,
    WON
};

class GlobalState {
private:
    static GlobalState* instance;
    GameState currentState;
    int currentLevel;
    int maxLevels;

    std::unique_ptr<GameGraph> gameGraph;
    std::vector<std::shared_ptr<Character>> actors;
    std::shared_ptr<PacMan> pacman;

    float canvasWidth;
    float canvasHeight;
    float levelStartTime;
    float gameTime;

    GlobalState();

public:
    static GlobalState* getInstance();
    static void cleanup();

    void init();
    void initLevel(int level);

    void update(float dt);
    void draw() const;
    void handleInput();

    void pauseGame() { currentState = GameState::PAUSED; }
    void resumeGame() { currentState = GameState::PLAYING; }
    void resetGame();
    void nextLevel();

    GameGraph* getGraph() const { return gameGraph.get(); }
    PacMan* getPacMan() const { return pacman.get(); }
    const std::vector<std::shared_ptr<Character>>& getActors() const { return actors; }

    float getCanvasWidth() const { return canvasWidth; }
    float getCanvasHeight() const { return canvasHeight; }

    void checkCollisions();
    bool allPelletsEaten() const;
    void drawUI() const; // Η μέθοδος που θα σχεδιάσει τα κείμενα
};