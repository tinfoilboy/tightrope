#pragma once

#include "tightrope/graphics.hpp"

class Window {

public:

    ~Window();

    void Create(uint32_t width, uint32_t height);

    void Update();

    inline Graphics& GetGraphics()
    {
        return m_graphics;
    }

    inline bool IsCloseRequested()
    {
        return m_isCloseRequested;
    }
    
private:

    uint32_t m_width = 0;
    uint32_t m_height = 0;

    HWND m_handle = nullptr;

    Graphics m_graphics;

    bool m_isCloseRequested = false;

};