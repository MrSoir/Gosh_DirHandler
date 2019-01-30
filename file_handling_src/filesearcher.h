#ifndef FILESEARCHER_H
#define FILESEARCHER_H

#include <QObject>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "stringops.h"

#include "searchable.h"

//class Searchable: public QObject{
//    Q_OBJECT
//public:
//    virtual ~Searchable(){}
//    virtual void setSearched(bool searched) = 0;
//    virtual void setSearchFocused(bool focused) = 0;
//    virtual std::string path() const = 0;
//};

class FileSearcher : public QObject
{
    Q_OBJECT
public:
    explicit FileSearcher(QObject *parent = nullptr);
    ~FileSearcher();

    bool isSearched(const std::string& path) const;
    bool isFocused(const std::string& path) const;

    void processSearchables(std::vector<Searchable*>& searchables);
signals:
    void searchResultsChanged();
public slots:
    void entriesChanged(std::unordered_map<std::string, std::string>* fileName_paths);
    void search(std::string key_word);
    void clearSearch();

    void enable();
    void disable();

    void focusNextMatch();
    void focusPreviousMatch();
private:
    void findMatches();

    std::string m_key_word; // string that is searched for
    std::unordered_set<std::string> m_matched_paths; // all paths, which's fileNames contain the m_key_word
    std::unordered_map<int, std::string> m_ord_matchedPaths;

//    std::unordered_set<std::string> m_paths; // all paths
    std::unordered_map<std::string, std::string>* m_fileName_path; // map: fileName -> path

    bool m_enabled;
    int m_focused_match_id;
    std::string m_focused_path;
    int m_matchCount;
};

#endif // FILESEARCHER_H
