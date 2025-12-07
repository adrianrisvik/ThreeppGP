
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


    auto mesh = sceneManager.getLoadedMesh();
    if (!mesh) {
        std::cerr << "[ERROR] No mesh found in loaded model!" << std::endl;
        return -1;
    }

    auto geometry = mesh->geometry();

    auto positionAttr = geometry->getAttribute("position");
    auto normalAttr   = geometry->getAttribute("normal");
    auto uvAttr       = geometry->getAttribute("uv");


    std::cout << "[INFO] Geometry details:" << std::endl;

if (positionAttr) {
    std::cout << " - Position count: " << positionAttr->count() << std::endl;
}
if (normalAttr) {
    std::cout << " - Normal count: " << normalAttr->count() << std::endl;
}
if (uvAttr) {
    std::cout << " - UV count: " << uvAttr->count() << std::endl;
}

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

    // MC setup
    MC mc(Vector3(-836, 1, -170));
    mc.setRotation({0, threepp::math::PI / 4, 0});
    mc.setMaxSpeed(100.0f);
    mc.setAcceleration(15.0f);
    mc.setBraking(30.0f);
    mc.setFriction(1.5f);
    mc.setTurn(2.5f);
    mc.enablePhysics(true); // Enable Bullet physics mode

    MCKeyController mcKeyController(mc);
    canvas.addKeyListener(mcKeyController);

    // Physics setup
    Physics physics(500.0f, mc);

    // Power-ups
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
    GameUI gameUI("Ducati", mc.getMaxSpeed());
    ImguiFunctionalContext ui(canvas, [&] {
        gameUI.render(mc);
    });

    // Game loop
    Clock clock;
    canvas.animate([&] {
        const float dt = clock.getDelta();

        // Update input and game logic
        mcKeyController.update(dt);
        powerUpManager.update(mc, dt);
        powerUpManager.animate(dt);

        // Apply Bullet physics movement based on MC speed
        physics.applyAcceleration(mc);
        physics.update(mc, dt);

        // Update visuals
        vehicleLoader.updateTransform(mc, Vector3(0, 0, 0));
        cameraController.update(mc);
        renderer.render(scene, camera);
        ui.render();

        // Debug position every second
        static int frameCount = 0;
        if (++frameCount % 60 == 0) {
            auto pos = mc.getPosition();
            std::cout << "MC Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        }
    });

    return 0;
}
