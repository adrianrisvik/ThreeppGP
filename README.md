ThreeppGP

Overview

This is a simple motorcycle racing game inspired by MotoGP, built in C++ using:

Bullet Physics for realistic movement
Assimp for loading 3D models
ImGui for UI controls

The track is based on the MotoGP Argentina circuit, so it feels like a real race track (at least visually 😉).

Controls

W / A / S / D → Move the bike
SPACE → Active braking
UI Buttons → Reset position/Physics
UI Sliders → Adjust gravity, friction, and other physics settings in real-time


Features

Dynamic Motorcycle Movement

Acceleration, braking, friction
Leaning effect when turning for realism


Power-Ups

Oil Spill: Makes the bike lose traction and spin
Speed Boost: Gives a temporary speed boost


ImGui Interface

Sliders for gravity, damping, and more
Button for resetting position


Class Structure


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


Code Layout

src/
├── main.cpp                   # Entry point and game loop

├── MC.hpp                     # Motorcycle class: position, rotation, speed, lean logic

├── MCKeyController.hpp        # Handles keyboard input for motorcycle

├── CameraController.hpp / cpp # Controls camera following the bike

├── Physics.hpp / cpp          # Bullet physics integration (gravity, collisions)

├── VehicleLoader.hpp / cpp    # Loads motorcycle model and applies materials

├── SceneManager.hpp / cpp     # Sets up lights, grid, and track model

├── PowerUpManager.hpp / cpp   # Handles oil spills and speed boosts

├── UI.hpp / UI.cpp            # ImGui interface for sliders and buttons

I have also used Catch2 for testing basic functions like the MC and PowerUps

Build Instructions

This project uses CMake and vcpkg for dependencies.
1. Clone vcpkg
   git clone https://github.com/microsoft/vcpkg.gitcd vcpkg./bootstrap-vcpkg.sh
2. Install vcpkg
   ./vcpkg install bullet3
3. Change working directory
   Run Debug -> Edit Config -> Working Directory -> copy paste this - $PROJECT_DIR$


Future Improvements

Use Bullet’s Raycast Vehicle for better physics, currently not the best physics implementation.
Make the motorcycle align with the slope when going up/downhill
Add sound/more visual effetcs


AI-Usage

I have used AI for several aspects of my project. I have used a combination of ChatGPT, Copilot and CLions built-in AI assistant
AI have been used for these things
- Mathematical equations to correctly apply physics, rotations, etc
- Troubleshooting, when certain lines of code gave errors i would ask AI for assistance
- AI generated most of the ImgUI implementation, where i manually adjusted numbers and certain words
- I asked AI for how to implement Bullet physics (it was hard to find information manually online)
- I used CLion's built in AI for overall improvements and simplification of code
- AI has been used a decent amount when troubleshooting, with certain errors
    - Example: Process finished with exit code -1073741819 (0xC0000005) was an error i got several times.
- 