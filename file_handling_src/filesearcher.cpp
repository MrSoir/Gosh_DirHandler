#include "filesearcher.h"

FileSearcher::FileSearcher(QObject *parent)
    : QObject(parent),
      m_key_word(std::string()),
      m_matched_paths(std::unordered_set<std::string>()),
      m_ord_matchedPaths(std::unordered_map<int, std::string>()),
      m_fileName_path(nullptr),
      m_enabled(false),
      m_focused_match_id(-1),
      m_focused_path(""),
      m_matchCount(0)
{
}

FileSearcher::~FileSearcher()
{
    m_fileName_path = nullptr; // aufgabe von FileManager dieses zu deleten

    m_matched_paths.clear();
    m_ord_matchedPaths.clear();
    m_key_word = "";
    m_focused_match_id = -1;
    m_focused_path = "";
    m_matchCount = 0;
}

bool FileSearcher::isSearched(const std::string& path) const
{
    return m_matched_paths.find(path) != m_matched_paths.end();
}
bool FileSearcher::isFocused(const std::string &path) const
{
    return m_focused_path != "" && path == m_focused_path;
}

void FileSearcher::processSearchables(std::vector<Searchable*>& searchables)
{
    for(auto it = searchables.begin(); it != searchables.end(); ++it)
    {
        (*it)->setSearched( isSearched((*it)->path()) );
        (*it)->setSearchFocused( isFocused((*it)->path()) );
    }
}

//-------------------------------------------------------

void FileSearcher::entriesChanged(std::unordered_map<std::string, std::string>* fileName_paths)
{
    m_fileName_path = fileName_paths;
    findMatches();
}

void FileSearcher::search(std::string key_word)
{
    key_word = StringOps::toLower(key_word);

    if(key_word == m_key_word)
    {
        // nichts machen
    }else{
        findMatches();
    }
}

void FileSearcher::clearSearch()
{
    if(m_key_word != "")
    {
        m_matched_paths.clear();
        m_ord_matchedPaths.clear();
        m_key_word = "";

        emit searchResultsChanged();
    }
}

void FileSearcher::enable()
{
    m_enabled = true;
}
void FileSearcher::disable()
{
    m_enabled = false;
}

void FileSearcher::focusNextMatch()
{
    int oldId = m_focused_match_id;

    if(++m_focused_match_id >= m_matchCount)
        m_focused_match_id = 0;

    m_focused_path = m_focused_match_id > -1 ? m_ord_matchedPaths[m_focused_match_id] : "";

    if(oldId != m_focused_match_id)
        emit searchResultsChanged();
}
void FileSearcher::focusPreviousMatch()
{
    int oldId = m_focused_match_id;

    if(--m_focused_match_id < 0)
        m_focused_match_id = m_matchCount == 0 ? 0 : std::max(m_matchCount - 1, 0);

    m_focused_path = m_focused_match_id > -1 ? m_ord_matchedPaths[m_focused_match_id] : "";

    if(oldId != m_focused_match_id)
        emit searchResultsChanged();
}

//--------------------------------------------------------

void FileSearcher::findMatches()
{
    m_matched_paths.clear();
    m_ord_matchedPaths.clear();
    m_focused_match_id = -1;
    m_focused_path = "";

    if(m_fileName_path)
    {
        int cntr = 0;
        for(auto it=m_fileName_path->begin(); it != m_fileName_path->end(); ++it)
        {
            const std::string& entry_name = it->first;
            if(StringOps::inStringIgnoreCase(entry_name, m_key_word))
            {
                m_matched_paths.insert( it->second );
                m_ord_matchedPaths[cntr++] = it->second;
            }
        }
        m_matchCount = static_cast<int>(m_matched_paths.size());
        m_focused_match_id = 0;
        m_focused_path = m_ord_matchedPaths[m_focused_match_id];
    }

    emit searchResultsChanged();
}



