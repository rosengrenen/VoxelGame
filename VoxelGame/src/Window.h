#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

class Window
{
public:
    /* Public methods */
    Window(int width, int height, const std::string& title);

    void Clear() const;
    void Display() const;
    void PollEvents() const;
    void Activate() const;
    void SetVerticalSync(bool on) const;
    void SetCursorVisible(bool visible) const;

    glm::vec2 GetMousePos() const;
    bool KeyDown(int key) const;

    glm::ivec2 GetSize() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    /* Private members */
    GLFWwindow* m_window;
};
