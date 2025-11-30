//
// Created by Lenovo on 20.11.2025.
//

#include "../include/UI.hpp"
#include "../include/UI.hpp"
#include "MC.hpp"
#include <threepp/extras/imgui/ImguiContext.hpp>
#include <algorithm>

GameUI::GameUI(const std::string& vehicleName, float baseMaxSpeed)
    : vehicleName_(vehicleName), baseMaxSpeed_(baseMaxSpeed) {}

void GameUI::render(const MC& mc) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("Vehicle & Stats");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Separator();
    ImGui::Text("Current: %s", vehicleName_.c_str());
    
    // Speed readout and bar
    const float curSpd = mc.getCurrentSpeed();
    const float maxSpd = mc.getMaxSpeed();
    ImGui::Text("Speed: %.2f / %.2f", curSpd, maxSpd);
    float speedRatio = (maxSpd > 0.0f) ? std::min(std::abs(curSpd) / maxSpd, 1.0f) : 0.0f;
    ImGui::ProgressBar(speedRatio, ImVec2(200, 0), "");

    // Boost indicator
    bool boostActive = maxSpd > baseMaxSpeed_ * 1.05f;
    if (boostActive) {
        ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "BOOST ACTIVE");
    } else {
        ImGui::TextDisabled("Boost: inactive");
    }

    ImGui::Separator();
    ImGui::TextDisabled("Controls: WASD or arrows to drive");
    ImGui::End();
}