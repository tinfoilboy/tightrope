#pragma once

class Window {

public:

    ~Window();

    void Create(uint32_t width, uint32_t height);

    void Update();

    inline bool IsCloseRequested()
    {
        return m_isCloseRequested;
    }
    
private:

    uint32_t m_width = 0;
    uint32_t m_height = 0;

#ifdef _WIN32
    HWND m_handle = nullptr;
#endif

    bool m_isCloseRequested = false;

};