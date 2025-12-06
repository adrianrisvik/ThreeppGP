#include <iostream>
#include <threepp/threepp.hpp>
#include "threepp/extras/imgui/ImguiContext.hpp"

#include "MC.hpp"
#include "MCKeyController.hpp"
#include "CameraController.hpp"
#include "PowerUpManager.hpp"
#include "SceneManager.hpp"
#include "include/VehicleLoader.hpp"
#include "include/UI.hpp"
#include "Physics.hpp"

using namespace threepp;

int main() {
    Canvas canvas("ThreeppGP");
    GLRenderer renderer(canvas.size());
    Scene scene;
    
    // Scene setup
    SceneManager sceneManager;
    sceneManager.setupScene(scene);
    sceneManager.loadSceneModel("models/ArgentinaTrack/source/PistaArgentina2.obj");
    sceneManager.addLoadedToScene(scene);
    
    // Camera setup
    PerspectiveCamera camera(75, canvas.aspect(), 0.1, 2000);
    camera.position.set(0, 2, -5);
    CameraController cameraController(camera, 8.0f, 3.0f);
    
    canvas.onWindowResize([&](WindowSize size) {
        camera.aspect = size.aspect();
        camera.updateProjectionMatrix();
        renderer.setSize(size);
    });
    
    // Vehicle setup
    VehicleLoader vehicleLoader;
    if (!vehicleLoader.loadVehicle("models/Ducati/1388 Motorcycle.obj")) {
        return -1;
    }
    vehicleLoader.addToScene(scene);

    // MC (physics) setup and start pos
    MC mc(Vector3(-836, 1, -170));

    // MC Start rot, physics and visuals
    threepp::Vector3 startRotation{0, threepp::math::PI / 4, 0};
    mc.setRotation(startRotation);

    const float baseMaxSpeed = 100.0f;
    mc.setMaxSpeed(baseMaxSpeed);
    mc.setAcceleration(15.0f);
    mc.setBraking(30.0f);
    mc.setFriction(1.5f);
    mc.setTurn(2.5f);

    MCKeyController mcKeyController(mc);
    canvas.addKeyListener(mcKeyController);

    //Physics
    Physics physics(-9.8f, 0.0f);


    // Power-ups setup and spawn locations
    PowerUpManager powerUpManager;
    powerUpManager.addSpeedBoost(Vector3(-1023, 0, -223));
    powerUpManager.addSpeedBoost(Vector3(-1040, 0, 76));
    powerUpManager.addSpeedBoost(Vector3(-1046, 0, 88));
    powerUpManager.addSpeedBoost(Vector3(-277, 0, 916));
    powerUpManager.addSpeedBoost(Vector3(-270, 0, 910));

    powerUpManager.addOilSpill(Vector3(15, 0, 0));
    powerUpManager.addOilSpill(Vector3(-1016, 0, -202));
    powerUpManager.addOilSpill(Vector3(-1178, 0, -39));
    powerUpManager.addOilSpill(Vector3(-262, 0, -918));

    powerUpManager.addToScene(scene);

    // UI setup
    GameUI gameUI("Ducati", baseMaxSpeed);
    ImguiFunctionalContext ui(canvas, [&] {
        gameUI.render(mc);
    });

    // Game loop
    Clock clock;
    canvas.animate([&] {
        const float dt = clock.getDelta();

        mcKeyController.update(dt);
        powerUpManager.update(mc, dt);
        powerUpManager.animate(dt);
        
        // Don't pass any initial rotation - MC already has it stored
        vehicleLoader.updateTransform(mc, Vector3(0, 0, 0));  // Changed to zeros
        
        cameraController.update(mc);
        renderer.render(scene, camera);

        ui.render();

        static int frameCount = 0;
        if (++frameCount % 60 == 0) {
            auto pos = mc.getPosition();
            std::cout << "MC Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        }
        float deltaTime = 0.016f; // or calculate dynamically
        physics.update(mc, deltaTime);
    });
    return 0;
}
