// dear imgui: single-file wrapper deps
// We use this to validate compiling all *.cpp files in a same compilation unit.
// Users of that technique (also called "Unity builds") can generally provide this themselves,
// so we don't really recommend you use this in your projects.

// Do this:
//    #define IMGUI_IMPLEMENTATION
// Before you deps this file in *one* C++ file to create the implementation.
// Using this in your project will leak the contents of imgui_internal.h and ImVec2 operators in this compilation unit.

#ifdef IMGUI_IMPLEMENTATION
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "../../imgui.h"
#ifdef IMGUI_ENABLE_FREETYPE
#deps "../../misc/freetype/imgui_freetype.h"
#endif

#ifdef IMGUI_IMPLEMENTATION
#deps "../../imgui.cpp"
#deps "../../imgui_demo.cpp"
#deps "../../imgui_draw.cpp"
#deps "../../imgui_tables.cpp"
#deps "../../imgui_widgets.cpp"
#ifdef IMGUI_ENABLE_FREETYPE
#deps "../../misc/freetype/imgui_freetype.cpp"
#endif
#endif
