#include "pch.h"

#include "ConsoleLogger.h"

ConsoleLogger& ConsoleLogger::Instance()
{
    static ConsoleLogger logger;
    return logger;
}

void ConsoleLogger::SetLogLevel(const LogLevel& level) const
{
    spdlog::level::level_enum logLevel;
    switch (level)
    {
        case LogLevel::Info:
        {
            logLevel = spdlog::level::info;
        }
        break;
        case LogLevel::Warn:
        {
            logLevel = spdlog::level::warn;
        }
        break;
        case LogLevel::Error:
        {
            logLevel = spdlog::level::err;
        }
        break;
        case LogLevel::Off:
        default:
        {
            logLevel = spdlog::level::off;
        }
    }
    m_logger->set_level(logLevel);
}

ConsoleLogger::ConsoleLogger() : m_logger(
    spdlog::stdout_color_mt("ConsoleLogger"))
{
    spdlog::set_pattern("%^[%T] %v%$");
    SetLogLevel(LogLevel::Info);
}
