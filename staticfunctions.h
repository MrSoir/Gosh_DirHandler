#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H


#include <QObject>
#include <QString>
#include <QLayout>
#include <QWidget>
#include <QPoint>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QErrorMessage>
#include <QFileIconProvider>
#include <QVector>
#include <QLayoutItem>
#include <QPainter>
#include <QDebug>
#include <QGraphicsItem>
#include <QDateTime>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QMimeDatabase>
#include <QImageReader>

#include <memory>

#include <qmath.h>

#include <sstream>

#include "orientation.h"
#include "orderby.h"
#include "process.h"

using namespace std;
using namespace Orientation;

static QMimeDatabase m_mimeDataBase;

namespace StaticFunctions {

    bool controlPressed();
    bool shiftPressed();
    bool altPressed();

    QString strinListToString(const QStringList& list, QString delimiter = QString(""));

    void openFileWithAppcliation(const QFileInfo& fileToOpen, const QString& executionProgram);

    QString curDateTimeToFormattedString();
    QString generateRandomFilePath(const QDir& dir, const QString& fileExtension = QString(""));

    //--------------------------------------------------------------

    QFileInfo getCommonParentDir(const QFileInfo& one,
                                       QFileInfo two);

    QFileInfo getCommonParentDir(const QVector<QFileInfo>& entries);

    //--------------------------------------------------------------

    QFileInfoList getEntriesInDirectoryHelper(QDir  dir, QDir::Filters flags);
    QFileInfoList getFoldersInDirectory(const QDir& dir, bool includeHiddenFiles = false);
    QFileInfoList getFilesInDirectory(  const QDir& dir, bool includeHiddenFiles = false);
    QFileInfoList getEntriesInDirectory(const QDir& dir, bool includeHiddenFiles = false);
    // mit sortFlag:
    QFileInfoList getEntriesInDirectoryHelperSorted(QDir  dir, QDir::Filters flags, QDir::SortFlags sortFlags);
    QFileInfoList getFoldersInDirectorySorted(const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles = false);
    QFileInfoList getFilesInDirectorySorted(  const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles = false);
    QFileInfoList getEntriesInDirectorySorted(const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles = false);

//    ------------------------------------------------------------------------------------------------------------------------------------

    QColor generateAntiColor(const QColor &col);

    void setIconToWidget(QWidget* widget);

    void showInfoDialog(const QString& str, const QString& title);

    QFont getGoshFont(int fontSize = 8, int weight = QFont::Bold);
    QColor getGoshBlueColor();

    QString getFileType(const QString& fileName);
    QString getFileTypeWithDot(const QString& fileName);
    QString getFileNameWithoutFileType(const QString& fileName);

    void clearLayout_and_DeleteContainingWidgets(QLayout* layout, bool deleteWidgets = true);

    QPoint rotatePoint(const QPoint& p, const QPoint& center, double angle);

//    bool getPixmapFromFile(const QFileInfo& fileInfo, const QSize& size, QPixmap& pixmap);
    QPixmap getFilePixmap(const QFileInfo&     info, const QSize& size);
    QPixmap getFilePixmap(const QString&       path, const QSize& size);
    QPixmap getFilePixmap(const std::string&   path, const QSize& size);
    QIcon getFileIcon(const QFileInfo& info);

    void openTerminal(QString sourceDir);

    //-------------------------------------------------------------------------------------------------------

    QString getUniqueFileName(const QString& absFilePath);
    QString getUniqueFileName  (const QString& dirName, const QString& fileName);
    QString getUniqueFolderName(const QString& path,    const QString& foldName);

    QString getAbsoluteFilePathFromDir(const QDir& dir);
    QString getDir(const QString& path);
    QDir getDir(const QFileInfo& fi);
    bool isSubDirectory(const QString& potentialSubFile, const QString& potentialParentFolder);
    bool isSubDirectory(const QFileInfo& potentialSubFile, const QFileInfo& potentialParentFolder);

    unsigned long evaluateContentCount(const QFileInfo& entry);

    bool unmountDrive(const QDir& drive);

    bool moveFile(const QString& sourceFilePath,
                  const QString& targetFilePath);
    bool moveFolder(std::shared_ptr<bool> cancelled,
                                     const QString& absSourcePath, const QString& tarPath, const QString& tarName,
                                     bool overrideIfExists = false,
                                     std::function<void()> callAfterProgress = nullptr);
    bool moveFile(std::shared_ptr<bool> cancelled,
                                     const QString& absSourcePath, const QString& tarPath, const QString& tarName,
                                     bool overrideIfExists = false,
                                     std::function<void()> callAfterProgress = nullptr);

    bool copyFile  (std::shared_ptr<bool> cancelled,
                    const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool deleteSource = false,
                    std::function<void()> callAfterProgress = nullptr);
    bool copyFolder(std::shared_ptr<bool> cancelled,
                    const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool deleteSource = false,
                    std::function<void()> callAfterProgress = nullptr);

    bool duplicateFile  (std::shared_ptr<bool> cancelled, const QFileInfo& source);
    bool duplicateFolder(std::shared_ptr<bool> cancelled, const QFileInfo& source);

    bool deleteFile  (std::shared_ptr<bool> cancelled, const QFileInfo& path);
    bool deleteFolder(std::shared_ptr<bool> cancelled, const QFileInfo& path);

    QString getTextViaDialog(const QString& init, const QString& heading, int minWidth = 200);

    bool renameFolder(const QFileInfo& path);

    bool isZippedFile(const QFileInfo& file);
    void zipFiles(std::shared_ptr<bool> cancelled,
                 const QList<QFileInfo>& absSourcePaths,
                 const QFileInfo& tarZipFile,
                 bool wait_for_command_to_finish = false,
                 std::function<void()> callAfterProgress = nullptr);
    void unzipFile(std::shared_ptr<bool> cancelled,
                   const QFileInfo &tarZipFile,
                   bool wait_for_command_to_finish = false,
                   std::function<void()> callAfterProgress = nullptr);

    bool createNewFolder(const QString& tarPath, QString& foldName);
    bool createNewFile(const QString& tarPath, QString& fileName);

    QFileInfo openFileDialog(const QString& rootPath = QObject::tr(""));
    //-------------------------------------------------------------------------------------------------------

    QRect qRectF_to_qRect(const QRectF& rectF);
    QRectF qRect_to_qRectF(const QRect& rct);
    void paintCrossRect(QPainter *painter, const QRectF& rct);
    void paintCrossRect(QPainter *painter, const QRect& rct);

    void paintArrowUpDown(QPainter *painter, const QRectF& rct, bool up);
    void paintArrowUp(QPainter *painter, const QRectF& rct);
    void paintArrowDown(QPainter *painter, const QRectF& rct);

    void paintRect(QPainter *painter, const QRectF& rct);
    void paintRect(QPainter *painter, const QRect& rct);

    void paintRect(QPainter *painter, const QRectF& rct,
                   const QColor& gradCol1, const QColor& gradCol2 = nullptr,
                   const QColor& penColor = QColor(0,0,0, 70),
                   int bordWidth = 1);
    void paintTextRect(QPainter *painter, const QString& str,
                       const QRectF& rct,
                       const QColor& gradCol1, const QColor& gradCol2 = nullptr,
                       const QColor& textCol = QColor(0,0,0),
                       const QFont font = StaticFunctions::getGoshFont(7),
                       const QColor& borderCol = QColor(0,0,0),
                       int bordWidth = 1);

    void paintPixmapRect(QPainter *painter, const QPixmap& pixmap,
                       const QRectF& rct,
                       const QColor& gradCol1, const QColor& gradCol2 = nullptr,
                       const QColor& borderCol = QColor(0,0,0),
                       int bordWidth = 1);
    void paintPixmapTranspRect(QPainter* painter, const QPixmap& pixmap,
                               const QRectF& frameRct,
                               float padding = 2.0f);


    enum SHAPE{PLUS, MINUS, NONE};
    void paintLoupe(QPainter *painter, const QRectF& rct, SHAPE shape = SHAPE::NONE);

    void paintPixmap(QPainter *painter, const QRectF& target, QString imgPath);


    //-----------------------------------------------------------------------------------

    QString getPictureResourceFromFileName(const QString& fileName);
    QPushButton* PixmapButtonFromPicsResources(const QString& pixmapFileName, QSize* size = nullptr);
    QPushButton* PixmapButton(const QString& pixmapPath, QSize* size = nullptr);

    //-----------------------------------------------------------------------------------

    void setBackgroundColorToWidget(QWidget* widget, QColor col = QColor(255,255,255, 255));

    //-----------------------------------------------------------------------------------

    template<class T>
    bool contains(const QVector<std::weak_ptr<T>>& vec, std::weak_ptr<T> tar){
        auto it = std::find_if(vec.begin(), vec.end(),
                               [&](std::weak_ptr<T> ref1){
                if(auto sp1 = ref1.lock()){
                    if(auto sp2 = tar.lock()){
                        return sp1 == sp2;
                    }
                    return false;
                }
                return false;
        });
        return !(it == vec.end());
    }
    template<class T>
    bool contains(const QVector<std::shared_ptr<T>>& vec, std::weak_ptr<T> tar){
        auto it = std::find_if(vec.begin(), vec.end(),
                               [&](std::weak_ptr<T> sp1){
                if(auto sp2 = tar.lock()){
                    return sp1 == sp2;
                }
                return false;
        });
        return !(it == vec.end());
    }

    template<class T>
    int indexOf(const QVector<std::weak_ptr<T>>& vec, std::weak_ptr<T> tar){
        int id = -1;
        int counter = 0;
        std::find_if(vec.begin(), vec.end(),
                               [&](std::weak_ptr<T> ref1){
                if(auto sp1 = ref1.lock()){
                    if(auto sp2 = tar.lock()){
                        id = counter;
                        return sp1 == sp2;
                    }
                }
                ++counter;
                return false;
        });
        return id;
    }
    template<class T>
    int indexOf(const QVector<std::shared_ptr<T>>& vec, std::weak_ptr<T> tar){
        int id = -1;
        int counter = 0;
        std::find_if(vec.begin(), vec.end(),
                               [&](std::shared_ptr<T> sp1){
                if(auto sp2 = tar.lock()){
                    id = counter;
                    return sp1 == sp2;
                }
                ++counter;
                return false;
        });
        return id;
    }

    template<class T>
    bool removeOne(QVector<std::weak_ptr<T>>& vec, std::weak_ptr<T> tar){
        int id = indexOf(vec, tar);
        if(id > -1){
            vec.removeAt(id);
            return true;
        }
        return false;
    }

    template<class T>
    bool removeOne(QVector<std::shared_ptr<T>>& vec, std::weak_ptr<T> tar){
        int id = indexOf(vec, tar);
        if(id > -1){
            vec.removeAt(id);
            return true;
        }
        return false;
    }

//    ---------------------------------------------------------------------------------------------

    bool launchProcess(const QString& program, const QStringList& arguments,
                       const QString& workingDirectory = QString(""),
                       bool waitForFinished = true);
}
#endif // STATICFUNCTIONS_H
