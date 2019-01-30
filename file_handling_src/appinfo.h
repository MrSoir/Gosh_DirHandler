#ifndef APPINFO_H
#define APPINFO_H

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <stdexcept>
#include <array>
#include <limits.h>
#include <unistd.h>
#include <hash_fun.h>

#include "stringops.h"

class AppInfo
{
public:
    std::string name;
    std::string exec_cmnd;
    std::string icon_file_name;
    std::string icon_file_path;
    std::unordered_set<std::string> mime_types;

    AppInfo();
    AppInfo(const AppInfo& ai);

    AppInfo(std::string appInfoStr);

    std::string genExecCmnd(const std::vector<std::string>& args = std::vector<std::string>()) const;

    void addMimeTypesFromVector(const std::vector<std::string>& mts);

    std::string toString() const;
    bool operator==(const AppInfo& rhs);
};

namespace std
{
    template<> struct hash<AppInfo>
    {
        std::size_t operator()(const AppInfo& ai) const
        {
            return std::hash<std::string>()(ai.toString());
        }
    };
}

#endif // APPINFO_H
