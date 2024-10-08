//
// Created by Davis Polito on 2/1/24.
//

#ifndef ELECTROSYNTH2_MAIN_SECTION_H
#define ELECTROSYNTH2_MAIN_SECTION_H
#include "synth_section.h"


class MainSection : public SynthSection
{
public:
    class Listener {
    public:
        virtual ~Listener() { }

        //virtual void showAboutSection() = 0;
    };

    MainSection(juce::ValueTree v, juce::UndoManager &um, OpenGlWrapper &open_gl, SynthGuiData * data);

    void paintBackground(Graphics& g) override;
    void resized() override;
//    void reset() override;
//
//    void buttonClicked(Button* clicked_button) override;
//
//    void notifyChange();
//    void notifyFresh();

    void addListener(Listener* listener) { listeners_.push_back(listener); }
private:
    juce::ValueTree v;
    juce::UndoManager &um;


    std::vector<Listener*> listeners_;

};

#endif //ELECTROSYNTH2_MAIN_SECTION_H
