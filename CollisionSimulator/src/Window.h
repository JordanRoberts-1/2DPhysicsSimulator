#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace AppData
{
    inline float windowWidth = 1920.0f;
    inline float windowHeight = 1080.0f;
}

class Window
{
public:
	Window(const std::string& title);
    GLFWwindow* getGLFWWindow() const { return m_GlfwWindow; };
    inline float getWindowWidth() const { return AppData::windowWidth; }
    inline float getWindowHeight() const { return AppData::windowHeight; }
private:
    GLFWwindow* m_GlfwWindow;
};
