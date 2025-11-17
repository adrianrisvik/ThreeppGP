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

    // Initialize ImGui context
    ImguiFunctionalContext ui(canvas.windowPtr(), [&] {
        // ImGui rendering will happen here during the main loop
    });


    // Initialize and show logo
    // Logo logo(renderer, canvas, "models/ChatGPT Image Nov 15, 2025, 09_41_38 PM.png", 6.0f, true);
    //
    // bool logoShowing = true;
    //
    // // Logo display loop
    // Clock logoClock;
    // while (!logo.isComplete()) {
    //     if (!logo.render()) {
    //         break;  // User skipped or window closed
    //     }
    // }

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
    speederModel->scale.set(3.0f, 3.0f, 3.0f);
    speederModel->rotation.y = math::PI / 2.0f;  // 90 degrees counter-clockwise
    speederModel->rotation.z = math::PI / 2.0f;

    //Fix model orientations and scales
    //Bicycle - rotate 90 degrees counter-clockwise
    speederModel->rotation.y = -math::PI / 2.0f;  // 90 degrees counter-clockwise
    speederModel->rotation.z = -math::PI / 2.0f;  // 90 degrees counter-clockwise
    speederModel->scale.set(1.0f, 1.0f, 1.0f);

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
    auto raceTrack = loader.load("models/ArgentinaTrack/source/PistaArgentina2.obj");
    if (!raceTrack) {
        std::cerr << "Warning: Failed to load race track (continuing without it)" << std::endl;
    } else {
        raceTrack->scale.set(5.0f, 5.0f, 5.0f);  // Scale up 5 times
        //raceTrack->rotation.x = -math::PI / 2;   // Rotate 90 degrees to lay flat
        scene.add(raceTrack);
        std::cout << "Race track loaded successfully!" << std::endl;
    }

    auto grid = GridHelper::create();
    scene.add(grid);

    PerspectiveCamera camera(75, canvas.aspect(), 0.1, 1000);
    camera.position.set(0, 2, -5);

    CameraController cameraController(camera, 5.0f, 3.0f);

    MC mc(Vector3(0, 2 , 0));  // Start at height 2 to be above ground

   VehicleManager vehicleManager;
    vehicleManager.addVehicle(ducatiModel, VehicleStats{30.0f, 15.0f, 14.0f, 1.5f, 2.5f, "Ducati"});
    vehicleManager.addVehicle(speederModel, VehicleStats{20.0f, 6.0f, 10.0f, 2.0f, 2.3f, "Speeder Bike"});
    vehicleManager.addVehicle(bicycleModel, VehicleStats{8.0f, 4.0f, 8.0f, 2.5f, 2.0f, "Bicycle"});

    vehicleManager.switchVehicle(0, scene, mc);


    PowerUpManager powerUpManager;

    powerUpManager.addSpeedBoost(Vector3(10, 0, 0));
    powerUpManager.addSpeedBoost(Vector3(50, 0, 20));
    powerUpManager.addSpeedBoost(Vector3(100, 0, 300));

    powerUpManager.addOilSpill(Vector3(10, 0, 0));
    powerUpManager.addOilSpill(Vector3(500, 0, 20));
    powerUpManager.addOilSpill(Vector3(1000, 0, 200));

    powerUpManager.addToScene(scene);

    MCKeyController mcKeyController(mc);

    // Set up vehicle switching callback
    mcKeyController.onVehicleChange = [&](int newIndex) {
        vehicleManager.switchVehicle(newIndex, scene, mc);
    };

    canvas.addKeyListener(mcKeyController);

    Clock clock;
    canvas.animate([&] {
        const float dt = clock.getDelta();

        bicycleModel->rotation.y = -math::PI / 2.0f;  // 90 degrees counter-clockwise
        bicycleModel->rotation.z = -math::PI / 2.0f;  // 90 degrees counter-clockwise

        mcKeyController.update(dt);

        powerUpManager.update(mc, dt);
        powerUpManager.animate(dt);

        auto mcPos = mc.getPosition();
        auto mcRot = mc.getRotation();

        vehicleManager.updateCurrentVehicle(mcPos, mcRot, mc.getCurrentLean());

        cameraController.update(mc);

        renderer.render(scene, camera);
    });

    return 0;
}