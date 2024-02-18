#include "timemeas.hpp"

#include <iostream>
#include <iomanip>
#include <format>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdint>
#include <cmath>


std::string profiling::now()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%F %T");
    return oss.str();
}