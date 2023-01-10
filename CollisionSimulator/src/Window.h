#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(const std::string& title, int windowWidth, int windowHeight);
    GLFWwindow* getGLFWWindow();
private:
    GLFWwindow* glfwWindow;
};

Window::Window(const std::string& title, int windowWidth, int windowHeight)
    : glfwWindow(nullptr)
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

inline GLFWwindow* Window::getGLFWWindow()
{
    return glfwWindow;
}
