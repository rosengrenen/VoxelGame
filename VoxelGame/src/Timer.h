#pragma once

class Timer
{
public:
    /* Public methods */
    Timer();
    double Elapsed() const;
    double Reset();

private:
    /* Private members */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_timestamp;
};
