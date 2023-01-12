#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(const std::string& title, float windowWidth, float windowHeight);
    GLFWwindow* getGLFWWindow() const { return m_GlfwWindow; };
    inline float getWindowWidth() const { return w; }
    inline float getWindowHeight() const { return h; }
private:
    GLFWwindow* m_GlfwWindow;
    float w, h;
};
