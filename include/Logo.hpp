//
// Created by Lenovo on 15.11.2025.
//

#ifndef THREEPPGP_LOGO_HPP
#define THREEPPGP_LOGO_HPP
#pragma once
#include <threepp/threepp.hpp>
#include <imgui.h>
#include <chrono>
#include <string>
#include <memory>

class Logo {
public:
    Logo(threepp::GLRenderer& renderer,
         threepp::Canvas& canvas,
         const std::string& imagePath,
         float durationSeconds = 6.0f,
         bool fadeOut = true);

    bool render();

    bool isComplete() const;

    void skip();

private:
    threepp::GLRenderer &renderer_;
    threepp::Canvas& canvas_;  // Changed to reference
    std::shared_ptr<threepp::Texture> texture_;

    unsigned int textureID_{0};  // Changed from GLuint
    float duration_;
    bool fadeOut_;
    bool isComplete_{false};

    std::chrono::steady_clock::time_point startTime_;

    float getElapsedTime() const;

    float calculateAlpha() const;


};


#endif //THREEPPGP_LOGO_HPP