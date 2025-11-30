#include "../include/VehicleLoader.hpp"
#include "MC.hpp"
#include <threepp/threepp.hpp>
#include <threepp/loaders/AssimpLoader.hpp>
#include <iostream>

using namespace threepp;

VehicleLoader::VehicleLoader() = default;

bool VehicleLoader::loadVehicle(const std::string& modelPath) {
    AssimpLoader loader;
    model_ = loader.load(modelPath);
    
    if (!model_) {
        std::cerr << "Failed to load vehicle model: " << modelPath << std::endl;
        return false;
    }
    
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