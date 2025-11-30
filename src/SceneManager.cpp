//
// Created by Lenovo on 20.11.2025.
//

#include "SceneManager.hpp"
#include <threepp/threepp.hpp>
#include <threepp/loaders/AssimpLoader.hpp>
#include <iostream>

using namespace threepp;

SceneManager::SceneManager() = default;

void SceneManager::setupScene(Scene& scene) {
    scene.background = Color::aliceblue;
    createLighting(scene);
    createGround(scene);
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

void SceneManager::createGround(Scene& scene) {
    auto groundGeo = PlaneGeometry::create(1000, 1000);
    auto groundMat = MeshLambertMaterial::create();
    groundMat->color = 0x2e8b57; // sea green
    auto ground = Mesh::create(groundGeo, groundMat);
    ground->rotation.x = -math::PI / 2.0f; // lay flat
    ground->position.y = 0.0f;
    scene.add(ground);
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
    // Conservative scale for large assets; users can adjust later
    if (loaded->scale.x == 10 && loaded->scale.y == 10 && loaded->scale.z == 10) {
        loaded->scale.set(0.01f, 0.01f, 0.01f);
    }

    loadedModel_ = loaded;
    return true;
}

void SceneManager::addLoadedToScene(Scene& scene) {
    if (loadedModel_) {
        scene.add(*loadedModel_);
    }
}