#include "tightrope/pch.hpp"

#include "tightrope/tightrope.hpp"

Tightrope::Tightrope()
    :
    m_window(Window())
{
}

void Tightrope::Run()
{
    spdlog::info("Starting tightrope...");

    // todo: implement config entries for width and height instead of hardcoding
    m_window.Create(1280, 720);

    while (!m_window.IsCloseRequested())
    {
        m_window.GetGraphics().Begin();
        m_window.GetGraphics().End();

        m_window.Update();
    }
}