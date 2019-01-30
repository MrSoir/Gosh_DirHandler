#include "fibdviewer.h"

FiBDViewer::FiBDViewer()
    : m_path(""),
      m_fileName(""),

      m_folder(false),
      m_searched(false),
      m_searchFocused(false),
      m_selected(false),
      m_isLoaded(false),
      m_isElapsed(false),

      m_filesCount(0),
      m_subDirCount(0),
      m_depthId(0),
      m_order(Order()),

      m_fileSize(0),
      m_lastModified(QDateTime::currentDateTime())
{
}

FiBDViewer::FiBDViewer(const QFileInfo &fi)
    : m_path(fi.absoluteFilePath()),
      m_fileName(fi.fileName()),

      m_folder(fi.isDir()),
      m_searched(false),
      m_searchFocused(false),
      m_selected(false),
      m_isLoaded(false),
      m_isElapsed(false),

      m_filesCount(0),
      m_subDirCount(0),
      m_depthId(0),
      m_order(Order()),

      m_fileSize(static_cast<unsigned long long int>(fi.size())),
      m_lastModified(fi.lastModified())
{
}

FiBDViewer::FiBDViewer(const FiBDViewer &fi)
    : m_path(fi.m_path),
      m_folder(fi.m_folder),
      m_searched(fi.m_searched),
      m_searchFocused(fi.m_searchFocused),
      m_selected(fi.m_selected),
      m_isLoaded(fi.m_isLoaded),
      m_isElapsed(fi.m_isElapsed),
      m_filesCount(fi.m_filesCount)
{
}

FiBDViewer::~FiBDViewer()
{
}

void FiBDViewer::setSearched(bool searched)
{
    m_searched = searched;
}
void FiBDViewer::setSearchFocused(bool focused)
{
    m_searchFocused = focused;
}
bool FiBDViewer::searched() const
{
    return m_searched;
}
bool FiBDViewer::searchFocused() const
{
    return m_searchFocused;
}

void FiBDViewer::setSelected(bool selected)
{
    m_selected = selected;
}

std::string FiBDViewer::path() const
{
    return m_path.toStdString();
}

QString FiBDViewer::q_path() const
{
    return m_path;
}

QString FiBDViewer::fileName() const
{
    return m_fileName;
}

unsigned long long FiBDViewer::fileSize() const
{
    return m_fileSize;
}

QDateTime FiBDViewer::lastModified() const
{
    return m_lastModified;
}
bool FiBDViewer::selected() const
{
    return m_selected;
}


bool FiBDViewer::isFolder() const
{
    return m_folder;
}

bool FiBDViewer::isLoaded() const
{
    return m_isLoaded;
}

bool FiBDViewer::isElapsed() const
{
    return m_isElapsed;
}

bool FiBDViewer::containsFiles() const
{
    return m_filesCount > 0;
}

bool FiBDViewer::isEmpty() const
{
    return m_filesCount > 0 || m_subDirCount > 0;
}

bool FiBDViewer::isReversedSortedBy(Order ord) const
{
    return ord.ordered_by == m_order.ordered_by && m_order.reversedOrdered;
}

bool FiBDViewer::isSortedBy(ORDERED_BY ordered_by) const
{
    return m_order.ordered_by == ordered_by;
}

Order FiBDViewer::order() const
{
    return m_order;
}

int FiBDViewer::depthId() const
{
    return m_depthId;
}

//std::string FiBDViewer::path()
//{
//    return m_path;
//}
