#include "exec.h"


std::string Exec::exec_Linux(const std::string &cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return StringOps::trim(result);
}


std::string Exec::getMimeType_Linux(const std::string &path)
{
    std::string path_trmdQuotes = StringOps::trimLeadingQuotationMarks(path);
    return StringOps::trim(exec_Linux("file --mime-type -b \"" + path_trmdQuotes + "\"")); // -b for brief -> ohne -b wird z.B. folgendes zurueckgegeben: $/home/hippo/Documents/exmpl.pdf: application/pdf
}

std::vector<std::string> Exec::getXDG_DATA_DIR()
{
    std::string xdg_data_dirs = "echo $XDG_DATA_DIRS";
    std::string xdg_pths = exec_Linux(xdg_data_dirs);
    return StringOps::split(xdg_pths, ":");
}
