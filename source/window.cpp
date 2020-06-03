#include "tightrope/pch.hpp"

#include "tightrope/window.hpp"

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Window::~Window()
{    
}

void Window::Create(uint32_t width, uint32_t height)
{
    m_width  = width;
    m_height = height;

#ifdef _WIN32
    WNDCLASS windowClass = {};

    windowClass.lpfnWndProc   = WindowProc;
    windowClass.hInstance     = GetModuleHandle(0);
    windowClass.lpszClassName = "tightrope";
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClass(&windowClass);

    // create a rectangle for resizing the window size to fit the full resolution
    RECT rect = { 0 };

    rect.right  = m_width;
    rect.bottom = m_height;

    AdjustWindowRectEx(
        &rect,
        WS_OVERLAPPEDWINDOW,
        FALSE,
        0
    );

    m_handle = CreateWindowEx(
        0,
        windowClass.lpszClassName,
        "tightrope",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        windowClass.hInstance,
        this
    );

    if (m_handle == NULL)
    {
        spdlog::error("Failed to create Win32 window!");
    }

    ShowWindow(m_handle, SW_SHOW);
    UpdateWindow(m_handle);
#endif
}

void Window::Update()
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_isCloseRequested = true;

            break;
        }

        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }
}

#ifdef _WIN32
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_CREATE:
    {
        if (!window)
        {
            window = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }

        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif