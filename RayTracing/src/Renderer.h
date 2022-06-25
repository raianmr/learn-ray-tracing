#pragma once

#include "Walnut/Image.h"

#include <glm/glm.hpp>
#include <memory>


class Renderer
{
public:
    Renderer() = default;

    void OnResize(uint32_t width, uint32_t height);
    void Render();
    auto GetFinalImage() const -> std::shared_ptr<Walnut::Image> { return m_FinalImage; }
    
private:
    std::shared_ptr<Walnut::Image> m_FinalImage;
    uint32_t* m_ImageData = nullptr;

    auto PerPixel(glm::vec2 coord) -> uint32_t;
};
