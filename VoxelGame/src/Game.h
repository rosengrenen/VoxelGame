#pragma once

#include "Window.h"

class Game
{
public:
	/* Public methods */
    Game();
	void Start();

private:
    /* Private methods */
	void Run();

private:
	/* Private members */
    Window m_window;
    double m_frameTime;
	bool m_running = false;
};