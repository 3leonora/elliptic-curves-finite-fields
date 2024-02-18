#ifndef PROFILING_LOGGER_HPP
#define PROFILING_LOGGER_HPP

#include <string>
#include <iostream>
#include <chrono>

namespace profiling{
    // CLASS logger
    //
    // DESCRIPTION
    //      Provide logging and time measurement output
    class Logger {
        public:

        Logger(const std::string& name);

        template<typename T>
        std::ostream& operator<<(const T& x);

        void reset(const std::string& label);

        std::string getTimeStampPfx(void) const;

        private:

        std::string _name;
        std::chrono::time_point<std::chrono::steady_clock> _reset_time;
        std::string _current_label;
    };

    template<typename T>
    std::ostream& Logger::operator<<(const T& x)
    {
        return std::cout << getTimeStampPfx() << x;
    }


}

#endif // PROFILING_LOGGER_HPP