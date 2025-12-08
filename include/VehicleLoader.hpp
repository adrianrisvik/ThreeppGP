//
// Created by Lenovo on 20.11.2025.
//

#ifndef THREEPPGP_VEHICLELOADER_HPP
#define THREEPPGP_VEHICLELOADER_HPP

#include <memory>
#include <string>

namespace threepp {
    class Group;
    class Scene;
    class Vector3;
}
class MC;
class VehicleLoader {
public:
    VehicleLoader();

    static void upgradeMaterials(threepp::Group& group);
    bool loadVehicle(const std::string& modelPath);
    void addToScene(threepp::Scene& scene);
    void updateTransform(const MC& mc, const threepp::Vector3& initialRotation);
    
    bool isLoaded() const { return model_ != nullptr; }
    
private:
    std::shared_ptr<threepp::Group> model_;
};

#endif // THREEPPGP_VEHICLELOADER_HPP