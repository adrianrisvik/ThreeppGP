cmake_minimum_required(VERSION 3.20)
project(ThreeppGP)

set(CMAKE_CXX_STANDARD 20)

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
        GIT_TAG docking  # Using docking branch (latest stable)
)


FetchContent_MakeAvailable(threepp)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(imgui)

# Create ImGui library
add_library(imgui_lib
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        include/VehicleStats.hpp
        include/Vehicle.hpp
)

target_include_directories(imgui_lib PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC glfw)

add_executable(ThreeppGP main.cpp
        include/MC.hpp
        include/MCKeyController.hpp
        include/PowerUp.hpp
        include/PowerUpManager.hpp
        include/SpeedBoost.hpp
        include/OilSpill.hpp
        include/CameraController.hpp
        include/ImguiContext.hpp

        src/PowerUpManager.cpp
        src/SpeedBoost.cpp
        src/OilSpill.cpp
        src/Logo.cpp
        include/Logo.hpp
        src/CameraController.cpp
        src/VehicleStats.cpp
        include/VehicleStats.hpp
        include/VehicleStats.hpp
        src/Vehicle.cpp
        include/Vehicle.hpp
        include/Vehicle.hpp
        src/VehicleManager.cpp
        include/VehicleManager.hpp
)


target_link_libraries(ThreeppGP PUBLIC threepp::threepp)
target_link_libraries(ThreeppGP PUBLIC assimp::assimp)
target_link_libraries(ThreeppGP PUBLIC imgui_lib)

target_include_directories(ThreeppGP PUBLIC "include")
target_include_directories(ThreeppGP PUBLIC "models")
target_include_directories(ThreeppGP PUBLIC "src")  # Add src to include paths
