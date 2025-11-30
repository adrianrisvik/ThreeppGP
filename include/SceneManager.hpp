//
#ifndef THREEPPGP_SCENEMANAGER_HPP
#define THREEPPGP_SCENEMANAGER_HPP
#include <memory>
#include <string>

// Forward declarations
namespace threepp {
    class Scene;
    class Group;
}

class SceneManager {
public:
    SceneManager();
    
    void setupScene(threepp::Scene& scene);
    
    // Load an external scene/model (OBJ/GLB) but do not add it to the scene yet
    bool loadSceneModel(const std::string& filePath);
    // Add the previously loaded scene/model to the given scene
    void addLoadedToScene(threepp::Scene& scene);
    // Access the loaded model (may be null if nothing is loaded)
    std::shared_ptr<threepp::Group> getLoadedModel() const { return loadedModel_; }
    
private:
    void createLighting(threepp::Scene& scene);
    void createGround(threepp::Scene& scene);
    void createGrid(threepp::Scene& scene);
    std::shared_ptr<threepp::Group> loadedModel_{}; // stored (not auto-added) model
};

#endif // THREEPPGP_SCENEMANAGER_HPP