//
// Created by Davis Polito on 10/9/24.
//
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
namespace davis {

    class GlContextHolder
            : private juce::ComponentListener,
              private juce::OpenGLRenderer {
    public:
        static GlContextHolder *globalContextHolder;

        GlContextHolder(juce::Component &topLevelComponent)
                : parent(topLevelComponent) {
            jassert (globalContextHolder == nullptr);
            globalContextHolder = this;

            context.setRenderer(this);
            context.setContinuousRepainting(true);
            context.setComponentPaintingEnabled(true);
            context.attachTo(parent);
        }

        //==============================================================================
        // The context holder MUST explicitely call detach in their destructor
        void detach() {
            jassert (juce::MessageManager::getInstance()->isThisTheMessageThread());

            const int n = clients.size();
            for (int i = 0; i < n; ++i)
                if (juce::Component *comp = clients.getReference(i).c)
                    comp->removeComponentListener(this);

            context.detach();
            context.setRenderer(nullptr);
        }

        //==============================================================================
        // Clients MUST call unregisterOpenGlRenderer manually in their destructors!!
        void registerOpenGlRenderer(juce::Component *child) {
            jassert (juce::MessageManager::getInstance()->isThisTheMessageThread());

            if (dynamic_cast<juce::OpenGLRenderer *> (child) != nullptr) {
                if (findClientIndexForComponent(child) < 0) {
                    clients.add(Client(child,
                                       (parent.isParentOf(child) ? Client::State::running : Client::State::suspended)));
                    child->addComponentListener(this);
                }
            } else
                jassertfalse;
        }

        void unregisterOpenGlRenderer(juce::Component *child) {
            jassert (juce::MessageManager::getInstance()->isThisTheMessageThread());

            const int index = findClientIndexForComponent(child);

            if (index >= 0) {
                Client &client = clients.getReference(index);
                {
                    juce::ScopedLock stateChangeLock(stateChangeCriticalSection);
                    client.nextState = Client::State::suspended;
                }

                child->removeComponentListener(this);
                context.executeOnGLThread([this](juce::OpenGLContext &) {
                    checkComponents(false, false);
                }, true);
                client.c = nullptr;

                clients.remove(index);
            }
        }

        void setBackgroundColour(const juce::Colour c) {
            backgroundColour = c;
        }

        juce::OpenGLContext context;

    private:
        //==============================================================================
        void checkComponents(bool isClosing, bool isDrawing) {
            juce::Array<juce::Component *> initClients, runningClients;

            {
                juce::ScopedLock arrayLock(clients.getLock());
                juce::ScopedLock stateLock(stateChangeCriticalSection);

                const int n = clients.size();

                for (int i = 0; i < n; ++i) {
                    Client &client = clients.getReference(i);
                    if (client.c != nullptr) {
                        Client::State nextState = (isClosing ? Client::State::suspended : client.nextState);

                        if (client.currentState == Client::State::running &&
                            nextState == Client::State::running)
                            runningClients.add(client.c);
                        else if (client.currentState == Client::State::suspended &&
                                 nextState == Client::State::running)
                            initClients.add(client.c);
                        else if (client.currentState == Client::State::running &&
                                 nextState == Client::State::suspended) {
                            dynamic_cast<juce::OpenGLRenderer *> (client.c)->openGLContextClosing();
                        }

                        client.currentState = nextState;
                    }
                }
            }

            for (int i = 0; i < initClients.size(); ++i)
                dynamic_cast<juce::OpenGLRenderer *> (initClients.getReference(i))->newOpenGLContextCreated();

            if (runningClients.size() > 0 && isDrawing) {
                const float displayScale = static_cast<float> (context.getRenderingScale());
                const juce::Rectangle<int> parentBounds = (parent.getLocalBounds().toFloat() *
                                                           displayScale).getSmallestIntegerContainer();

                for (int i = 0; i < runningClients.size(); ++i) {
                    juce::Component *comp = runningClients.getReference(i);

                    juce::Rectangle<int> r = (parent.getLocalArea(comp, comp->getLocalBounds()).toFloat() *
                                              displayScale).getSmallestIntegerContainer();
                    juce::gl::glViewport((GLint) r.getX(),
                               (GLint) parentBounds.getHeight() - (GLint) r.getBottom(),
                               (GLsizei) r.getWidth(), (GLsizei) r.getHeight());
                    juce::OpenGLHelpers::clear(backgroundColour);

                    dynamic_cast<juce::OpenGLRenderer *> (comp)->renderOpenGL();
                }
            }
        }

        //==============================================================================
        void componentParentHierarchyChanged(juce::Component &component) override {
            if (Client * client = findClientForComponent(&component)) {
                juce::ScopedLock stateChangeLock(stateChangeCriticalSection);

                client->nextState = (parent.isParentOf(&component) && component.isVisible() ? Client::State::running
                                                                                            : Client::State::suspended);
            }
        }

        void componentVisibilityChanged(juce::Component &component) override {
            if (Client * client = findClientForComponent(&component)) {
                juce::ScopedLock stateChangeLock(stateChangeCriticalSection);

                client->nextState = (parent.isParentOf(&component) && component.isVisible() ? Client::State::running
                                                                                            : Client::State::suspended);
            }
        }

        void componentBeingDeleted(juce::Component &component) override {
            const int index = findClientIndexForComponent(&component);

            if (index >= 0) {
                Client &client = clients.getReference(index);

                // You didn't call unregister before deleting this component
                jassert (client.nextState == Client::State::suspended);
                client.nextState = Client::State::suspended;

                component.removeComponentListener(this);
                context.executeOnGLThread([this](juce::OpenGLContext &) {
                    checkComponents(false, false);
                }, true);

                client.c = nullptr;

                clients.remove(index);
            }
        }

        //==============================================================================
        void newOpenGLContextCreated() override {
            checkComponents(false, false);
        }

        void renderOpenGL() override {
            juce::OpenGLHelpers::clear(backgroundColour);
            checkComponents(false, true);
        }

        void openGLContextClosing() override {
            checkComponents(true, false);
        }

        //==============================================================================
        juce::Component &parent;

        struct Client {
            enum class State {
                suspended,
                running
            };

            Client(juce::Component *comp, State nextStateToUse = State::suspended)
                    : c(comp), currentState(State::suspended), nextState(nextStateToUse) {}


            juce::Component *c = nullptr;
            State currentState = State::suspended, nextState = State::suspended;
        };

        juce::CriticalSection stateChangeCriticalSection;
        juce::Array<Client, juce::CriticalSection> clients;

        //==============================================================================
        int findClientIndexForComponent(juce::Component *comp) const {
            const int n = clients.size();
            for (int i = 0; i < n; ++i)
                if (comp == clients.getReference(i).c)
                    return i;

            return -1;
        }

        Client *findClientForComponent(juce::Component *comp) const {
            const int index = findClientIndexForComponent(comp);
            if (index >= 0)
                return &clients.getReference(index);

            return nullptr;
        }

        //==============================================================================
        juce::Colour backgroundColour{juce::Colours::black};
    };
}