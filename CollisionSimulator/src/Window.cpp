#include "Window.h"

Window::Window(const std::string& title)
    : m_GlfwWindow(nullptr)
{
    /* Create a windowed mode window and its OpenGL context */
    float windowWidth = AppData::windowWidth;
    float windowHeight = AppData::windowHeight;
    m_GlfwWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
    if (!m_GlfwWindow)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_GlfwWindow);
}