# ThreeppGP

Kandidat nummer: 10045


## IMPORTANT!

When launching the program it will say "Not responding/Svarer ikke" for a decent while. It might take up to a minute or so but it will run eventually. Just let it run!

### Overview

This is a simple motorcycle racing game inspired by MotoGP, built in C++ using:

Bullet Physics for realistic movement
Assimp for loading 3D models
ImGui for UI controls

The track is based on the MotoGP Argentina circuit

### Controls

W / A / S / D → Move the bike
SPACE → Active braking
UI Buttons → Reset position/Physics
UI Sliders → Adjust gravity, friction, and other physics settings in real-time


### Features

Dynamic motorcycle movement with acceleration, friction, leaning when turning. UI with sliders for physics functions. Two powerups, oilspill and speedboost. Catch2 test functions. And a map with collision integration.

### Build Instructions
1. Change working directory
   Run Debug -> Edit Config -> Working Directory -> copy paste this  `$PROJECT_DIR$`

### What works great

I'm happy with the power ups, the visual effect of the oil spill especially, the handling and controls of the motorcycle. 

### Future Improvements

Use Bullet’s Raycast Vehicle for better physics, currently not the best physics implementation.
Make the motorcycle align with the slope when going up/downhill
Add sound/more visual effetcs
The program takes a long time to load, and will be "not responding" for many seconds. I have not found a fix for this. 
I would like to understand pointers in a better way. 
Tried to include a pop-up logo at the beginning but was unable to do this.


### AI-Usage

I have used AI for several aspects of my project. I have used a combination of ChatGPT, Copilot and CLions built-in AI assistant
AI have been used for these things
- Mathematical equations to correctly apply physics, rotations, etc
- Troubleshooting, when certain lines of code gave errors i would ask AI for assistance
- AI generated most of the ImgUI implementation, where i manually adjusted numbers and certain words
- I asked AI for how to implement Bullet physics (it was hard to find help online)
- I used CLion's built in AI for overall improvements and simplification of code
- AI has been used a decent amount when troubleshooting, with certain errors
    - Example: Process finished with exit code -1073741819 (0xC0000005) was an error i got several times.
- 