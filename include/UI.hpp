//
// Created by Lenovo on 06.12.2025.
//
#ifndef THREEPPGP_UI_HPP
#define THREEPPGP_UI_HPP

#include <string>

class MC;
class Physics;
class GameUI {
public:
    GameUI(const std::string& vehicleName, float baseMaxSpeed);

    void render(MC& mc, Physics& physics);

private:
    std::string vehicleName_;
    float baseMaxSpeed_;
};

#endif // THREEPPGP_UI_HPP
