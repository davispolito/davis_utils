//
// Created by Davis Polito on 10/9/24.
//

#include "open_gl_render_window.h"

namespace davis {
    OpenGlRenderWindow::OpenGlRenderWindow()
    {

    }

    void OpenGlRenderWindow::setTopLevelParentComponent (juce::Component& topLevelComponent)
    {
        this->topLevelComponent = &topLevelComponent;
        open_gl.context.attachTo (topLevelComponent);
    }

    void OpenGlRenderWindow::detachTopLevelParentComponent ()
    {
        open_gl.context.detach();
    }
} // davis