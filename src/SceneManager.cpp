//
// Created by Lenovo on 06.12.2025.
//
#include "SceneManager.hpp"
#include <threepp/threepp.hpp>
#include <threepp/loaders/AssimpLoader.hpp>
#include <iostream>
#include <threepp/materials/MeshStandardMaterial.hpp>
#include <threepp/objects/Mesh.hpp>

using namespace threepp;

SceneManager::SceneManager() = default;

void SceneManager::setupScene(Scene& scene) {
    scene.background = Color::aliceblue;
    createLighting(scene);
    createGrid(scene);
}

void SceneManager::createLighting(Scene& scene) {
    auto ambientLight = AmbientLight::create(0xffffff, 0.6f);
    scene.add(ambientLight);

    auto directionalLight = DirectionalLight::create(0xffffff, 0.8f);
    directionalLight->position.set(10, 10, 5);
    scene.add(directionalLight);

    auto directionalLight2 = DirectionalLight::create(0xffffff, 0.4f);
    directionalLight2->position.set(-10, 10, -5);
    scene.add(directionalLight2);
}

void SceneManager::createGrid(Scene& scene) {
    auto grid = GridHelper::create();
    scene.add(grid);
}

bool SceneManager::loadSceneModel(const std::string& filePath) {
    threepp::AssimpLoader loader;
    auto loaded = loader.load(filePath);
    if (!loaded) {
        std::cerr << "Failed to load scene model: " << filePath << std::endl;
        return false;
    }

    // Basic normalization: ensure model is visible and above ground
    loaded->position.y = 0.0f;
    if (loaded->scale.x == 10 && loaded->scale.y == 10 && loaded->scale.z == 10) {
        loaded->scale.set(0.01f, 0.01f, 0.01f);
    }

    loadedModel_ = loaded;

    // Capture the first mesh immediately (safe and simple)

    firstMesh_ = nullptr;
    loadedModel_->traverse([&](threepp::Object3D& obj) {
        if (!firstMesh_) {
            if (auto mesh = dynamic_cast<threepp::Mesh*>(&obj)) {
                // We can't create a new shared_ptr from obj, but we can store a raw pointer temporarily
                // Instead, store the mesh name and search later OR just keep a raw pointer for short-term use
                // For simplicity, store a raw pointer and wrap it in a non-owning shared_ptr
                firstMesh_ = std::shared_ptr<threepp::Mesh>(mesh, [](threepp::Mesh*) {
                    // Do nothing: non-owning deleter
                });
            }
        }
    });

    return true;
}

void SceneManager::addLoadedToScene(Scene& scene) {
    if (loadedModel_) {
        scene.add(*loadedModel_);
    }
}

std::shared_ptr<threepp::Mesh> SceneManager::getLoadedMesh() const {
    return firstMesh_;
}
