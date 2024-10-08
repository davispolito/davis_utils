//
// Created by Davis Polito on 12/8/23.
//
#pragma once
#if JUCE_MODULE_AVAILABLE_juce_core
#include <juce_core/juce_core.h>
#endif
#if JUCE_MODULE_AVAILABLE_juce_data_structures
#include <juce_data_structures/juce_data_structures.h>
#endif
namespace davis::IDs
{
#define DECLARE_ID(name) const juce::Identifier name (#name);

    DECLARE_ID (TREE)
    DECLARE_ID (CONTROLS)
    DECLARE_ID (INPUT)
    DECLARE_ID (OUTPUT)
    DECLARE_ID (PIANO)
    DECLARE_ID (CONNECTIONS)
    DECLARE_ID (PREPARATIONS)

    DECLARE_ID (name)

DECLARE_ID (PREPARATION)
    DECLARE_ID (id)
    DECLARE_ID (type)
    DECLARE_ID (x)
    DECLARE_ID (y)
    DECLARE_ID (height)
    DECLARE_ID (width)

    DECLARE_ID (CONNECTION)
    DECLARE_ID (uuid)
}

#undef DECLARE_ID
    inline juce::ValueTree createUuidProperty (juce::ValueTree& v)
    {
        if (! v.hasProperty (davis::IDs::uuid))
            v.setProperty (davis::IDs::uuid, juce::Uuid().toString(), nullptr);

        return v;
    }


