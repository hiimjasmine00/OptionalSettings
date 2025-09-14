#pragma once
#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_WINDOWS
    #ifdef OPTIONAL_SETTINGS_EXPORTING
        #define OPTIONAL_SETTINGS_DLL __declspec(dllexport)
    #else
        #define OPTIONAL_SETTINGS_DLL __declspec(dllimport)
    #endif
#else
    #ifdef OPTIONAL_SETTINGS_EXPORTING
        #define OPTIONAL_SETTINGS_DLL __attribute__((visibility("default")))
    #else
        #define OPTIONAL_SETTINGS_DLL
    #endif
#endif
