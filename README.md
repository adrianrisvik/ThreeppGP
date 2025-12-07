🏍️ ThreeppGP
📖 Overview
This is a simple motorcycle racing game inspired by MotoGP, built in C++ using:

Bullet Physics for realistic movement
Assimp for loading 3D models
ImGui for UI controls

The track is based on the MotoGP Argentina circuit, so it feels like a real race track (at least visually 😉).

🎮 Controls

W / A / S / D → Move the bike
SPACE → Active braking
UI Buttons → Reset position/Physics
UI Sliders → Adjust gravity, friction, and other physics settings in real-time


✨ Features

Dynamic Motorcycle Movement

Acceleration, braking, friction
Leaning effect when turning for realism


Power-Ups

Oil Spill: Makes the bike lose traction and spin
Speed Boost: Gives a temporary speed boost


ImGui Interface

Sliders for gravity, damping, and more
Button for resetting position


🛠️ How It’s Built
The project tries to follow Separation of Concerns (SoC) and high cohesion (as much as possible for a first project):
✅ Class Structure


MC (Motorcycle)

Handles bike state: position, rotation, speed, lean angle
Movement logic (acceleration, braking, turning)
Cohesion: All motorcycle-related logic is in one place



Physics

Sets up Bullet Physics world
Handles gravity, collisions, and updates bike position
SoC: Physics is separate from rendering
Coupling: Talks to MC for position updates



VehicleLoader

Loads the motorcycle 3D model using Assimp
Applies materials and sets visual scale
Cohesion: Only cares about visuals



SceneManager

Adds lights, grid, and track model
Keeps rendering setup separate from game logic



UI

ImGui sliders and buttons for runtime tuning
Decoupled from physics and rendering


📂 Code Layout
src/
├── main.cpp            # Game loop
├── MC.hpp / MC.cpp     # Motorcycle logic
├── Physics.hpp / cpp   # Bullet physics integration
├── VehicleLoader.cpp   # Loads bike model
├── SceneManager.cpp    # Scene setup
├── UI.cpp              # ImGui interface


⚙️ Build Instructions
This project uses CMake and vcpkg for dependencies.
1. Clone vcpkg
   git clone https://github.com/microsoft/vcpkg.gitcd vcpkg./bootstrap-vcpkg.sh   # or bootstrap-vcpkg.bat on WindowsVis flere linjer
2. Install dependencies
   ./vcpkg install threepp assimp bullet3 imguiVis flere linjer
3. Build the project
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake

✅ How It Works

Game Loop updates input, physics, and UI every frame
Physics uses Bullet to simulate gravity and collisions
Rendering uses threepp for visuals
UI lets you tweak physics in real-time


🚀 Future Improvements

Use Bullet’s Raycast Vehicle for realistic suspension, currently not the best physics implementation. 
Make the motorcycle align with the slope when going up/downhill
Add sound/more visual effetcs
