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
    SetVerticalSync(false);
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

void Window::SetVerticalSync(bool on) const
{
    glfwSwapInterval(on ? 1 : 0);
}

void Window::SetCursorVisible(bool visible) const
{
    glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

glm::vec2 Window::GetMousePos() const
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return { x, y };
}

bool Window::KeyDown(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

glm::ivec2 Window::GetSize() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return { width, height };
}

int Window::GetWidth() const
{
    return GetSize().x;
}

int Window::GetHeight() const
{
    return GetSize().y;
}
