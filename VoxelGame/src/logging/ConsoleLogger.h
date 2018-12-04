#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

enum class LogLevel
{
    Off,
    Info,
    Warn,
    Error,
};

class ConsoleLogger
{
public:
    /* Public methods */
    static ConsoleLogger& Instance();

    template<typename... Args>
    void Info(const Args&... args)
    {
        m_logger->info(args...);
    }

    template<typename... Args>
    void Warn(const Args&... args)
    {
        m_logger->warn(args...);
    }

    template<typename... Args>
    void Error(const Args&... args)
    {
        m_logger->error(args...);
    }

    void SetLogLevel(const LogLevel& level) const;

private:
    /* Private methods */
    ConsoleLogger();

private:
    /* Private members */
    std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_LEVEL(level) ConsoleLogger::Instance().SetLogLevel(LogLevel::level)
#define LOG_INFO(...) ConsoleLogger::Instance().Info(__VA_ARGS__)
#define LOG_WARN(...) ConsoleLogger::Instance().Warn(__VA_ARGS__)
#define LOG_ERROR(...) ConsoleLogger::Instance().Error(__VA_ARGS__)
