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

    m_window.SetTypedCallback(std::bind(&Tightrope::_OnTyped, this, std::placeholders::_1));
    m_window.SetKeyDownCallback(std::bind(&Tightrope::_OnKeyDown, this, std::placeholders::_1));

    while (!m_window.IsCloseRequested())
    {
        Graphics& context = m_window.GetGraphics();

        context.Begin();
        context.TextLine(
            L"int main(int argc, char* argv[]) {",
            12.0f,
            12.0f);
        context.TextLine(
            L"    return 0;",
            12.0f,
            36.0f);
        context.TextLine(
            L"}",
            12.0f,
            60.0f);
        context.End();

        m_window.Update();
    }
}

void Tightrope::_OnKeyDown(const KeyCode& typed)
{
    if (typed == KeyCode::ENTER)
    {
        spdlog::info("Got newline from typed!");

        return;
    }
}

void Tightrope::_OnTyped(wchar_t typed)
{
    spdlog::info("Got character {} from typed!", static_cast<char>(typed));
}