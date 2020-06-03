#pragma once

#ifdef _WIN32
template<class T>
inline void SafeRelease(T** toBeReleased)
{
    if (*toBeReleased != nullptr)
    {
        (*toBeReleased)->Release();
        (*toBeReleased) = nullptr;
    }
}

#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#endif

#include <spdlog/spdlog.h>
#include <memory>

enum class FatalErrorCodes : int32_t
{
    WINDOW_CREATION_ERROR         = -1,
    GRAPHICS_INITIALIZATION_ERROR = -2,
    GRAPHICS_DRAW_ERROR           = -3
};

inline static void FatalError(const FatalErrorCodes& code, const char* message)
{
    spdlog::error(message);

#ifdef _WIN32
    MessageBox(NULL, message, "Fatal Error", MB_OK | MB_ICONERROR);
#endif

    exit(static_cast<int32_t>(code));
}