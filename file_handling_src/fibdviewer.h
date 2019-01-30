#ifndef FIBDVIEWER_H
#define FIBDVIEWER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QFileInfo>

#include <string>

#include "searchable.h"
#include "selectable.h"
#include "orderby.h"

//using namespace ORDERED_BY;

class FiBDViewer : public Searchable,
                   public Selectable
{
public:
    explicit FiBDViewer();
    explicit FiBDViewer(const QFileInfo& fi);
    FiBDViewer(const FiBDViewer& fi);
    virtual ~FiBDViewer() override;

    bool searched() const;
    bool searchFocused() const;
    bool selected() const;
    bool isFolder() const;
    bool isLoaded() const;
    bool isElapsed() const;
    bool containsFiles() const;
    bool isEmpty() const;
    bool isReversedSortedBy(Order ord) const;
    bool isSortedBy(ORDERED_BY ordered_by) const;
    Order order() const;

    int depthId() const;

    void setSearched(bool searched) override;
    void setSearchFocused(bool focused) override;
    void setSelected(bool selected) override;

    virtual std::string path() const override;
    QString q_path() const;
    QString fileName() const;

    unsigned long long int fileSize() const;
    QDateTime lastModified() const;
private:
    QString m_path;
    QString m_fileName;

    bool m_folder;
    bool m_searched;
    bool m_searchFocused;
    bool m_selected;
    bool m_isLoaded;
    bool m_isElapsed;

    int m_filesCount;
    int m_subDirCount;
    int m_depthId;

    Order m_order;

    unsigned long long int m_fileSize;
    QDateTime m_lastModified;
};



#endif // FIBDVIEWER_H
