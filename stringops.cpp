#include "stringops.h"


std::string StringOps::path_separator()
{
    return QString(QDir::separator()).toStdString();
}
std::string StringOps::new_line()
{
    return "\n";
}


// std::toLower (thus FileSearcher::toLower) does NOT transform UMLAUTE from capital to lowercase!!!
std::string StringOps::toLower(const std::string& s)
{
    std::string sc = std::string(s);
    std::transform(sc.begin(), sc.end(), sc.begin(),
                   [](unsigned char c){ return std::tolower(c); }
                  );
    return sc;
}
bool StringOps::startsWithIgnoreCase(const std::string& src, const std::string& start)
{
    return startsWith(toLower(src), toLower(start));
}
bool StringOps::startsWith(const std::string &s, const std::string& start)
{
    return s.length() >= start.length() &&
            static_cast<bool>(s.substr(0, start.length()) == start);
}

bool StringOps::endsWithIgnoreCase(const std::string& src, const std::string& end)
{
    return endsWith(toLower(src), toLower(end));
}
bool StringOps::endsWith(const std::string &s, const std::string &end)
{
    return s.length() >= end.length() &&
            static_cast<bool>(s.substr(s.length() - end.length()) == end);
}

bool StringOps::inStringIgnoreCase(const std::string& ref, const std::string& key)
{
    return ref.size() >= key.size()
            && toLower(ref).find(toLower(key)) != std::string::npos;
}

bool StringOps::equalsIgnoreCase(const std::string s1, const std::string s2)
{
    return s1.size() == s2.size() &&
            toLower(s1) == toLower(s2);
}

// replaceAll: ersetzt alle toRepl im string s durch rplcmnt
std::string StringOps::replaceAll(const std::string& s, const std::string& toRepl, const std::string& rplcmnt)
{
    std::string rs = s;

    auto p = rs.find(toRepl);
    while(p != std::string::npos)
    {
        rs.replace(p, toRepl.length(), rplcmnt);
        p = rs.find(toRepl);
    }
    return rs;
}

template<char delimiter>
class WordDelimitedBy : public std::string
{};
std::vector<std::string> StringOps::split(const std::string& s, const std::string& sep)
{
    std::vector<std::string> splts;
    std::size_t p1 = 0;
    auto p2 = s.find(sep);
    while(p2 != std::string::npos)
    {
        auto sub = s.substr(p1, p2-p1);
        if(sub != "")
            splts.push_back(sub);
        p1 = p2 + sep.length();
        p2 = s.find(sep, p2 + 1);
    }
    if(p1 != std::string::npos){
        auto sub = s.substr(p1);
        if(sub != "")
            splts.push_back(sub);
    }
    return splts;
}

std::string StringOps::right(const std::string& s, std::size_t n)
{
    return s.substr(s.length()-n, n);
}
std::string StringOps::left(const std::string &s, std::size_t n)
{
    return s.substr(0, n);
}


// trim from start
std::string StringOps::ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    return s;
}
// trim from end
std::string StringOps::rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}
// trim from both ends
std::string StringOps::trim(std::string s) {
    s = ltrim(s);
    return rtrim(s);
}

std::string StringOps::trimLeadingQuotationMarks(std::string s)
{
    if(s.size() > 2)
    {
        if(s[0] == '\"' && s[s.size()-1] == '\"')
        {
            s = right(s, s.size()-1);
            s = left(s, s.size()-1);
        }

    }
    return s;
}
