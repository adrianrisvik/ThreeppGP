
#ifndef THREEPPGP_UI_HPP
#define THREEPPGP_UI_HPP

#include <string>

// Forward declarations
class MC;
class Physics;

class GameUI {
public:
    GameUI(const std::string& vehicleName, float baseMaxSpeed);

    // Updated to include Physics reference

    void render(MC& mc, Physics& physics);

private:
    std::string vehicleName_;
    float baseMaxSpeed_;
};

#endif // THREEPPGP_UI_HPP
