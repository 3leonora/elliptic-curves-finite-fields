#ifndef PROFILING_LOGGER_HPP
#define PROFILING_LOGGER_HPP

#include <string>
#include <iostream>
#include <chrono>
#include <format>

namespace profiling{
    // CLASS logger
    //
    // DESCRIPTION
    //      Provide logging and time measurement output
    class Logger {
        public:

        Logger(const std::string& name);

        // Log out text by piping into logger obj
        // Need to end with << std::endl!
        template<typename T>
        std::ostream& operator<<(const T& x) const;

        // Log out using std::format syntax
        template<typename... Args>
        void format(const std::format_string<Args...> fmt, Args&&... args) const;

        void reset(const std::string& label);

        std::string getTimeStampPfx(void) const;

        private:

        std::string _name;
        std::chrono::time_point<std::chrono::steady_clock> _reset_time;
        std::string _current_label;
    };

    template<typename T>
    std::ostream& Logger::operator<<(const T& x) const
    {
        return std::cout << getTimeStampPfx() << x;
    }

    template<typename... Args>
    void Logger::format(const std::format_string<Args...> fmt, Args&&... args) const
    {
        std::cout << getTimeStampPfx() << std::vformat(fmt.get(), std::make_format_args(args...)) << std::endl;
    }


}

#endif // PROFILING_LOGGER_HPP