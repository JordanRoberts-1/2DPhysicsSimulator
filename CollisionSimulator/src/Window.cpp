#include "Window.h"

Window::Window(const std::string& title, float windowWidth, float windowHeight)
    : glfwWindow(nullptr), w(windowWidth), h(windowHeight)
{
    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
    if (!glfwWindow)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);
}