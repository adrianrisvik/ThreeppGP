//
// Created by Lenovo on 06.12.2025.
//

#include "../include/VehicleLoader.hpp"
#include "../include/MC.hpp"
#include <iostream>
#include "../include/SceneManager.hpp"
#include <threepp/loaders/AssimpLoader.hpp>
#include <threepp/threepp.hpp>

using namespace threepp;
VehicleLoader::VehicleLoader() = default;

void VehicleLoader::upgradeMaterials(threepp::Group& group) {

    group.traverse([](threepp::Object3D& obj) {

        if (auto mesh = dynamic_cast<threepp::Mesh*>(&obj)) {

            // Skip if mesh doesn't have material
            auto mat = mesh->material();
            if (!mat) return;

            if (!dynamic_cast<MeshStandardMaterial*>(mat.get())) {
                auto standardMat = threepp::MeshStandardMaterial::create();


                mesh->setMaterial(standardMat);
                mat = standardMat;
            }
            auto* sm = dynamic_cast<MeshStandardMaterial*>(mat.get());
            sm->roughness = 0.8f;
            sm->metalness = 0.0f;

            std::cout << "Vehicle material - Color: "
                      << sm->color.r << ", " << sm->color.g << ", " << sm->color.b
                      << " | Texture: " << (sm->map ? "Loaded" : "Missing")
                      << std::endl;
        }
    });
}
bool VehicleLoader::loadVehicle(const std::string& modelPath) {

    AssimpLoader loader;

    model_ = loader.load(modelPath);
    if (!model_) {
        std::cerr << "Failed to load vehicle model: " << modelPath << std::endl;
        return false;
    }

    upgradeMaterials(*model_);

    model_->scale.set(0.03f, 0.03f, 0.03f);
    model_->position.y = -0.5f;

    return true;
}

void VehicleLoader::addToScene(Scene& scene) {
    if (model_) {
        scene.add(*model_);
    }
}

void VehicleLoader::updateTransform(const MC& mc, const Vector3& initialRotation) {
    if (!model_) return;

    auto mcPos = mc.getPosition();
    auto mcRot = mc.getVisualRotation();

    model_->position = mcPos;
    model_->rotation.x = initialRotation.x + mcRot.x;
    model_->rotation.y = initialRotation.y + mcRot.y;
    model_->rotation.z = initialRotation.z + mcRot.z + mc.getCurrentLean();
}
