#include "fileselector.h"

FileSelector::FileSelector(QObject *parent)
    : QObject(parent),
      m_selected_paths(std::unordered_set<std::string>()),
      m_paths(nullptr),
      m_ord_paths(nullptr),
      m_paths_ord(nullptr),
      m_fileName_paths(nullptr),
      m_slctd_id(-1),
      m_latestSelctdPath(std::string("")),
      m_slct_key_word(std::string(""))
{
}

FileSelector::~FileSelector()
{
    m_selected_paths.clear();
    m_paths = nullptr;
    m_ord_paths = nullptr;
    m_paths_ord = nullptr;
    m_fileName_paths = nullptr;
    m_slctd_id = -1;
    m_latestSelctdPath = "";
    m_slct_key_word = "";
}

bool FileSelector::isSelected(const std::string& path) const
{
    return m_selected_paths.find(path) != m_selected_paths.end();
}

void FileSelector::processSelection(std::vector<Selectable*>& selection)
{
    for(auto it = selection.begin(); it != selection.end(); ++it)
    {
        (*it)->setSelected( isSelected((*it)->path()) );
    }
}

const std::unordered_set<std::string>& FileSelector::getSelectedFolders() const
{
    return m_selected_folders_paths;
}

const std::unordered_set<std::string>& FileSelector::getSelectedEntries() const
{
    return m_selected_paths;
}

std::string FileSelector::getLastSelectedEntry() const
{
    return m_latestSelctdPath;
//    if(m_slctd_id > -1 && m_ord_paths &&
//            m_ord_paths->find(m_slctd_id) != m_ord_paths->end())
//    {
//        return (*m_ord_paths)[m_slctd_id];
//    }
//    return "";
}

//---------------------------------------------------------

void FileSelector::entriesChanged(std::unordered_set<std::string>* paths,
                                  std::unordered_map<int, std::string>* ord_paths,
                                  std::unordered_map<std::string, int>* paths_ord,
                                  std::unordered_map<std::string, std::string>* fileName_paths,
                                  std::unordered_set<std::string>* folder_paths)
{
    m_paths = paths;
    m_ord_paths = ord_paths;
    m_paths_ord = paths_ord;
    m_fileName_paths = fileName_paths;
    m_folder_paths = folder_paths;

    if(m_paths_ord && !m_latestSelctdPath.empty() )
    {
        auto srch = (*m_paths_ord).find(m_latestSelctdPath);
        if(srch != m_paths_ord->end())
        {
            m_slctd_id = srch->second;
        }else{
            m_slctd_id = -1;
            m_latestSelctdPath = "";
        }
    }else{
        m_slctd_id = -1;
        m_latestSelctdPath = "";
    }

    for(auto it=m_selected_paths.begin(); it != m_selected_paths.end(); )
    {
        if( m_paths->find(*it) != m_paths->end() )
        {
            it = m_selected_paths.erase(it);
        }else{
            ++it;
        }
    }
}

void FileSelector::select(std::string path, bool cntrl_prsd, bool shift_prsd)
{
    if(shift_prsd)
    {
        if(m_paths_ord)
        {
            int id = (*m_paths_ord)[path];
            int lastId = m_slctd_id == -1 ? id : m_slctd_id;
            int lower = id < lastId ? id : lastId;
            int upper = id > lastId ? id : lastId;
            for(int i = lower; i <= upper; ++i)
            {
                const auto& cur_path = (*m_ord_paths)[i];

                m_selected_paths.emplace( cur_path );

                if(m_folder_paths->find(cur_path) != m_folder_paths->end())
                    m_selected_folders_paths.emplace(cur_path);
            }
            m_slctd_id = id;
        }
    }else{
        if( !cntrl_prsd )
        {
            m_selected_paths.clear();
            m_selected_folders_paths.clear();
        }
        m_selected_paths.emplace(path);
        m_slctd_id = (*m_paths_ord)[path];

        if(m_folder_paths->find(path) != m_folder_paths->end())
            m_selected_folders_paths.emplace(path);
    }

    m_latestSelctdPath = path;

    emit selectionChanged();
}

void FileSelector::selectEntiresContent()
{
    for(auto& pth: *m_paths)
        m_selected_paths.emplace(pth);
    for(auto& pth: *m_folder_paths)
        m_selected_folders_paths.emplace(pth);
    m_slctd_id = m_slctd_id == -1 ? 1 : m_slctd_id;

}

void FileSelector::clearSelection()
{
    m_selected_paths.clear();
    m_selected_folders_paths.clear();
    m_slctd_id = -1;
    m_latestSelctdPath = "";
    m_slct_key_word = "";

    emit selectionChanged();
}

void FileSelector::selectNext(bool cntrl_prsd, bool shift_prsd)
{
    if( !(cntrl_prsd || shift_prsd) )
    {
        m_selected_paths.clear();
    }

    if( m_ord_paths )
    {
        if( ++m_slctd_id > static_cast<int>(m_paths->size()) )
        {
            m_slctd_id = 0;
        }
        std::string nextPath = (*m_ord_paths)[m_slctd_id];
        m_latestSelctdPath = nextPath;
        m_selected_paths.emplace( nextPath );
    }
    emit selectionChanged();
}
void FileSelector::selectPrevious(bool cntrl_prsd, bool shift_prsd)
{
    if( !(cntrl_prsd || shift_prsd) )
    {
        m_selected_paths.clear();
    }

    if( m_ord_paths )
    {
        if( --m_slctd_id < 0 )
        {
            m_slctd_id = 0;
        }
        std::string prevPath = (*m_ord_paths)[m_slctd_id];
        m_latestSelctdPath = prevPath;
        m_selected_paths.emplace( prevPath );
    }
    emit selectionChanged();
}

void FileSelector::selectKeyWord(std::string key)
{
    if( m_slct_key_word != key )
    {
        for(auto it = m_fileName_paths->begin(); it != m_fileName_paths->end(); ++it )
        {
            if( StringOps::startsWithIgnoreCase(it->first, key))
            {
                m_selected_paths.clear();
                m_selected_paths.emplace(it->second);
                m_latestSelctdPath = it->second;
                m_slctd_id = (*m_paths_ord)[it->second];
                emit selectionChanged();
                return;
            }
        }
    }
}

//------------------------------------------------------------_
