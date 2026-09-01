#pragma once

#include <vector>
#include <cmath>

/**
 * Node.h - Κόμβος του γράφου για το Pac-Man game
 * Αντιπροσωπεύει μια επιτρεπόμενη θέση κίνησης
 */

class Node {
protected:
    int id;              // Μοναδικό ID του κόμβου
    float x, y;          // Θέση στο παιχνίδι (pixels)
    std::vector<int> neighbors;  // IDs γειτονικών κόμβων
    bool hasPellet;      // Αν υπάρχει pellet σε αυτόν τον κόμβο

public:
    Node(int id, float x, float y);
    virtual ~Node() = default;

    // Getters
    int getId() const { return id; }
    float getX() const { return x; }
    float getY() const { return y; }
    bool isPelletPresent() const { return hasPellet; }
    const std::vector<int>& getNeighbors() const { return neighbors; }

    // Setters
    void setX(float px) { x = px; }
    void setY(float py) { y = py; }
    void setPellet(bool has) { hasPellet = has; }

    // Graph operations
    void addNeighbor(int neighborId);
    bool hasNeighbor(int neighborId) const;
    double distanceTo(float px, float py) const;

    // Virtual methods για πολυμορφισμό
    virtual void draw() const = 0;
    virtual void update(float dt) {}
};
