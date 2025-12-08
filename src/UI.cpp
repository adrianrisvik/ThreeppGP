//
// Created by Lenovo on 06.12.2025.
//
#include "../include/UI.hpp"
#include "MC.hpp"
#include "Physics.hpp"
#include <threepp/extras/imgui/ImguiContext.hpp>
#include <algorithm>

// Implemented with help from AI
GameUI::GameUI(const std::string& vehicleName, float baseMaxSpeed)
    : vehicleName_(vehicleName), baseMaxSpeed_(baseMaxSpeed) {}


void GameUI::render(MC& mc, Physics& physics) {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::Begin("Vehicle & Stats");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Separator();
    ImGui::Text("Current: %s", vehicleName_.c_str());

    // Speed
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
    ImGui::TextDisabled("Controls: WASD to drive");

    // Physics Controls
    ImGui::Separator();
    ImGui::Text("Physics Settings");

    static float gravity = 10.0f;
    static float mass = 300.0f;
    static float friction = 0.8f;
    static float dampingLinear = 0.1f;
    static float dampingAngular = 0.1f;
    static int substeps = 5; // Default substeps

    ImGui::SliderFloat("Gravity", &gravity, -3000.0f, 500.0f); // Negative allowed
    ImGui::SliderFloat("Mass", &mass, 50.0f, 300.0f);
    ImGui::SliderFloat("Friction", &friction, 0.0f, 2.0f);
    ImGui::SliderFloat("Linear Damping", &dampingLinear, 0.0f, 1.0f);
    ImGui::SliderFloat("Angular Damping", &dampingAngular, 0.0f, 1.0f);
    ImGui::SliderInt("Substeps", &substeps, 1, 30);

    if (ImGui::Button("Reset Physics")) {
        gravity = 10.0f;
        mass = 300.0f;
        friction = 0.8f;
        dampingLinear = 0.0f;
        dampingAngular = 0.0f;
        substeps = 5;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Position")) {
        mc.setPosition({-836, 1, -170}); // Original spawn point
        physics.resetRigidBodyPosition(mc); // Sync Bullet body
    }
    physics.setGravity(gravity);
    physics.setMass(mc, mass);
    physics.setFriction(friction);
    physics.setDamping(dampingLinear, dampingAngular);
    physics.setSubsteps(substeps); // New method for substeps

    ImGui::End();
}
