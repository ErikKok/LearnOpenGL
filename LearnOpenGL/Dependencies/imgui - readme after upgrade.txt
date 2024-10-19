To use glad with imgui, there will be a #include "imgui_impl_opengl3_loader.h" in imgui_impl_opengl3.cpp, you need to change the entire block, including the #define and #endif to:

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>
#endif

There is no need to have the imgui_impl_opengl3_loader.h file as we are already using glad.