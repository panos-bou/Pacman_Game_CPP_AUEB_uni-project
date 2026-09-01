#include "Ghost.h"
#include "GameGraph.h"
#include "GlobalState.h"
#include "sgg/graphics.h"
#include <cstdlib>
#include <ctime>

Ghost::Ghost(int id, int startNodeId, float r, float spd, GameGraph* graph)
    : Character(startNodeId, r, spd, graph), ghostId(id), spawnNodeId(startNodeId),
    behavior(GhostBehavior::SCATTER), behaviorTimer(0.0f), behaviorDuration(10.0f) {
}

bool Ghost::canMoveTo(int nodeId) const {
    // αν το φάντασμα δεν είναι μάτια (EATEN), τότε απαγόρευσέ του το τούνελ
    if (behavior != GhostBehavior::EATEN) {
        if ((currentNodeId == 44 && nodeId == 54) || (currentNodeId == 54 && nodeId == 44)) {
            return false;
        }
    }
    // για όλες τις άλλες περιπτώσεις (συμπεριλαμβανομένων των ματιών), χρησιμοποίησε τη βασική λογική
    return Character::canMoveTo(nodeId);
}

int Ghost::chooseNextNode() {
    if (!gameGraph) return currentNodeId;

    // κατάσταση EATEN (Μάτια)
    if (behavior == GhostBehavior::EATEN) {
        auto path = gameGraph->bfs(currentNodeId, spawnNodeId);
        if (path.size() > 1 && canMoveTo(path[1])) return path[1];
        return currentNodeId;
    }

    // κατάσταση SCATTER (Προς τις γωνίες)
    if (behavior == GhostBehavior::SCATTER) {
        return scatterBehavior();
    }

    // Κατάσταση CHASE (Καταδίωξη)
    if (behavior == GhostBehavior::CHASE) {
        int target = GlobalState::getInstance()->getPacMan()->getCurrentNodeId();
        auto path = gameGraph->bfs(currentNodeId, target);
        if (path.size() > 1 && canMoveTo(path[1])) return path[1];
    }

    return randomMovement();
}


int Ghost::randomMovement() {
    auto currentNode = gameGraph->getNode(currentNodeId);
    if (currentNode) {
        auto neighbors = currentNode->getNeighbors();
        std::vector<int> validNeighbors;
        for (int neighborId : neighbors) {
            if (canMoveTo(neighborId)) validNeighbors.push_back(neighborId);
        }
        if (!validNeighbors.empty()) return validNeighbors[rand() % validNeighbors.size()];
    }
    return currentNodeId;
}

void Ghost::draw() const {
    graphics::Brush br;
    br.outline_opacity = 0.0f; 

    std::string texturePath;

	//κατάσταση φαντάσματος
    switch (behavior) {
    case GhostBehavior::EATEN:
        
        texturePath = "assets/ghost_eyes.png";
        break;

    case GhostBehavior::FRIGHTENED:
        
        texturePath = "assets/ghost_scared.png";
        break;

    default:
        // κανονική Κατάσταση (CHASE ή SCATTER)
        switch (ghostId) {
        case 0: texturePath = "assets/ghost_red.png"; break;
        case 1: texturePath = "assets/ghost_pink.png"; break;
        case 2: texturePath = "assets/ghost_cyan.png"; break;
        case 3: texturePath = "assets/ghost_orange.png"; break;
        default: texturePath = "assets/ghost_red.png"; break; // Fallback
        }
        break;
    }

    
    br.texture = texturePath;

    graphics::drawRect(getX(), getY(), radius * 2.0f, radius * 2.0f, br);
}

void Ghost::update(float dt) {
    //όταν EATEN, τρέχει πιο γρήγορα στη βάση
    float originalSpeed = speed;
    if (behavior == GhostBehavior::EATEN) speed = 300.0f;

    Character::update(dt);
    speed = originalSpeed;

    if (behavior == GhostBehavior::EATEN) {
        if (currentNodeId == spawnNodeId) {
            setBehavior(GhostBehavior::CHASE, 10.0f); // μόλις φτάσει, ξαναζωντανεύει
        }
        return;
    }

    behaviorTimer += dt;
    if (behaviorTimer >= behaviorDuration) {
        setBehavior(behavior == GhostBehavior::CHASE ? GhostBehavior::SCATTER : GhostBehavior::CHASE, 10.0f);
    }
}

void Ghost::setBehavior(GhostBehavior b, float duration) {
    behavior = b;
    behaviorDuration = duration;
    behaviorTimer = 0.0f;
}



int Ghost::chasePacMan(int pacmanNodeId) {
    // BFS προς τον PacMan
    auto path = gameGraph->bfs(currentNodeId, pacmanNodeId);
    if (path.size() > 1) {
        return path[1];
    }
    return randomMovement();
}

int Ghost::scatterBehavior() {
    int targetNode = 0;
    // Ανάθεση γωνίας βάσει ID
    switch (ghostId) {
    case 0: targetNode = 0; break;
    case 1: targetNode = 10; break;
    case 2: targetNode = 99; break;
    case 3: targetNode = 109; break;
    default: targetNode = 0; break;
    }

    // Χρήση BFS για εύρεση διαδρομής προς τη γωνία
    auto path = gameGraph->bfs(currentNodeId, targetNode);

    // Επιστροφή του επόμενου κόμβου αν η διαδρομή είναι έγκυρη και επιτρέπεται η κίνηση
    if (path.size() > 1 && canMoveTo(path[1])) {
        return path[1];
    }

    return randomMovement(); // Fallback αν κολλήσει
}



