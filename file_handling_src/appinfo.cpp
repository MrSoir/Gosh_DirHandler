#include "appinfo.h"

AppInfo::AppInfo()
    : name(""),
      exec_cmnd(""),
      icon_file_name(""),
      icon_file_path(""),
      mime_types(std::unordered_set<std::string>())
{}
AppInfo::AppInfo(const AppInfo& ai)
    : name(ai.name),
      exec_cmnd(ai.exec_cmnd),
      icon_file_name(ai.icon_file_name),
      icon_file_path(ai.icon_file_path),
      mime_types(std::unordered_set<std::string>(ai.mime_types))
{}

AppInfo::AppInfo(std::string appInfoStr)
{
    const auto& infos = StringOps::split(appInfoStr, "__|__");
    name = infos.at(0);
    exec_cmnd = infos.at(1);
    icon_file_name = infos.at(2);
    icon_file_path = infos.at(3);
    addMimeTypesFromVector(StringOps::split(infos.at(3), ";"));
}

std::string AppInfo::genExecCmnd(const std::vector<std::string>& args) const
{
    std::string cmnd = exec_cmnd;
    cmnd = StringOps::replaceAll(cmnd, "%u", "");
    cmnd = StringOps::replaceAll(cmnd, "%U", "");
    cmnd = StringOps::replaceAll(cmnd, "%f", "");
    cmnd = StringOps::replaceAll(cmnd, "%F", "");
    cmnd = StringOps::trim(cmnd);

    std::string args_str = "";
    for(unsigned long i=0; i < args.size(); ++i)
    {
        args_str.append(" \"");
        args_str.append(StringOps::trimLeadingQuotationMarks(args.at(i)));
        args_str.append("\"");
    }
    cmnd.append(args_str);

    return StringOps::trim(cmnd);
}

void AppInfo::addMimeTypesFromVector(const std::vector<std::string>& mts)
{
    mime_types.clear();
    for(auto& mt: mts)
        mime_types.insert(mt);
}

std::string AppInfo::toString() const{
    std::string s;
    std::string sep("__|__");
    s.append(name);
    s.append(sep);
    s.append(exec_cmnd);
    s.append(sep);
    s.append(icon_file_name);
    s.append(sep);
    s.append(icon_file_path);
    s.append(sep);
    unsigned long i = 0;
    for(const auto& mt: mime_types)
    {
        s.append(mt);
        if(i++ < mime_types.size()-1)
            s.append(";");
    }
    return s;
}
bool AppInfo::operator==(const AppInfo& rhs) {
    if(mime_types.size() != rhs.mime_types.size())
        return false;

    for(const auto& ai: rhs.mime_types)
    {
        if(mime_types.find(ai) != mime_types.end())
            return false;
    }
    for(const auto& ai: mime_types)
    {
        if(rhs.mime_types.find(ai) != rhs.mime_types.end())
            return false;
    }
    return name == rhs.name
        && exec_cmnd == rhs.exec_cmnd
        && icon_file_name == rhs.icon_file_name
        && icon_file_path== rhs.icon_file_path;;
}
