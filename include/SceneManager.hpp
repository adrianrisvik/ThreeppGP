//
#ifndef THREEPPGP_SCENEMANAGER_HPP
#define THREEPPGP_SCENEMANAGER_HPP
#include <memory>
#include <string>

#include "threepp/objects/Mesh.hpp"

// Forward declarations
namespace threepp {
    class Scene;
    class Group;
}

class SceneManager {
public:
    SceneManager();
    
    void setupScene(threepp::Scene& scene);
    bool loadSceneModel(const std::string& filePath);
    // Add the previously loaded scene/model to the given scene
    void addLoadedToScene(threepp::Scene& scene);

    std::shared_ptr<threepp::Mesh> getLoadedMesh() const;
    std::shared_ptr<threepp::Group> getLoadedModel() const { return loadedModel_; }
    
private:
    static void createLighting(threepp::Scene& scene);
    //void createGround(threepp::Scene& scene);
    void createGrid(threepp::Scene& scene);
    static void upgradeMaterials(threepp::Group& group);
    std::shared_ptr<threepp::Group> loadedModel_{}; // stored (not auto-added) model
    std::shared_ptr<threepp::Mesh> firstMesh_; // Store first mesh
};

#endif // THREEPPGP_SCENEMANAGER_HPP