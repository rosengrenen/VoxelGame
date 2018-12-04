#include "pch.h"

#include "Game.h"

#include "Timer.h"

void Game::Start()
{
    if (m_running)
    {
        return;
    }

    Run();
}

void Game::Run()
{
    m_running = true;

    constexpr double frameTime = 1.0 / 60.0;

    Timer timer;
    Timer frameTimer;
    double unprocessedTime = 0;
    int frames = 0;

    while (m_running)
    {
        bool render = false;
        unprocessedTime += timer.Reset();

        while (unprocessedTime > frameTime)
        {
            unprocessedTime -= frameTime;
            render = true;
        }
        if (render)
        {
            frames++;
        }
        if (frameTimer.Elapsed() > 1.0)
        {
            double time = frameTimer.Reset();
            LOG_INFO("FPS: {}", static_cast<double>(frames) / time);
            frames = 0;
        }
    }
}
