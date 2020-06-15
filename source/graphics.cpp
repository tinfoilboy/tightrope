#include "tightrope/pch.hpp"

#include "tightrope/graphics.hpp"

Graphics::~Graphics()
{
    SafeRelease(&m_d2dFactory);
    SafeRelease(&m_renderTarget);

    SafeRelease(&m_directWriteFactory);
    SafeRelease(&m_defaultFontFormat);
    SafeRelease(&m_whiteBrush);

    for (auto& layout : m_layouts)
    {
        SafeRelease(&layout.second.layout); // release the layout from memory on the DirectWrite side
    }
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

    _CreateDirectWrite();

    m_clearColor = D2D1::ColorF(0.2f, 0.2f, 0.2f, 1.0f);

    _CreateRenderTargetResources();
}

void Graphics::_CreateRenderTarget()
{
    // create the render target for the window based on the window handle passed in
    RECT rect;
    GetClientRect(m_parentHandle, &rect); // grab the client rectangle to use for render target size

    D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

    m_width  = size.width;
    m_height = size.height;

    HRESULT result = m_d2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_parentHandle, size),
        &m_renderTarget);

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_INITIALIZATION_ERROR, "Failed to create render target!");
    }
}

void Graphics::_CreateDirectWrite()
{
    HRESULT result = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_directWriteFactory),
        reinterpret_cast<IUnknown**>(&m_directWriteFactory));

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_INITIALIZATION_ERROR, "Failed to create DirectWrite factory!");
    }

    result = m_directWriteFactory->CreateTextFormat(
        L"Consolas",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        13,
        L"",
        &m_defaultFontFormat);

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_INITIALIZATION_ERROR, "Failed to create DirectWrite format!");
    }

    // align text at the start of the rectangle
    m_defaultFontFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    m_defaultFontFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void Graphics::_CreateRenderTargetResources()
{
    HRESULT result = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush);

    if (FAILED(result))
    {
        FatalError(FatalErrorCodes::GRAPHICS_DRAW_ERROR, "Failed to create white color brush!");
    }
}

void Graphics::Begin()
{
    // Begin drawing to the render target by clearing to a black screen
    m_renderTarget->BeginDraw();

    m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    m_renderTarget->Clear(m_clearColor);
}

void Graphics::TextLine(const std::wstring& text, float x, float y, float width, float height)
{
    if (width <= 0.0f)
    {
        width = static_cast<float>(m_width);
    }

    if (height <= 0.0f)
    {
        height = static_cast<float>(m_height);
    }

    auto& find = m_layouts.find(text);

    IDWriteTextLayout* textLayout = nullptr;

    // couldn't find a layout for the text provided, so create one
    if (find == m_layouts.end())
    {
        HRESULT result = m_directWriteFactory->CreateTextLayout(
            text.c_str(),
            text.size(),
            m_defaultFontFormat,
            width,
            height,
            &textLayout);

        if (FAILED(result))
        {
            FatalError(FatalErrorCodes::GRAPHICS_DRAW_ERROR, "Failed to create text layout for string!");
        }

        m_layouts.insert(std::make_pair(text, TextLayoutResource {
            textLayout, // layout
            true, // usedInUpdate
            0 // inactiveUpdates
        }));
    }
    else
    {
        textLayout = find->second.layout;

        find->second.usedInUpdate    = true; // used in this current update cycle, so reflect
        find->second.inactiveUpdates = 0; // reset the inactive counter as it should be zero
    }

    m_renderTarget->DrawTextLayout(
        D2D1::Point2F(x, y),
        textLayout,
        m_whiteBrush);
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

    // iterate through each layout and erase if necessary
    for (auto& itr = m_layouts.begin(); itr != m_layouts.end();)
    {
        if (!itr->second.usedInUpdate)
        {
            itr->second.inactiveUpdates++; // this resource wasn't used in an update, so increase the inactive counter
        }

        if (itr->second.inactiveUpdates >= m_maxKeepUpdates)
        {
            SafeRelease(&itr->second.layout); // release the layout from memory on the DirectWrite side

            itr = m_layouts.erase(itr); // erase the current item and set itr to the next item
        }
        else
        {
            ++itr; // advance the iterator
        }
    }
}