//
// Created by Lenovo on 20.11.2025.
//

#include "SceneManager.hpp"
#include <threepp/threepp.hpp>
#include <threepp/loaders/AssimpLoader.hpp>
#include <iostream>
#include <threepp/materials/MeshStandardMaterial.hpp>

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

// Add this improved helper function
// void SceneManager::upgradeMaterials(threepp::Group& group) {
//     group.traverse([](threepp::Object3D& obj) {
//         if (auto mesh = dynamic_cast<threepp::Mesh*>(&obj)) {
//             auto currentMat = mesh->material();
//             if (currentMat) {
//                 auto newMat = MeshStandardMaterial::create();
//
//                 // Type-safe copying
//                 if (auto basicMat = std::dynamic_pointer_cast<MeshBasicMaterial>(currentMat)) {
//                     newMat->color = basicMat->color;
//                     newMat->map = basicMat->map;
//                     newMat->roughness = 0.8f;  // Higher for less shine
//                     newMat->metalness = 0.0f;
//                 } else if (auto lambertMat = std::dynamic_pointer_cast<MeshLambertMaterial>(currentMat)) {
//                     newMat->color = lambertMat->color;
//                     newMat->map = lambertMat->map;
//                     newMat->emissive = lambertMat->emissive;
//                     newMat->roughness = 0.7f;
//                     newMat->metalness = 0.0f;  // Non-metallic
//                 } else if (auto phongMat = std::dynamic_pointer_cast<MeshPhongMaterial>(currentMat)) {
//                     newMat->color = phongMat->color;
//                     newMat->map = phongMat->map;
//                     newMat->emissive = phongMat->emissive;
//                     newMat->roughness = 0.8f - (phongMat->shininess / 256.0f);  // Adjusted for less shine
//                     newMat->metalness = 0.0f;
//                 } else {
//                     // Fallback: Neutral defaults with texture check
//                     newMat->color.setRGB(0.5f, 0.5f, 0.5f);
//                     newMat->roughness = 0.8f;
//                     newMat->metalness = 0.0f;
//                 }
//
//                 // Anti-white safeguard
//                 if (newMat->color.equals(Color(1,1,1)) && !newMat->map) {
//                     newMat->color.setRGB(0.8f, 0.8f, 0.8f);
//                 }
//
//                 // Log for debugging textures
//                 std::cout << "Material upgraded - Color: " << newMat->color.r << ", " << newMat->color.g << ", " << newMat->color.b
//                           << " | Texture: " << (newMat->map ? "Loaded" : "Missing") << std::endl;
//
//                 // Enable shadows
//                 //newMat->receiveShadow = true;
//                 //newMat->castShadow = true;
//
//                 mesh->setMaterial(newMat);
//             }
//         }
//     });
// }

// void SceneManager::upgradeMaterials(threepp::Group& group) {
//     group.traverse([](threepp::Object3D& obj) {
//         auto mesh = dynamic_cast<threepp::Mesh*>(&obj);
//         if (!mesh) return;
//
//         auto currentMat = mesh->material();
//         if (!currentMat) return;
//
//         auto newMat = MeshStandardMaterial::create();
//
//         // Copy diffuse (map_Kd)
//         if (auto phongMat = std::dynamic_pointer_cast<MeshPhongMaterial>(currentMat)) {
//             newMat->color = phongMat->color;
//             if (phongMat->map) newMat->map = phongMat->map; // diffuse
//             if (phongMat->normalMap) newMat->normalMap = phongMat->normalMap;
//             if (phongMat->specularMap) newMat->metalnessMap = phongMat->specularMap;
//             newMat->emissive = phongMat->emissive; // ambient/emissive
//         } else if (auto lambertMat = std::dynamic_pointer_cast<MeshLambertMaterial>(currentMat)) {
//             newMat->color = lambertMat->color;
//             if (lambertMat->map) newMat->map = lambertMat->map;
//             newMat->emissive = lambertMat->emissive;
//         } else if (auto basicMat = std::dynamic_pointer_cast<MeshBasicMaterial>(currentMat)) {
//             newMat->color = basicMat->color;
//             if (basicMat->map) newMat->map = basicMat->map;
//         }
//
//         // Set PBR defaults
//         newMat->roughness = 0.8f;
//         newMat->metalness = 0.0f;
//
//         mesh->setMaterial(newMat);
//
//         // Debug log
//         std::cout << "[INFO] Mesh: " << mesh->name
//                   << " | Diffuse: " << (newMat->map ? "Loaded" : "Missing")
//                   << " | Normal: " << (newMat->normalMap ? "Loaded" : "Missing")
//                   << std::endl;
//     });
// }


bool SceneManager::loadSceneModel(const std::string& filePath) {
    threepp::AssimpLoader loader;
    auto loaded = loader.load(filePath);
    if (!loaded) {
        std::cerr << "Failed to load scene model: " << filePath << std::endl;
        return false;
    }
   // upgradeMaterials(*loaded);



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