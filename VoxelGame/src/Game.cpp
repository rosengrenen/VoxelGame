#include "pch.h"
#include "Game.h"

#include "Shader.h"
#include "Timer.h"

Game::Game() : m_window(Window(800, 600, "Game"))
{
    m_frameTime = 1.0 / 60;
}

void Game::Start()
{
    if (m_running)
    {
        return;
    }

    Run();
}

void Game::Update() {}

void Game::Run()
{
    m_running = true;

    constexpr float vertices[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f,
    };

    glEnable(GL_CULL_FACE);

    Shader shader("res/shaders/triangle.vert.glsl", "res/shaders/triangle.frag.glsl");

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);


    Timer timer;
    Timer frameTimer;
    double unprocessedTime = 0;
    int frames = 0;

    while (m_running)
    {
        bool render = false;
        unprocessedTime += timer.Reset();

        while (unprocessedTime > m_frameTime)
        {
            unprocessedTime -= m_frameTime;
            render = true;

            m_window.PollEvents();

            Update();
        }
        if (render)
        {
            frames++;

            m_window.Clear();

            shader.Use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            m_window.Display();
        }
        if (frameTimer.Elapsed() > 1.0)
        {
            double time = frameTimer.Reset();
            LOG_INFO("FPS: {}", static_cast<double>(frames) / time);
            frames = 0;
        }
    }
}
