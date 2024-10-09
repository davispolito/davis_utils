setup_juce_lib(gui_tests_lib
    juce::juce_gui_basics
        davis::davis_core
        davis::davis_look_and_feel

)

add_subdirectory(test_test)
add_subdirectory(look_and_feel_tests)
add_subdirectory(opengl_tests)
if(DAVIS_BUILD_LIVE_GUI_TEST)
    add_subdirectory(live_gui_test)
endif()
