#include <format>
#include "logger.hpp"

namespace profiling {

    Logger::Logger(const std::string& name)
    : _name(name)
    {
        reset("Logger start");
    }

    void Logger::reset(const std::string& label)
    {
        _current_label = label;
        _reset_time = std::chrono::steady_clock::now();
        *this << "RESET (" << label << ")" << std::endl;
    }

    std::string Logger::getTimeStampPfx(void) const
    {
        using MS = std::chrono::milliseconds;

        std::ostringstream oss;

        // First write a wall clock time string
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);
        oss << _name << ": " << std::put_time(std::localtime(&time), "%F %T (");

        // Add an DD:HH:MM:nnn offset timestamp to last reset time.
        auto ms = std::chrono::duration_cast<MS>(
            std::chrono::steady_clock::now() - _reset_time).count();
        auto secs = ms / 1000;
        auto minutes = secs / (60 * 1000);
        auto hours = minutes / 60;
        auto days = hours / 24;
        oss << std::format("{:02}:{:02}:{:02}:{:02}:{:03}",
            days,
            hours % 24,
            minutes % 60,
            secs % 60,
            ms % 1000) << ") ";

        return oss.str();
    }

}