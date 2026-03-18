#include <iostream>
#include <cstdlib>
#include <string>



namespace error {
    extern void log(const std::string& e, std::string end = "\n");
    extern void warn(const std::string& e, std::string end = "\n");
    extern void error(const std::string& e, std::string end = "\n");
    extern void fatal(const std::string& e, std::string end = "\n");

    
    // log, warning, error, fatal
}