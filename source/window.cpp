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

    // create the graphics device for drawing to the window
    m_graphics.Create(m_handle);
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
    case WM_KEYDOWN:
    {
        KeyCode code;

        switch (wParam)
        {
        case VK_RETURN:
            code = KeyCode::ENTER;
            break;
        case VK_LEFT:
            code = KeyCode::LEFT_ARROW;
            break;
        case VK_RIGHT:
            code = KeyCode::RIGHT_ARROW;
            break;
        case VK_UP:
            code = KeyCode::UP_ARROW;
            break;
        case VK_DOWN:
            code = KeyCode::DOWN_ARROW;
            break;
        case VK_ESCAPE:
            code = KeyCode::ESCAPE;
            break;
        default:
            code = KeyCode::UNKNOWN;
            break;
        }

        (window->GetKeyDownCallback())(code);

        break;
    }
    case WM_CHAR:
    {
        (window->GetTypedCallback())(static_cast<wchar_t>(wParam));

        break;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}