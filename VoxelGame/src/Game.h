#pragma once

class Game
{
public:
	/* Public methods */
	void Start();

private:
    /* Private methods */
	void Run();

private:
	/* Private members */
	bool m_running = false;
};