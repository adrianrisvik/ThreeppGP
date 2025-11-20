#include <iostream>
#include <vector>
#include <functional>
#include <threepp/threepp.hpp>


#include "CameraController.hpp"
#include "include/MC.hpp"
#include "include/MCKeyController.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "include/PowerUpManager.hpp"

#include "threepp/extras/imgui/ImguiContext.hpp"

using namespace threepp;

int main() {
    Canvas canvas("ThreeppGP");
    GLRenderer renderer(canvas.size());


    AssimpLoader loader;

    auto ducatiModel = loader.load("models/Ducati/1388 Motorcycle.obj");

        if (!ducatiModel) {
            std::cerr << "Failed to load Ducati model!" << std::endl;
            return -1;
        }

        ducatiModel->scale.set(0.02f, 0.02f, 0.02f);
        ducatiModel->position.y = -0.5f;

        Scene scene;
        scene.background = Color::aliceblue;

        auto ambientLight = AmbientLight::create(0xffffff, 0.6f);
        scene.add(ambientLight);

        auto directionalLight = DirectionalLight::create(0xffffff, 0.8f);
        directionalLight->position.set(10, 10, 5);
        scene.add(directionalLight);

        auto directionalLight2 = DirectionalLight::create(0xffffff, 0.4f);
        directionalLight2->position.set(-10, 10, -5);
        scene.add(directionalLight2);

        {
            auto groundGeo = PlaneGeometry::create(1000, 1000);
            auto groundMat = MeshLambertMaterial::create();
            groundMat->color = 0x2e8b57; // sea green
            auto ground = Mesh::create(groundGeo, groundMat);
            ground->rotation.x = -math::PI / 2.0f; // lay flat
            ground->position.y = 0.0f;
            scene.add(ground);
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

        // Single-vehicle setup (no Vehicle/VehicleManager)
        const std::string vehicleName = "Ducati";
        const float baseMaxSpeed = 30.0f;
        const float baseAcceleration = 15.0f;
        const float baseBraking = 14.0f;
        const float baseFriction = 1.5f;
        const float baseTurn = 2.5f;

        // Sync MC physics with the chosen vehicle stats
        mc.setMaxSpeed(baseMaxSpeed);
        mc.setAcceleration(baseAcceleration);
        mc.setBraking(baseBraking);
        mc.setFriction(baseFriction);
        mc.setTurn(baseTurn);

        // Add the Ducati model directly to the scene
        scene.add(*ducatiModel);

        // Optional initial rotation offset for the model
        Vector3 modelInitialRotation{0, 0, 0};


        PowerUpManager powerUpManager;

        powerUpManager.addSpeedBoost(Vector3(10, 0, 0));
        powerUpManager.addSpeedBoost(Vector3(50, 0, 20));
        powerUpManager.addSpeedBoost(Vector3(100, 0, 300));

        powerUpManager.addOilSpill(Vector3(15, 0, 0));
        powerUpManager.addOilSpill(Vector3(500, 0, 20));
        powerUpManager.addOilSpill(Vector3(100, 0, 200));

        powerUpManager.addToScene(scene);

        MCKeyController mcKeyController(mc);

        // Vehicle switching removed (only Ducati is available)

        canvas.addKeyListener(mcKeyController);

        // Initialize ImGui context (after managers are set up)
        // Use the Canvas overload so the context hooks into the canvas lifecycle properly
        ImguiFunctionalContext ui(canvas, [&] {
            // Simple HUD/controls
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::Begin("Vehicle & Stats");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

            // Vehicle selection removed (only Ducati)

            ImGui::Separator();
            ImGui::Text("Current: %s", vehicleName.c_str());
            // Speed readout and bar
            const float curSpd = mc.getCurrentSpeed();
            const float maxSpd = mc.getMaxSpeed();
            ImGui::Text("Speed: %.2f / %.2f", curSpd, maxSpd);
            float speedRatio = (maxSpd > 0.0f) ? std::min(std::abs(curSpd) / maxSpd, 1.0f) : 0.0f;
            ImGui::ProgressBar(speedRatio, ImVec2(200, 0), "");

            // Boost indicator: if current max speed exceeds base vehicle max, show active
            bool boostActive = maxSpd > baseMaxSpeed * 1.05f; // 5% threshold
            if (boostActive) {
                ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "BOOST ACTIVE");
            } else {
                ImGui::TextDisabled("Boost: inactive");
            }

            ImGui::Separator();
            ImGui::TextDisabled("Controls: WASD or arrows to drive");
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

            // Update the single vehicle model transform directly from MC
            if (ducatiModel) {
                // Position
                ducatiModel->position = mcPos;
                // Rotation: apply initial rotation + mc rotation + lean around Z
                ducatiModel->rotation.x = modelInitialRotation.x + mcRot.x;
                ducatiModel->rotation.y = modelInitialRotation.y + mcRot.y;
                ducatiModel->rotation.z = modelInitialRotation.z + mcRot.z + mc.getCurrentLean();
            }

            cameraController.update(mc);

            renderer.render(scene, camera);
        });

        return 0;
    }
