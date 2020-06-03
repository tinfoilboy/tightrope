#pragma once

class Graphics
{

public:

    ~Graphics();

    void Create(HWND handle);

    void Begin();
    
    void End();

private:

    HWND m_parentHandle = nullptr;

    ID2D1Factory*          m_d2dFactory   = nullptr;
    ID2D1HwndRenderTarget* m_renderTarget = nullptr;

    void _CreateRenderTarget();
};