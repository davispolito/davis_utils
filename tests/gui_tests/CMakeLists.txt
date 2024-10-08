setup_juce_lib(gui_tests_lib
    juce::juce_gui_basics
        davis::davis_core

)

add_subdirectory(test_test)
if(DAVIS_BUILD_LIVE_GUI_TEST)
    add_subdirectory(live_gui_test)
endif()
