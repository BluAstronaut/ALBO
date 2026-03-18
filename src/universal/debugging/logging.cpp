#include "universal/debugging/logging.h"
#include <fstream>
#include <utility>

class sqlite3;

namespace logging {
    template<typename... Args>
    void log(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args) {
        
        auto first_arg = std::get<0>(args);

        std::ofstream file(outputFile, std::ios::app);
        file  << "LOG: " << e << end;
        file.close();
    }
    
    template<typename... Args>
    void warn(const std::string& outputFile, const std::string& e, std::string end = "\n" Args... args) {

        auto first_arg = std::get<0>(args);

        std::ofstream file(outputFile, std::ios::app);
        file  << "WARNING: " << e << end;
        file.close();
    }

    template<typename... Args>
    void error(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args) {

        auto first_arg = std::get<0>(args);

        std::ofstream file(outputFile, std::ios::app);
        file  << "ERROR: " << e << end;
        file.close();
    }

    template<typename... Args>
    void fatal(const std::string& outputFile, const std::string& e, std::string end = "\n", Args... args) {

        auto first_arg = std::get<0>(args);
        
        std::ofstream file(outputFile, std::ios::app);
        file  << "FATAL: " << e << end;
        file.close();
        std::abort();
    }

}