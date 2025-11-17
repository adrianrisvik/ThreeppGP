//
// Created by Lenovo on 15.11.2025.
//

#include "../include/Logo.hpp"
#include <algorithm>
#include <cstdint>

Logo::Logo(threepp::GLRenderer& renderer,
           threepp::Canvas& canvas,
           const std::string& imagePath,
           float durationSeconds,
           bool fadeOut)
    : renderer_(renderer),
      canvas_(canvas),
      duration_(durationSeconds),
      fadeOut_(fadeOut),
      startTime_(std::chrono::steady_clock::now()) {
    
    // Load texture
    threepp::TextureLoader loader;
    texture_ = loader.load(imagePath);
    
    if (texture_) {
        texture_->needsUpdate();
        // Get the OpenGL texture ID - this might need adjustment based on threepp's API
        // You may need to access it differently
        textureID_ = static_cast<unsigned int>(texture_->id);
    }
}

bool Logo::render() {
    if (isComplete_) {
        return false;
    }

    float elapsed = getElapsedTime();
    
    // Check if duration has elapsed
    if (elapsed >= duration_) {
        isComplete_ = true;
        return false;
    }

    // Calculate alpha for fade effect
    float alpha = calculateAlpha();
    
    // Get canvas size
    auto size = canvas_.size();
    
    // Calculate image dimensions (centered on screen)
    float imageWidth = 400.0f;
    float imageHeight = 400.0f;
    
    // Center position - call the methods with ()
    float sizeWidth = static_cast<float>(size.width());
    float sizeHeight = static_cast<float>(size.height());
    ImVec2 windowPos((sizeWidth - imageWidth) * 0.5f, 
                     (sizeHeight - imageHeight) * 0.5f);
    
    // Set up ImGui window for logo display
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(imageWidth, imageHeight));
    
    // Window flags to make it look like a splash screen
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | 
                             ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | 
                             ImGuiWindowFlags_NoScrollbar | 
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoInputs |
                             ImGuiWindowFlags_NoBackground;
    
    ImGui::Begin("Logo", nullptr, flags);
    
    if (texture_ && textureID_ != 0) {
        // Apply tint with alpha for fade effect
        ImVec4 tint(1.0f, 1.0f, 1.0f, alpha);
        
        // Display the image
        ImGui::Image(
            (void*)(intptr_t)textureID_,
            ImVec2(imageWidth, imageHeight),
            ImVec2(0, 1),  // UV coordinates (flipped for OpenGL)
            ImVec2(1, 0)

        );
    } else {
        // Fallback: display text if texture failed to load
        ImGui::SetCursorPos(ImVec2(imageWidth * 0.5f - 100, imageHeight * 0.5f));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, alpha), "ThreeppGP");
    }
    
    // Optional: Add "Press any key to skip" text
    ImGui::SetCursorPos(ImVec2(imageWidth * 0.5f - 80, imageHeight - 30));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, alpha * 0.5f), "Press any key to skip");
    
    ImGui::End();
    
    return true;
}

bool Logo::isComplete() const {
    return isComplete_;
}

void Logo::skip() {
    isComplete_ = true;
}

float Logo::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_);
    return elapsed.count() / 1000.0f;
}

float Logo::calculateAlpha() const {
    float elapsed = getElapsedTime();
    
    if (!fadeOut_) {
        return 1.0f;
    }
    
    // Fade in during first 0.5 seconds
    if (elapsed < 0.5f) {
        return elapsed / 0.5f;
    }
    
    // Stay at full opacity until last 0.5 seconds
    if (elapsed < duration_ - 0.5f) {
        return 1.0f;
    }
    
    // Fade out during last 0.5 seconds
    float fadeOutProgress = (duration_ - elapsed) / 0.5f;
    return std::max(0.0f, fadeOutProgress);
}