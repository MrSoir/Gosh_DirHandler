#ifndef STRINGOPS_H
#define STRINGOPS_H

#include <QDir>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <locale>

namespace StringOps
{

    std::string path_separator();
    std::string new_line();

    // std::toLower (thus FileSearcher::toLower) does NOT transform UMLAUTE from capital to lowercase!!!
    std::string toLower(const std::string& s);

    bool startsWithIgnoreCase(const std::string& src, const std::string& start);
    bool startsWith(const std::string& s, const std::string& end);
    bool endsWith(const std::string& s, const std::string& end);
    bool endsWithIgnoreCase(const std::string& src, const std::string& end);

    bool inStringIgnoreCase(const std::string& ref, const std::string& key);

    bool equalsIgnoreCase(const std::string s1, const std::string s2);

    std::string replaceAll(const std::string& s, const std::string& toRepl, const std::string& rplcmnt);

    std::vector<std::string> split(const std::string& s, const std::string& sep);

    std::string right(const std::string& s, std::size_t n);
    std::string left(const std::string& s, std::size_t n);

    std::string ltrim(std::string s);
    std::string rtrim(std::string s);
    std::string trim(std::string s);


    std::string trimLeadingQuotationMarks(std::string s);
}

#endif // STRINGOPS_H
