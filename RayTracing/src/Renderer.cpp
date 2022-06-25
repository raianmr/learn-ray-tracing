#include "Walnut/Random.h"

#include "Renderer.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (m_FinalImage != nullptr)
    {
        if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
        {
            // no resize needed
            return;
        }

        m_FinalImage->Resize(width, height);
    }
    else
    {
        m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] m_ImageData;
    m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
    auto width = m_FinalImage->GetWidth();
    auto height = m_FinalImage->GetHeight();

    auto f32 = [](auto num) { return static_cast<float>(num); };

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            glm::vec2 coord = {f32(x) / f32(width), f32(y) / f32(height)};
            coord = coord * 2.0f - 1.0f; // (-1 to 1)
            m_ImageData[x + y * width] = PerPixel(coord);
        } 
    }

    m_FinalImage->SetData(m_ImageData);
}

auto Renderer::PerPixel(glm::vec2 coord) -> uint32_t
{
    auto ui8 = [](auto num) { return static_cast<uint8_t>(num); };
    
    auto r = ui8(coord.x * 255.0f);
    auto g = ui8(coord.y * 255.0f);

    auto radius = 0.5f;
    glm::vec3 rayOrigin{0.0f, 0.0f, 2.0f};
    glm::vec3 rayDirection{coord.x, coord.y, -1.0f};
    // rayDirection = glm::normalize(rayDirection);

    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = radius
    // t = hit distance

    auto sq = [](auto num) { return num * num; };

    auto a = glm::dot(rayDirection, rayDirection);
    auto b = 2.0f * glm::dot(rayOrigin, rayDirection);
    auto c = glm::dot(rayOrigin, rayOrigin) - sq(radius);

    // quadratic formula discriminant
    // b^2 - 4ac
    
    if (auto disc = sq(b) - 4.0f * a * c; disc >= 0.0f)
    {
        return 0xff00ffcc;
    }

    return 0xff303030;
}
