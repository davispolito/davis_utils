include(CPM)

# download JUCE
CPMAddPackage(
  NAME juce
  GITHUB_REPOSITORY juce-framework/juce
  GIT_TAG 7.0.12
)

include(AddJUCEModules)

# OSX deployment target must be defined at a project level
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment target" FORCE)
project(davis_utils_examples VERSION 2.2.0)

# download CLAP extensions
CPMAddPackage(
    NAME clap-juce-extensions
    GITHUB_REPOSITORY free-audio/clap-juce-extensions
    GIT_TAG e72d59a870ab6dea156d4912cbd004b715fca5f7
)

include(SetupExamplePlugin)

# Treat warnings as errors
add_compile_options(
  $<$<CXX_COMPILER_ID:MSVC>:/WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
)

#add_subdirectory(SimpleEQ)

