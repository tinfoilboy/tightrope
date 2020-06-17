#pragma once

#include "tightrope/window.hpp"

class Tightrope {

public:
    
    Tightrope();

    void Run();

private:

    Window m_window;

    void _OnKeyDown(const KeyCode& typed);

    void _OnTyped(wchar_t typed);

};