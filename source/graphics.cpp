#include "tightrope/pch.hpp"

#include "tightrope/graphics.hpp"

Graphics::~Graphics()
{
    SafeRelease(&m_d2dFactory);
    SafeRelease(&m_renderTarget);
}

void Graphics::Create(HWND handle)
{
    m_parentHandle = handle;

    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_INITIALIZATION_ERROR, "Failed to create D2D factory!");
    }

    _CreateRenderTarget();
}

void Graphics::_CreateRenderTarget()
{
    // create the render target for the window based on the window handle passed in
    RECT rect;
    GetClientRect(m_parentHandle, &rect); // grab the client rectangle to use for render target size

    D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

    HRESULT result = m_d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_parentHandle, size),
        &m_renderTarget
    );

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_INITIALIZATION_ERROR, "Failed to create render target!");
    }
}

void Graphics::Begin()
{
    // Begin drawing to the render target by clearing to a black screen
    m_renderTarget->BeginDraw();

    m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void Graphics::End()
{
    HRESULT result = m_renderTarget->EndDraw();

    if (result == D2DERR_RECREATE_TARGET)
    {
        SafeRelease(&m_renderTarget);

        _CreateRenderTarget();
    }
    else if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_DRAW_ERROR, "Direct2D draw end failed!");

        spdlog::error("HRESULT code for failure: {}", result);
    }
}