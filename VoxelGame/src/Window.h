#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    /* Public methods */
    Window(int width, int height, const std::string& title);

    void Clear() const;
    void Display() const;
    void PollEvents() const;
    void Activate() const;

private:
    /* Private members */
    GLFWwindow* m_window;
};