#include <iostream>
#include <cstdlib>
#include <string>

namespace logging {

    template<typename... Args>
    extern void log(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args);
    
    template<typename... Args>
    extern void warn(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args);
    
    template<typename... Args>
    extern void error(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args);
    
    template<typename... Args>
    extern void fatal(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args);

    
    // log, warning, error, fatal
}