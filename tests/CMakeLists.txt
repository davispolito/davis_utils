include(SubprojectVersion)
include(EnableCoverageFlags)
include(SetupJuceTest)
include(SetupCatchTest)
include(SetupLib)

message(STATUS "Configuring tests for davis_utils")

# set up JUCE
option(DAVIS_BUILD_LIVE_GUI_TEST "Build live GUI test app" OFF)
if(NOT DAVIS_BUILD_LIVE_GUI_TEST)
    message(STATUS "Live GUI test is not being built... skipping juceaide!")
    set(JUCE_MODULES_ONLY ON CACHE INTERNAL "Only configure the JUCE modules")
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/../../JUCE JUCE)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

# Set up JUCE modules
include(AddJUCEModules)


# include CPM
include(CPM)
CPMAddPackage("gh:catchorg/Catch2@3.2.1")
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)

option(CODE_COVERAGE "Enable coverage reporting" OFF)


option(DAVIS_CODE_QUALITY_CHECKS "Configure code quality checks for davis modules" OFF)
if(DAVIS_CODE_QUALITY_CHECKS)
    include(SetupCodeQuality)
endif()

option(DAVIS_ENABLE_STATIC_TESTS "Configure compile-time static tests for davis modules" OFF)
if(DAVIS_ENABLE_STATIC_TESTS)
    add_subdirectory(static_tests)
endif()
add_subdirectory(gui_tests)