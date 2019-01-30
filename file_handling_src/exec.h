#ifndef EXEC_H
#define EXEC_H

#include <string>
#include <vector>
#include "stringops.h"

namespace Exec
{
    std::string exec_Linux(const std::string& cmd);

    std::string getMimeType_Linux(const std::string& path);

    std::vector<std::string> getXDG_DATA_DIR();
}

#endif // EXEC_H
