#include "pch.h"
#include "Window.h"

Window::Window(int width, int height, const std::string& title)
{
    if (!glfwInit())
    {
        LOG_ERROR("GLFW could not be initialised");
        throw;
    }
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        LOG_ERROR("GLFW window could not be created");
        throw;
    }
    Activate();
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        LOG_ERROR("glad could not load OpenGL");
        throw;
    }
    LOG_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);
}

void Window::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Display() const
{
    glfwSwapBuffers(m_window);
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

void Window::Activate() const
{
    glfwMakeContextCurrent(m_window);
}
