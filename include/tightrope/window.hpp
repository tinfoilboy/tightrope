#pragma once

#include "tightrope/graphics.hpp"

enum class KeyCode : uint8_t
{

    UNKNOWN     = 0,
    UP_ARROW    = 1,
    DOWN_ARROW  = 2,
    LEFT_ARROW  = 3,
    RIGHT_ARROW = 4,
    ESCAPE      = 5,
    ENTER       = 6
};

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

    inline void SetTypedCallback(std::function<void(wchar_t)> callback)
    {
        this->m_typedCallback = callback;
    }

    inline std::function<void(wchar_t)>& GetTypedCallback()
    {
        return m_typedCallback;
    }

    inline void SetKeyDownCallback(std::function<void(const KeyCode&)> callback)
    {
        this->m_keyDownCallback = callback;
    }

    inline std::function<void(const KeyCode&)>& GetKeyDownCallback()
    {
        return m_keyDownCallback;
    }
    
private:

    uint32_t m_width = 0;
    uint32_t m_height = 0;

    HWND m_handle = nullptr;

    Graphics m_graphics;

    std::function<void(const KeyCode&)> m_keyDownCallback;

    std::function<void(wchar_t)> m_typedCallback;

    bool m_isCloseRequested = false;

};