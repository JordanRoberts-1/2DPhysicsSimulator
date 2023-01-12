#include "Window.h"

Window::Window(const std::string& title, float windowWidth, float windowHeight)
    : m_GlfwWindow(nullptr), w(windowWidth), h(windowHeight)
{
    /* Create a windowed mode window and its OpenGL context */
    m_GlfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
    if (!m_GlfwWindow)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_GlfwWindow);
}