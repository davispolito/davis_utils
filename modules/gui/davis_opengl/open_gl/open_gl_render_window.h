//
// Created by Davis Polito on 10/9/24.
//

#ifndef DAVIS_UTILS_OPEN_GL_RENDERER_H
#define DAVIS_UTILS_OPEN_GL_RENDERER_H
#include "open_gl_component.h"

namespace davis {
    struct OpenGlWrapper {
        OpenGlWrapper() : shaders(nullptr), display_scale(1.0f) {}

        juce::OpenGLContext context;
        Shaders *shaders;
        float display_scale;
    };
    class OpenGlRenderWindow : private juce::OpenGLRenderer {

    public:
        OpenGlRenderWindow();

        ~OpenGlRenderWindow();
        void setTopLevelParentComponent (juce::Component& topLevelComponent);

        void detachTopLevelParentComponent();

        OpenGlWrapper open_gl;

        std::vector<OpenGlComponent*> children;
    private:
        juce::Component* topLevelComponent = nullptr;
        void newOpenGLContextCreated() override;

        void renderOpenGL() override;

        void openGLContextClosing() override;
    }; //render window




}// davis

#endif //DAVIS_UTILS_OPEN_GL_RENDERER_H
