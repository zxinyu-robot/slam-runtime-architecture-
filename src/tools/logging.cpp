#include "tools/logging.h"

#include <iostream>

namespace slam::tools {
void info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}
}
