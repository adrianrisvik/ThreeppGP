//
// Created by Lenovo on 20.11.2025.
//

#ifndef THREEPPGP_UI_HPP
#define THREEPPGP_UI_HPP

#ifndef THREEPPGP_GAMEUI_HPP
#define THREEPPGP_GAMEUI_HPP

#include <string>

// Forward declarations
class MC;

class GameUI {
public:
    GameUI(const std::string& vehicleName, float baseMaxSpeed);

    void render(const MC& mc);

private:
    std::string vehicleName_;
    float baseMaxSpeed_;
};

#endif // THREEPPGP_GAMEUI_HPP


#endif //THREEPPGP_UI_HPP