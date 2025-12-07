cmake_minimum_required(VERSION 3.20)
project(ThreeppGP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

include(FetchContent)
set(THREEPP_BUILD_TESTS OFF)
set(THREEPP_BUILD_EXAMPLES OFF)


FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 619cdaacc2e7dc6ea709d8c7f482e2994af7349d
)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(threepp)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(catch2)

# Create ImGui library
add_library(imgui_lib
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC glfw)

add_library(threeppgp_lib STATIC
        src/PowerUpManager.cpp
        src/SpeedBoost.cpp
        src/OilSpill.cpp
        src/CameraController.cpp
        src/SceneManager.cpp
        src/UI.cpp
        src/VehicleLoader.cpp
        src/Physics.cpp
)
target_link_libraries(threeppgp_lib PUBLIC threepp::threepp assimp::assimp imgui_lib)
target_include_directories(threeppgp_lib PUBLIC "include" "models" "src")


add_executable(ThreeppGP
        main.cpp

)

# ============ PRECOMPILED HEADERS - ADD THIS! ============
target_precompile_headers(ThreeppGP PRIVATE
        <threepp/threepp.hpp>
        <threepp/loaders/AssimpLoader.hpp>
        <threepp/extras/imgui/ImguiContext.hpp>
        <threepp/scenes/Scene.hpp>      # Add if using scenes
        <threepp/lights/Light.hpp>    # Add if using lights
        <threepp/cameras/Camera.hpp>  # Add if using cameras
        <threepp/materials/MeshStandardMaterial.hpp>  # Add if using materials
        <memory>
        <vector>
        <string>
        <iostream>
)
# =========================================================
find_package(Bullet CONFIG REQUIRED)


target_link_libraries(ThreeppGP PRIVATE threeppgp_lib)
target_link_libraries(ThreeppGP PRIVATE BulletDynamics BulletCollision LinearMath)
target_include_directories(ThreeppGP PUBLIC "include" "models" "src")
include_directories(${BULLET_INCLUDE_DIRS})

add_subdirectory(tests)