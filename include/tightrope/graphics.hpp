#pragma once

struct TextLayoutResource
{

    IDWriteTextLayout* layout = nullptr;
    
    /*
    Whether the resource was used in the current update (before call to End).
    */
    bool usedInUpdate = false;

    /*
    An amount of calls to End where the resource has not been used.

    If this number is equal to or exceeds the maximum value, the resource is removed and freed.
    */
    size_t inactiveUpdates = 0;

    TextLayoutResource() = default;

    TextLayoutResource(IDWriteTextLayout* layout, bool usedInUpdate, size_t inactiveUpdates)
        :
        layout(layout),
        usedInUpdate(usedInUpdate),
        inactiveUpdates(inactiveUpdates)
    {
    }

};

class Graphics
{

public:

    ~Graphics();

    void Create(HWND handle);

    /*
    Start drawing a single frame to the main render target.
    */
    void Begin();

    /*
    Draw a single text element to the main render target at the origin specified.

    Optionally, pass in a width and height to layout text in a specified size. Defaults to the window width and height.
    */
    void TextLine(const std::wstring& text, float x, float y, float width=0.0f, float height=0.0f);
    
    /*
    Cease drawing to the render target and initiate display to the window.
    */
    void End();

private:

    HWND m_parentHandle = nullptr;

    uint32_t m_width  = 0;
    uint32_t m_height = 0;

    /*
    The maximum amount of calls to End that a resource should be kept around after not being used, if that resource is
    using a keep-around scheme, such as text layouts.
    */
    uint32_t m_maxKeepUpdates = 60;

    D2D1::ColorF m_clearColor = D2D1::ColorF(D2D1::ColorF::White);

    ID2D1Factory*          m_d2dFactory   = nullptr;
    ID2D1HwndRenderTarget* m_renderTarget = nullptr;
    ID2D1SolidColorBrush*  m_whiteBrush   = nullptr;

    IDWriteFactory*    m_directWriteFactory = nullptr;
    IDWriteTextFormat* m_defaultFontFormat  = nullptr;
    
    /*
    A map of active text layouts to strings that represent the text layouts.
    */
    std::unordered_map<std::wstring, TextLayoutResource> m_layouts;

    /*
    Create the main render target that is drawn to.
    */
    void _CreateRenderTarget();

    /*
    Create assets related to DirectWrite.
    */
    void _CreateDirectWrite();

    /*
    Create resources such as brushes that depend on the render target for creation.
    */
    void _CreateRenderTargetResources();

};