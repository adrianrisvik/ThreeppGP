#include <iostream>
#include <vector>
#include <functional>
#include <threepp/threepp.hpp>


#include "CameraController.hpp"
#include "VehicleManager.hpp"
#include "include/MC.hpp"
#include "include/MCKeyController.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "include/PowerUpManager.hpp"
#include "include/Logo.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"

using namespace threepp;

int main() {

    Canvas canvas("ThreeppGP");
    GLRenderer renderer(canvas.size());

    // We'll initialize ImGui for the splash logo first, then for the HUD later


    // Initialize and show logo
    // {
    //     Logo logo(renderer, canvas, "models/ChatGPT Image Nov 15, 2025, 09_41_38 PM.png", 6.0f, true);
    //
    //     // Create a temporary ImGui context to render the logo
    //     ImguiFunctionalContext logoUi(canvas, [&] {
    //         // Draw the logo using ImGui every frame
    //         // Logo::render performs the ImGui drawing and returns completion state
    //         logo.render();
    //     });

        // // Drive a minimal render loop until the logo completes or the window closes
        // while (canvas.isOpen() && !logo.isComplete()) {
        //     // animateOnce processes events, starts a frame, calls our lambda once, and presents
        //     const bool keepRunning = canvas.animateOnce([&] {
        //         // The ImGui frame itself is driven by logoUi.render()
        //         logoUi.render();
        //     });
        //
        //     if (!keepRunning) break;
        // }
    } // destroy logo ImGui context before creating the main HUD context

    AssimpLoader loader;

    // Load all three vehicle models
    auto ducatiModel = loader.load("models/Ducati/1388 Motorcycle.obj");
    auto bicycleModel = loader.load("models/Bicycle/Bike.obj");
    auto speederModel = loader.load("models/Castrol/source/A8A0JG3NAIHR3JFNL54EL5WL6_obj/A8A0JG3NAIHR3JFNL54EL5WL6.obj");

       if (!ducatiModel || !bicycleModel || !speederModel) {
        std::cerr << "Failed to load one or more models!" << std::endl;
        return -1;
    }

    // Fix model orientations and scales
    // Bicycle - rotate 90 degrees counter-clockwise
    // bicycleModel->rotation.y = -math::PI / 2.0f;  // 90 degrees counter-clockwise
    // bicycleModel->rotation.z = -math::PI / 2.0f;  // 90 degrees counter-clockwise
    bicycleModel->scale.set(1.0f, 1.0f, 1.0f);

    // Ducati - adjust scale and position
    ducatiModel->scale.set(0.02f, 0.02f, 0.02f);
    ducatiModel->position.y = -0.5f;

    // Speeder - rotate 90 degrees counter-clockwise and scale up
    speederModel->scale.set(1.0f, 3.0f, 3.0f);
    // Don't set rotation here anymore - we'll use setInitialRotation instead

    // Bicycle
    bicycleModel->scale.set(1.0f, 1.0f, 1.0f);

    // Create scene
    Scene scene;
    scene.background = Color::aliceblue;

    auto ambientLight = AmbientLight::create(0xffffff, 0.6f);
    scene.add(ambientLight);

    auto directionalLight = DirectionalLight::create(0xffffff, 0.8f);
    directionalLight->position.set(10, 10, 5);
    scene.add(directionalLight);

    // Add another light from different angle
    auto directionalLight2 = DirectionalLight::create(0xffffff, 0.4f);
    directionalLight2->position.set(-10, 10, -5);
    scene.add(directionalLight2);

    // Load map
    auto raceTrack = loader.load("models/Race track/race_track__low_poly.glb");
    if (!raceTrack) {
        std::cerr << "Warning: Failed to load race track (continuing without it)" << std::endl;
    } else {
        raceTrack->scale.set(5.0f, 5.0f, 5.0f);  // Scale up 5 times
        scene.add(raceTrack);
        std::cout << "Race track loaded successfully!" << std::endl;
    }

    auto grid = GridHelper::create();
    scene.add(grid);

    PerspectiveCamera camera(75, canvas.aspect(), 0.1, 1000);
    camera.position.set(0, 2, -5);

    CameraController cameraController(camera, 5.0f, 3.0f);

    // Add window resize callback
    canvas.onWindowResize([&](WindowSize size) {
        camera.aspect = size.aspect();
        camera.updateProjectionMatrix();
        renderer.setSize(size);
    });


    MC mc(Vector3(0, 2 , 0));  // Start at height 2 to be above ground

   VehicleManager vehicleManager;
    vehicleManager.addVehicle(ducatiModel, VehicleStats{30.0f, 15.0f, 14.0f, 1.5f, 2.5f, "Ducati"});
    vehicleManager.addVehicle(speederModel, VehicleStats{20.0f, 6.0f, 10.0f, 2.0f, 2.3f, "Speeder Bike"});
    vehicleManager.addVehicle(bicycleModel, VehicleStats{8.0f, 4.0f, 8.0f, 2.5f, 2.0f, "Bicycle"});

    // Set initial rotations for vehicles that need it
    // Note: revert speeder-specific initial rotation so user can handle its orientation manually
    // vehicleManager.getVehicle(1).setInitialRotation(Vector3(0, math::PI) / -2.0f, math::PI*2)); // Speeder bike (reverted)
    vehicleManager.getVehicle(2).setInitialRotation(Vector3(0, 0, math::PI)); // Bicycle


    // Add initial Ducati (index 0) to the scene and sync MC stats
    vehicleManager.addInitialVehicleToScene(scene);
    {
        const auto& vStats = vehicleManager.getCurrentVehicle().getStats();
        mc.setMaxSpeed(vStats.maxSpeed);
        mc.setAcceleration(vStats.acceleration);
        mc.setBraking(vStats.braking);
        mc.setFriction(vStats.friction);
        mc.setTurn(vStats.turn);
    }


    PowerUpManager powerUpManager;

    powerUpManager.addSpeedBoost(Vector3(10, 0, 0));
    powerUpManager.addSpeedBoost(Vector3(50, 0, 20));
    powerUpManager.addSpeedBoost(Vector3(100, 0, 300));

    powerUpManager.addOilSpill(Vector3(15, 0, 0));
    powerUpManager.addOilSpill(Vector3(500, 0, 20));
    powerUpManager.addOilSpill(Vector3(100, 0, 200));

    powerUpManager.addToScene(scene);

    MCKeyController mcKeyController(mc);

    // Set up vehicle switching callback
    mcKeyController.onVehicleChange = [&](int newIndex) {
        vehicleManager.switchVehicle(newIndex, scene, mc);
    };

    canvas.addKeyListener(mcKeyController);

    // Initialize ImGui context (after managers are set up)
    // Use the Canvas overload so the context hooks into the canvas lifecycle properly
    ImguiFunctionalContext ui(canvas, [&] {
        // Simple HUD/controls
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::Begin("Vehicle & Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        int currentIdx = vehicleManager.getCurrentVehicleIndex();
        const int count = static_cast<int>(vehicleManager.getVehicleCount());
        std::vector<const char*> names;
        names.reserve(count);
        for (int i = 0; i < count; ++i) {
            names.push_back(vehicleManager.getVehicle(i).getStats().name.c_str());
        }
        if (!names.empty()) {
            if (ImGui::Combo("Vehicle", &currentIdx, names.data(), count)) {
                vehicleManager.switchVehicle(currentIdx, scene, mc);
            }
        }

        const auto& stats = vehicleManager.getCurrentVehicle().getStats();
        ImGui::Separator();
        ImGui::Text("Current: %s", stats.name.c_str());
        // Speed readout and bar
        const float curSpd = mc.getCurrentSpeed();
        const float maxSpd = mc.getMaxSpeed();
        ImGui::Text("Speed: %.2f / %.2f", curSpd, maxSpd);
        float speedRatio = (maxSpd > 0.0f) ? std::min(std::abs(curSpd) / maxSpd, 1.0f) : 0.0f;
        ImGui::ProgressBar(speedRatio, ImVec2(200, 0), "");

        // Boost indicator: if current max speed exceeds base vehicle max, show active
        bool boostActive = maxSpd > stats.maxSpeed * 1.05f; // 5% threshold
        if (boostActive) {
            ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "BOOST ACTIVE");
        } else {
            ImGui::TextDisabled("Boost: inactive");
        }

        ImGui::Separator();
        ImGui::TextDisabled("Controls: WASD or arrows to drive, 1/2/3 to switch vehicles");
        ImGui::End();
    });

    Clock clock;
    canvas.animate([&] {
        const float dt = clock.getDelta();

        mcKeyController.update(dt);

        powerUpManager.update(mc, dt);
        powerUpManager.animate(dt);

        auto mcPos = mc.getPosition();
        auto mcRot = mc.getVisualRotation();

        // DEBUG: Print MC position every 60 frames
        static int frameCount = 0;
        if (++frameCount % 60 == 0) {
            std::cout << "MC Position: (" << mcPos.x << ", " << mcPos.y << ", " << mcPos.z << ")" << std::endl;
        }

        vehicleManager.updateCurrentVehicle(mcPos, mcRot, mc.getCurrentLean());

        cameraController.update(mc);

        renderer.render(scene, camera);
    });

    return 0;
}