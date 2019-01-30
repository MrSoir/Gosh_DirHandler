#include "staticfunctions.h"

bool StaticFunctions::controlPressed()
{
    return QApplication::keyboardModifiers() & Qt::ControlModifier;
}
bool StaticFunctions::shiftPressed()
{
    return QApplication::keyboardModifiers() & Qt::ShiftModifier;
}
bool StaticFunctions::altPressed()
{
    return QApplication::keyboardModifiers() & Qt::AltModifier;
}

QFont StaticFunctions::getGoshFont(int fontSize, int weight)
{
//    return QFont("Helvetica [Cronyx]", fontSize, weight);
    return QFont("FreeMono", fontSize, weight);
}

QColor StaticFunctions::getGoshBlueColor()
{
    return QColor(180,180,255,90);
}

//--------------------------------------------------------------

QFileInfoList StaticFunctions::getEntriesInDirectoryHelper(QDir dir, QDir::Filters flags)
{
    return dir.entryInfoList(flags);
}

QFileInfoList StaticFunctions::getFoldersInDirectory(const QDir& dir, bool includeHiddenFiles)
{
    QDir::Filters filter = includeHiddenFiles ? (QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot | QDir::Hidden) :
                                                (QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);

    return getEntriesInDirectoryHelper(dir, filter);
}
QFileInfoList StaticFunctions::getFilesInDirectory(const QDir& dir, bool includeHiddenFiles)
{
    QDir::Filters filter = includeHiddenFiles ? (QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden) :
                                                (QDir::Files | QDir::NoDotAndDotDot);
    return getEntriesInDirectoryHelper(dir, filter);
}
QFileInfoList StaticFunctions::getEntriesInDirectory(const QDir& dir, bool includeHiddenFiles)
{
    // 1. files rausholen:
    QFileInfoList entries = getFilesInDirectory(dir, includeHiddenFiles);
    // 2. folders rausholen:
    entries.append( getFoldersInDirectory(dir, includeHiddenFiles));
    return entries;
}

QFileInfoList StaticFunctions::getEntriesInDirectoryHelperSorted(QDir dir, QDir::Filters flags, QDir::SortFlags sortFlags)
{
    return dir.entryInfoList(flags, sortFlags);
}

QFileInfoList StaticFunctions::getFoldersInDirectorySorted(const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles)
{
    QDir::Filters filter = includeHiddenFiles ? (QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot | QDir::Hidden) :
                                                (QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);

    return getEntriesInDirectoryHelperSorted(dir, filter, sortFlags);
}
QFileInfoList StaticFunctions::getFilesInDirectorySorted(const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles)
{
    QDir::Filters filter = includeHiddenFiles ? (QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden) :
                                                (QDir::Files | QDir::NoDotAndDotDot);
    return getEntriesInDirectoryHelperSorted(dir, filter, sortFlags);
}
QFileInfoList StaticFunctions::getEntriesInDirectorySorted(const QDir& dir, QDir::SortFlags sortFlags, bool includeHiddenFiles)
{
    // 1. files rausholen:
    QFileInfoList entries = getFilesInDirectorySorted(dir, sortFlags, includeHiddenFiles);
    // 2. folders rausholen:
    entries.append( getFoldersInDirectorySorted(dir, sortFlags, includeHiddenFiles));
    return entries;
}

//--------------------------------------------------------------

QString StaticFunctions::getFileType(const QString &fileName)
{
    int lastIndxOf = fileName.lastIndexOf(".");

    return lastIndxOf == -1 ? "" : fileName.section(".", -1);
}

QString StaticFunctions::getFileTypeWithDot(const QString &fileName)
{
    int lastIndxOf = fileName.lastIndexOf(".");

    return lastIndxOf == -1 ? "" : QString(".").append(fileName.section(".", -1));
}

QString StaticFunctions::getFileNameWithoutFileType(const QString &fileName)
{
    QString fileType = getFileType(fileName);
    if(fileType.isEmpty()){
        return fileName;
    }
    return fileName.left(fileName.length()-fileType.length()-1);
}

//void StaticFunctions::removeChildrenFromLayout(QLayout* lay, const QVector<QLayoutItem*>& notToDelete){
//    if ( lay != nullptr ){
//        QLayoutItem* item;
//        while ( (item = lay->takeAt(0)) != nullptr ){
//            bool deleteItem = true;
//            for(int i=0; i < notToDelete.size(); ++i)
//            {
//                if(notToDelete[i] == item)
//                {
//                    qDebug() << "\n\n-------------in StaticFunctions::removeChildrenFromLayout -> item not to delete detected!---------------\n\n";
//                    deleteItem = false;
//                    break;
//                }
//            }
//            if( deleteItem )
//            {
//                if(dynamic_cast<QLayout*>(item) != nullptr){
//                    deleteLayoutandContainingWidgets(dynamic_cast<QLayout*>(item), lay);
//                }else{
//                    if(item != nullptr){
//                        delete item->widget();
//                        delete item;
//                    }
//                }
//            }
//        }
//    }
//}

//void StaticFunctions::deleteLayoutandContainingWidgets(QLayout *lay, QLayout *parent)
//{
////    qDebug() << "in StaticFunctions::deleteLayoutandContainingWidgets";
//    if ( lay != NULL ){
//        QLayoutItem* item;
//        while ( (item = lay->takeAt(0)) != NULL ){
//            if(dynamic_cast<QLayout*>(item) != NULL){
//                deleteLayoutandContainingWidgets(dynamic_cast<QLayout*>(item), lay);
//            }else if(item != NULL){
//                delete item->widget();
//                delete item;
//            }
//        }
//        if(parent)
//            parent->removeItem(lay);
//        delete lay;
//    }
//}
//void StaticFunctions::deleteLayoutandContentsFromWidget(QWidget *widget, QLayout* parent)
//{
//    if(widget){
//        QLayout* lay = widget->layout();
//        widget->setLayout(nullptr);
//        if(lay){
//            deleteLayoutandContainingWidgets(lay);
//        }
//        if(parent)
//            parent->removeWidget(widget);
//        delete widget;
//    }
//}

QPoint StaticFunctions::rotatePoint(const QPoint& p, const QPoint& center, double angle)
{
    double s = qSin(qDegreesToRadians(angle));
    double c = qCos(qDegreesToRadians(angle));

    double px = p.x();
    double py = p.y();

    double cx = center.x();
    double cy = center.y();

    px -= cx;
    py -= cy;

    double xnew = px * c - py * s;
    double ynew = px * s + py * c;

    return QPoint( static_cast<int>(xnew + cx), static_cast<int>(ynew + cy) );
}

//bool StaticFunctions::getPixmapFromFile(const QFileInfo& fileInfo, const QSize& size, QPixmap& pixmap)
//{
//    Image image;
//    try {
//        image.read( fileInfo.absoluteFilePath().toStdString() );

//        // Crop the image to specified size (width, height, xOffset, yOffset)
//        image.resize( QString("%1x%2").arg(size.width())
//                                      .arg(size.height())
//                                      .toStdString() );

//        Blob blob;
//        image.magick("PNG"); // or PNG
//        image.write(&blob);
//        const QByteArray imageData((char*)(blob.data()),blob.length());

//        pixmap.loadFromData(imageData);

//        return true;
//    }
//    catch(Exception& error)
//    {
//        qDebug() << "StaticFunctions::getPixmapFromFile: Caught exception reading image";
//        return false;
//    }
//}

QPixmap StaticFunctions::getFilePixmap(const QFileInfo &info, const QSize& size)
{
//    QMimeDatabase m_mimeDataBase;

    // loading images: laeuft bisher nur leider unfaaaaassbar langsam, obwohl der QImageReader
    // sofort von anfang an auf setScaledSize(size) gesetzt wird
//    const QString absPath = info.absoluteFilePath();
//    QMimeType type = m_mimeDataBase.mimeTypeForFile(absPath);
//    if(type.name().startsWith(QObject::tr("image")))
//    {
////        QPixmap pixmap;
////        bool successfullyLoaded = getPixmapFromFile(info, size, pixmap);
////        if(successfullyLoaded)
////            return pixmap;
//        QImageReader reader;
//        reader.setScaledSize(size);
//        reader.setFileName(absPath);
//        QImage img = reader.read();
//        QPixmap pixmap(size);
//        bool successfullyLoaded = pixmap.convertFromImage(img);
//        if(successfullyLoaded)
//            return pixmap;
//    }

    QIcon icon = getFileIcon(info);
    return icon.pixmap(size);
}
QPixmap StaticFunctions::getFilePixmap(const QString &path, const QSize &size)
{
    return getFilePixmap(QFileInfo(path), size);
}
QPixmap StaticFunctions::getFilePixmap(const std::string &path, const QSize &size)
{
    return getFilePixmap(QFileInfo(QString::fromStdString(path)), size);
}

QIcon StaticFunctions::getFileIcon(const QFileInfo &info)
{
    QFileIconProvider ip;
    return ip.icon(info);
}

//-------------------------------------------------------------------------------------------------------


QRect StaticFunctions::qRectF_to_qRect(const QRectF& rectF){
    return QRect(static_cast<int>(rectF.x()), static_cast<int>(rectF.y()),
                 static_cast<int>(rectF.width()), static_cast<int>(rectF.height()));
}
QRectF StaticFunctions::qRect_to_qRectF(const QRect& rct){
    return QRectF((static_cast<int>(rct.x())),
                  (static_cast<int>(rct.y())),
                  (static_cast<int>(rct.width())),
                  (static_cast<int>(rct.height())));
}
void StaticFunctions::paintCrossRect(QPainter *painter, const QRectF& rct){
    float offsX = ((float)rct.width()) * 0.1;
    float offsY = ((float)rct.height()) * 0.1;

    QRectF wndwRct = QRect(rct.left()+offsX,
                    rct.top()+offsY,
                    rct.width()-offsX*2,
                    rct.height()-offsY*2);

    painter->save();
    float penWidth = (float)((qMin(offsX, offsY)) *0.2);
    if (penWidth < 1.5){penWidth = 1.5;}
    if (penWidth > 3.){penWidth = 3.;}
    painter->setPen(QPen(QColor(0,0,0, 255), penWidth, Qt::SolidLine));
    painter->drawLine(wndwRct.right(), wndwRct.top(),
                      wndwRct.left(),  wndwRct.bottom());
    painter->drawLine(wndwRct.left(), wndwRct.top(),
                      wndwRct.right(),  wndwRct.bottom());
    painter->restore();
}
void StaticFunctions::paintCrossRect(QPainter *painter, const QRect& rct){
    paintCrossRect(painter, qRect_to_qRectF(rct));
}

void StaticFunctions::paintArrowUpDown(QPainter *painter, const QRectF& rct, bool up){
    float offsX = ((float)rct.width()) * 0.2;
    float offsY = ((float)rct.height()) * 0.2;
    QRectF wndwRct = QRect(rct.left()+offsX,
                    rct.top()+offsY,
                    rct.width()-offsX*2,
                    rct.height()-offsY*2);

    painter->save();
    float penWidth = qMin((float)qMin(offsX, offsY), (float)2.);
    painter->setPen(QPen(QColor(0,0,0, 255), penWidth, Qt::SolidLine));
    QPainterPath path;
    if(up){
        path.moveTo(wndwRct.left(), wndwRct.bottom());
        path.lineTo(wndwRct.center().x(), wndwRct.top());
        path.lineTo(wndwRct.right(), wndwRct.bottom());
    }else{
        path.moveTo(wndwRct.left(), wndwRct.top());
        path.lineTo(wndwRct.center().x(), wndwRct.bottom());
        path.lineTo(wndwRct.right(), wndwRct.top());
    }
    painter->drawPath(path);
    painter->restore();
}
void StaticFunctions::paintArrowUp(QPainter *painter, const QRectF& rct){
    paintArrowUpDown(painter, rct, true);

}
void StaticFunctions::paintArrowDown(QPainter *painter, const QRectF& rct){
    paintArrowUpDown(painter, rct, false);
}

void StaticFunctions::paintRect(QPainter *painter, const QRectF& rct){
    QLinearGradient gradient = QLinearGradient(rct.topLeft(), rct.bottomRight());
    gradient.setColorAt(0, QColor(255,255,255, 255));
    gradient.setColorAt(1, QColor(220,220,220, 255));
    painter->setPen(QPen(QColor(0,0,0, 70)));
    painter->setBrush(QBrush(gradient));
    painter->drawRect(rct);
}
void StaticFunctions::paintRect(QPainter *painter, const QRect& rct){
    paintRect(painter, qRect_to_qRectF(rct));
}

void StaticFunctions::paintRect(QPainter *painter, const QRectF& rct,
               const QColor& gradCol1, const QColor& gradCol2,
               const QColor& penColor,
               int bordWidth){
    if(gradCol2 == nullptr){
        painter->setBrush(gradCol1);
    }else{
        QLinearGradient gradient = QLinearGradient(rct.topLeft(), rct.bottomRight());
        gradient.setColorAt(0, gradCol1);
        gradient.setColorAt(1, gradCol2);
        painter->setBrush(QBrush(gradient));
    }
    painter->setPen(QPen(penColor, bordWidth));
    painter->drawRect(rct);
}
void StaticFunctions::paintTextRect(QPainter *painter, const QString& str,
                   const QRectF& rct,
                   const QColor& gradCol1, const QColor& gradCol2,
                   const QColor& textCol,
                   const QFont font,
                   const QColor& borderCol,
                   int bordWidth){
    paintRect(painter, rct, gradCol1, gradCol2,borderCol, bordWidth);
    painter->setFont(font);
    QFontMetrics fm(painter->font());
    int strWidth = fm.width(str);
    int strHeight = fm.height();
    painter->setPen(QPen(textCol));
    painter->drawText(QPointF(rct.center()-
                             QPointF(strWidth*0.5, strHeight*0.5-fm.ascent())
                             ),
                      str);
}
void StaticFunctions::paintPixmapRect(QPainter *painter, const QPixmap& pixmap,
                   const QRectF& frameRct,
                   const QColor& gradCol1, const QColor& gradCol2,
                   const QColor& borderCol,
                   int bordWidth){
    paintRect(painter, frameRct, gradCol1, gradCol2, borderCol, bordWidth);
    QRectF pixmpRct( (frameRct.center()-QPointF(pixmap.width()*0.5, pixmap.height()*0.5)),
                     QSizeF(pixmap.width(), pixmap.height()) );
//    painter->drawPixmap(rct, pixmap, pixmap.rect());
    painter->drawPixmap(pixmpRct, pixmap, pixmap.rect());
}
void StaticFunctions::paintPixmapTranspRect(QPainter* painter, const QPixmap& pixmap,
                           const QRectF& frameRct,
                           float padding)
{
    QRectF tarRect(frameRct.left()  + padding, frameRct.top()    + padding,
                   frameRct.width() - padding, frameRct.height() - padding);
    painter->drawPixmap(tarRect, pixmap, pixmap.rect());
}


void StaticFunctions::paintLoupe(QPainter *painter, const QRectF& rct, SHAPE shape){
    qreal edge = rct.width(); // edge==Kante -> rct muss quadratisch sein!
    qreal radFctr = 1./3.;
    qreal centFctr = 4./10.;
    qreal circRadius = edge * radFctr;
    qreal circCenterX = rct.left() + edge * centFctr;
    qreal circCenterY = rct.top() + edge * centFctr;
    qreal circLeft = rct.left() + edge * (centFctr-radFctr);
    qreal circTop = rct.top() + edge * (centFctr-radFctr);
    qreal sine45 = sin(45. * 3.141592653589793 / 180.0);
    qreal loupeCenterX = circCenterX + sine45 * circRadius;
    qreal loupeCenterY = circCenterY + sine45 * circRadius;
    qreal radialCenterX = circCenterX - (sine45 * circRadius) / 2.;
    qreal radialCenterY = circCenterY - (sine45 * circRadius) / 2.;

    painter->save();

    QRadialGradient gradient = QRadialGradient(QPoint(radialCenterX,radialCenterY), circRadius);
    gradient.setSpread(QGradient::ReflectSpread);
    QColor circCol1(150,255,150, 255);
    QColor circCol2(255,255,255, 255);
    gradient.setColorAt(0, circCol1);
    gradient.setColorAt(1, circCol2);
    painter->setPen(QPen(QColor(0,0,0), 1, Qt::SolidLine));
    painter->setBrush(QBrush(gradient));
    painter->drawEllipse(circLeft, circTop, circRadius*2., circRadius*2.);

    painter->setBrush(QColor(0,0,0));
    qreal strokeWidth = 3.;
    painter->setPen(QPen(QColor(0,0,0), strokeWidth, Qt::SolidLine));
    QPointF loupeCenter(loupeCenterX +strokeWidth*0.5, // 7/12 == 1/3<->circCenter + 1/4<->circRadius
                       loupeCenterY +strokeWidth*0.5);
    QPointF loupeRightBottom(rct.right() - edge/6.,
                            rct.bottom() - edge/6.);
    painter->drawLine(loupeCenter, loupeRightBottom);

    painter->setPen(QPen(QColor(0,0,0), strokeWidth*0.5, Qt::SolidLine));
    if(shape == SHAPE::PLUS || shape == SHAPE::MINUS){
        painter->drawLine(QPointF(circCenterX-(circRadius*2./3.),
                                  circCenterY),
                          QPointF(circCenterX+(circRadius*2./3.),
                                  circCenterY));
        if (shape == SHAPE::PLUS){
            painter->drawLine(QPointF(circCenterX,
                                      circCenterY-(circRadius*2./3.)),
                              QPointF(circCenterX,
                                      circCenterY+(circRadius*2./3.)));
        }
    }

    painter->restore();
}

void StaticFunctions::paintPixmap(QPainter *painter, const QRectF& target, QString imgPath){
    QPixmap pixmap(imgPath);
    QSize source = pixmap.size();

    painter->drawPixmap(target, pixmap, QRectF(QPointF(0,0), source));
}

//-------------------------------------------------------------------------------------------------------

QString StaticFunctions::getUniqueFileName(const QString& absFilePath)
{
    QFileInfo fi(absFilePath);
    QDir dir = getDir(fi);
    QString fileName = fi.fileName();
    return getUniqueFileName(QString("%1%2%3").arg(dir.absolutePath())
                                              .arg(QDir::separator())
                                              .arg(fileName));

}
QString StaticFunctions::getUniqueFileName(const QString& dirName, const QString& fileName){
    int counter = 1;
    QString suffix("");
    QString blankFileName = getFileNameWithoutFileType(fileName);
    QString fileType = getFileTypeWithDot(fileName);
    while(QFileInfo(QString(dirName).append(QDir::separator())
                 .append(blankFileName)
                 .append(suffix)
                 .append(fileType)).exists()){
        suffix = QString("_(%1)").arg(counter++);
    }
    return blankFileName.append(suffix).append(fileType);
}
QString StaticFunctions::getUniqueFolderName(const QString& path, const QString &foldName){
    int counter = 1;
    QString suffix("");
    while(QFileInfo(QString(path)
                    .append(QDir::separator())
                    .append(foldName)
                    .append(suffix)).exists()){
        suffix = QString("_(%1)").arg(counter++);
    }
    return QString(foldName).append(suffix);
}

QString StaticFunctions::getDir(const QString &path)
{
    QFileInfo fi(path);
    return fi.isDir() ?
                fi.absoluteFilePath() :
                fi.absolutePath();
}

QDir StaticFunctions::getDir(const QFileInfo& fi)
{
    // ist wichtig, denn:
    // QFileInfo("/home/bigdaddy/Documents/test_folder").dir()  ~> QDir("/home/bigdaddy/Documents")
    // QFileInfo("/home/bigdaddy/Documents/test_folder/").dir() ~> QDir("/home/bigdaddy/Documents/test_folder")
    // -> beachte den '/' am ende des QFileInfo-paths!!! bescheuerte besonderheit von der QFileInfo::dir()-Funktion!!!
    return fi.isDir() ?
            QDir(fi.absoluteFilePath()) :
            fi.dir();
}

bool StaticFunctions::isSubDirectory(const QString& potentialSubFile, const QString& potentialParentFolder)
{
    return isSubDirectory(QFileInfo(potentialSubFile), QFileInfo(potentialParentFolder));
}
bool StaticFunctions::isSubDirectory(const QFileInfo& potentialSubFile, const QFileInfo& potentialParentFolder)
{
    // eine datei kann kein parent-folder sein von irgendwas:
    if(potentialParentFolder.isFile())
        return false;

    QDir parentFolder   = getDir(potentialParentFolder);
    QDir subFilesFolder = getDir(potentialSubFile);

//    qDebug() << "inisSubDirectory: \n potentialSubFile: " << subFilesFolder.absolutePath()
//             << "\n   potentialParentFolder: " << parentFolder.absolutePath();

    int parentFolderPathLength = parentFolder.absolutePath().length();

    do{
        if(subFilesFolder == parentFolder)
            return true;
    }while(subFilesFolder.cdUp() &&
           subFilesFolder.absolutePath().length() >= parentFolderPathLength);

    return false;
}


unsigned long StaticFunctions::evaluateContentCount(const QFileInfo& entry)
{
    if( !entry.exists() )
        return 0;

    unsigned long entriesCount = 1;

    if(entry.isDir())
    {
        const QString absSourcePath = entry.absoluteFilePath();

        QFileInfoList entries = StaticFunctions::getEntriesInDirectory(QDir(absSourcePath), true);//QDir(absSourcePath).entryInfoList(QDir::AllEntries | QDir::Hidden);

        for(auto it = entries.begin(); it != entries.end(); ++it)
        {
            if(it->isDir())
            {
                if( !isSubDirectory(absSourcePath, it->absoluteFilePath()) )
//              if( !absSourcePath.startsWith( it->absoluteFilePath() ) )
                {
                    entriesCount += evaluateContentCount( *it );
                }
            }else
            {
                ++entriesCount;
            }
        }
    }
    return entriesCount;
}

// diese moveFile-Methode verlaesst sich darauf, dass targetFilePath
// NICHT existiert und somit nicht versuchst wird, eine bereits
// existierende datei zu ueberschreiben (was mit dem os-move-befehl
// ohnehin nicht funktionieren wuerde):
bool StaticFunctions::moveFile(const QString& sourceFilePath,
                               const QString& targetFilePath)
{
    QStringList arguments;
    arguments << sourceFilePath;
    arguments << targetFilePath;

//        qDebug() << "moveing: absSourcePath: " << absSourcePath
//                 << "   moveTarget: " << moveTarget;

    Process *myProcess = new Process();
    myProcess->setWorkingDirectory( sourceFilePath );
    myProcess->execute("mv", arguments);
    return myProcess->waitForFinished();
}
bool StaticFunctions::moveFolder(std::shared_ptr<bool> cancelled,
                                 const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool overrideIfExists,
                                 std::function<void()> callAfterProgress)
{
    return moveFile(cancelled, absSourcePath, tarPath, tarName, overrideIfExists, callAfterProgress);
}
bool StaticFunctions::moveFile(std::shared_ptr<bool> cancelled,
                               const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool overrideIfExists,
                               std::function<void()> callAfterProgress)
{
    if(cancelled && *cancelled)
        return false;

    if(QFileInfo(absSourcePath).exists())
    {

        QString absTarPath = QString("%1%2%3").arg(tarPath).arg(QDir::separator()).arg(tarName);
        QString moveTarget;

        bool targetPathAlreadyExisted = false;

        if( QFile(absTarPath).exists() )
        {
            QString uniqueFileName = getUniqueFileName(tarPath, tarName);
            moveTarget = QString(tarPath).append(QDir::separator()).append(uniqueFileName);

            targetPathAlreadyExisted = true;
        }else
        {
            moveTarget = absTarPath;
        }

        moveFile(absSourcePath, moveTarget);

        if(overrideIfExists && targetPathAlreadyExisted)
        {
            QFileInfo entryToDelete(absTarPath);
            if(entryToDelete.exists())
            {
                if( entryToDelete.isDir() )
                {
                    StaticFunctions::deleteFolder(cancelled, entryToDelete);
                }else
                {
                    StaticFunctions::deleteFile(cancelled, entryToDelete);
                }
                QFile fileToRename(moveTarget);
                fileToRename.rename(absTarPath);
            }
        }

        if(callAfterProgress)
            callAfterProgress();

        return true;
    }
    return false;
}
bool StaticFunctions::copyFile(std::shared_ptr<bool> cancelled,
                               const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool deleteSource,
                               std::function<void()> callAfterProgress)
{
    if(cancelled && *cancelled)
        return false;

    if(QFileInfo(absSourcePath).exists()){
        QString uniqueFileName = getUniqueFileName(tarPath, tarName);

        QString copyTarget = QString(tarPath).append(QDir::separator()).append(uniqueFileName);

        bool copiedSuccessfully = QFile::copy(absSourcePath, copyTarget);

        if(copiedSuccessfully && deleteSource)
        {
            if(cancelled && *cancelled)
                return false;
            copiedSuccessfully = QFile::remove(absSourcePath);
        }

        if(callAfterProgress)
            callAfterProgress();

        return copiedSuccessfully;
    }

    if(callAfterProgress)
        callAfterProgress();
    return false;
}

bool StaticFunctions::copyFolder(std::shared_ptr<bool> cancelled,
                                 const QString& absSourcePath, const QString& tarPath, const QString& tarName, bool deleteSource,
                                 std::function<void()> callAfterProgress)
{
    if(cancelled && *cancelled)
        return false;

    if(QFileInfo(absSourcePath).exists()){

        if( isSubDirectory(tarPath, absSourcePath) )
        {
            return false;
        }

        QString uniqueTarFolderName = getUniqueFolderName(tarPath, tarName);
        QString tarFolderPath = QString(tarPath).append(QDir::separator()).append(uniqueTarFolderName);

        bool succ_created = QDir().mkdir(tarFolderPath);
        if(callAfterProgress)
            callAfterProgress();

        if(succ_created){
            QFileInfoList content = StaticFunctions::getEntriesInDirectory(QDir(absSourcePath), true);

            bool curSuccess;

            foreach(auto cont, content){
                if(cont.isDir())
                {
                    // absSourcePath.startsWith( cont.absoluteFilePath() ) -> da mit QDir::Hidden beim kopieren gearbeitet werden MUSS,
                    // wird unter linux mit dem . und .. auch ordner selbst und sein parent-folder mitgelistet in den entries.
                    // um diese beiden auszuschliessen, d.h. nicht auch zu kopieren (wuerde in endlosschleife enden), wird gecheckt,
                    // dass der absSOurcePath nicht den "subfolder" in sich komplett traegt, denn dann ware es eben entweder sich selbst oder
                    // sein eigener parent-folder:
                    if( !isSubDirectory(absSourcePath, cont.absoluteFilePath()) )
                    {
                        if(cancelled && *cancelled)
                            return false;
                        curSuccess = copyFolder(cancelled, cont.absoluteFilePath(), tarFolderPath, cont.fileName(), deleteSource, callAfterProgress);
                    }
                }else
                {
                    if(cancelled && *cancelled)
                        return false;
                    curSuccess = copyFile(cancelled, cont.absoluteFilePath(), tarFolderPath, cont.fileName(), deleteSource, callAfterProgress);
                }
                if( !curSuccess ){
                    succ_created = false;
                }
            }

            if(succ_created && deleteSource)
            {
                if(cancelled && *cancelled)
                    return false;
                succ_created = QDir(absSourcePath).removeRecursively();
            }

            return succ_created;
        }
    }
    return false;
}

bool StaticFunctions::duplicateFile(std::shared_ptr<bool> cancelled, const QFileInfo &source)
{
    return copyFile  (cancelled, source.absoluteFilePath(), source.absolutePath(), source.fileName());
}

bool StaticFunctions::duplicateFolder(std::shared_ptr<bool> cancelled, const QFileInfo &source)
{
    return copyFolder(cancelled, source.absoluteFilePath(), source.absolutePath(), source.fileName());
}

bool StaticFunctions::deleteFile(std::shared_ptr<bool> cancelled, const QFileInfo &path)
{
    if(cancelled && *cancelled)
        return false;
    if(path.exists() && path.isFile()){
        return QDir().remove(path.absoluteFilePath());
    }
    return true;// already deleted
}

bool StaticFunctions::deleteFolder(std::shared_ptr<bool> cancelled, const QFileInfo &path)
{
    if(cancelled && *cancelled)
        return false;

    if(path.isDir()){
        return QDir(path.absoluteFilePath()).removeRecursively();
    }
    return false;
//    if(path.exists()){
//        QFileInfoList content = QDir(path.absoluteFilePath()).entryInfoList();
//        foreach(QFileInfo fi, content){
//            if(fi.isDir()){
//                deleteFolder(fi);
//            }else{
//                deleteFile(fi);
//            }
//        }
//        return QDir::remove(path.absoluteFilePath());
//    }
//    return true; // already deleted
}



bool StaticFunctions::renameFolder(const QFileInfo &absFilePath)
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, QString(""),
                                         QString("Rename to:"), QLineEdit::Normal,
                                         absFilePath.fileName(), &ok);
    if (ok && !text.isEmpty()){
        QString newAbsFilePath = absFilePath.absolutePath().append(QDir::separator()).append(text);
        if( QFileInfo(newAbsFilePath).exists() ){
            QErrorMessage().showMessage(QString("could not rename file to '%1' \n - does alread exist!").arg(text));
        }else{
            QFile().rename(absFilePath.absoluteFilePath(), newAbsFilePath);
            return true;
        }
    }
    return false;
}

void StaticFunctions::showInfoDialog(const QString& str, const QString& title)
{
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setWindowTitle(title);
    msgBox->setText(str);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    msgBox->exec();
}


QFileInfo StaticFunctions::openFileDialog(const QString& rootPath)
{
    QString filePath = QFileDialog::getSaveFileName(nullptr,
                    QObject::tr("Open Image"),
                    QFileInfo(rootPath).exists() ? rootPath : QObject::tr(""));
    return QFileInfo(filePath);
}
bool StaticFunctions::isZippedFile(const QFileInfo& file)
{
    if(file.exists())
    {
        QString lowerCaseFileName = file.fileName().toLower();
        bool isCompressed =
                lowerCaseFileName.endsWith(QObject::tr(".zip"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".zipx"))    ||
                lowerCaseFileName.endsWith(QObject::tr(".iso"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".lbr"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".sbx"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".7z" ))     ||
                lowerCaseFileName.endsWith(QObject::tr(".arj"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".deb"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".pkg"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".rar"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".rpm"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".tar.gz"))  ||
                lowerCaseFileName.endsWith(QObject::tr(".z"))       ||
                lowerCaseFileName.endsWith(QObject::tr(".jar"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".odt"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".ods"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".odp"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".docx"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".xlsx"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".xlsm"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".pptx"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".vsdx"))     ||
                lowerCaseFileName.endsWith(QObject::tr(".apk"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".epub"))      ||
                lowerCaseFileName.endsWith(QObject::tr(".aar"))      ;

        if(isCompressed)
            return true;

        Process *myProcess = new Process();
        myProcess->start("file", QStringList() << file.absoluteFilePath());
        myProcess->waitForFinished(); // sets current thread to sleep and waits for pingProcess end

        QString output = (myProcess->readAllStandardOutput()).toLower();
        return output.contains("compres") ||
               output.contains("zip");
    }
    return false;
}
void StaticFunctions::zipFiles(std::shared_ptr<bool> cancelled,
                              const QList<QFileInfo>& absSourcePaths, const QFileInfo& tarZipFile,
                              bool wait_for_command_to_finish,
                              std::function<void()> callAfterProgress)
{
    Q_UNUSED(cancelled);

    // nun der plattformUNabhaengige weg ueber python-skript:
//    ZipFiles::zipFiles(tarZipFile, absSourcePaths,
//                       wait_for_command_to_finish, callAfterProgress, cancelled);
}
void StaticFunctions::unzipFile(std::shared_ptr<bool> cancelled,
                                const QFileInfo& tarZipFile,
                                bool wait_for_command_to_finish,
                                std::function<void()> callAfterProgress)
{
    Q_UNUSED(cancelled);

    // nun der plattformUNabhaengige weg ueber python-skript:
//    ZipFiles::unzipFile(tarZipFile, QFileInfo(getDir(tarZipFile).absolutePath()),
//                        wait_for_command_to_finish,
//                        callAfterProgress,
//                        cancelled);
}

bool StaticFunctions::createNewFolder(const QString &tarPath, QString &foldName)
{
    foldName = getUniqueFolderName(tarPath, foldName);
    bool successfullyCreated = QDir().mkdir( QString("%1%2%3")
                  .arg(tarPath)
                  .arg(QDir::separator())
                  .arg(foldName) );
    return successfullyCreated;
}

bool StaticFunctions::createNewFile(const QString& tarPath, QString& fileName)
{
        fileName = getUniqueFileName(tarPath, fileName);
    QFile file( QString("%1%2%3")
                .arg(tarPath)
                .arg(QDir::separator())
                .arg(fileName) );
    bool successfullyCreated = file.open(QIODevice::WriteOnly);
    file.close();
    return successfullyCreated;
}

void StaticFunctions::setIconToWidget(QWidget *widget)
{
    QString windowIconPath = QString("%1%2%3")
            .arg("pics")
            .arg(QDir::separator())
            .arg("MrSoirIcon.png");
//            .arg("MrSoir_antique.png");
    widget->setWindowIcon(QIcon(windowIconPath));
}

void StaticFunctions::openTerminal(QString sourceDir)
{
    Process* proc = new Process();
    proc->setWorkingDirectory(sourceDir);
    proc->start("gnome-terminal");
}

QString StaticFunctions::getTextViaDialog(const QString& init, const QString& heading, int minWidth)
{
    bool ok;
    QString text = QInputDialog::getText(nullptr, QString(""),
                                         heading, QLineEdit::Normal,
                                         init, &ok);
    return ok ? text : QObject::tr("");
}


QPushButton* StaticFunctions::PixmapButton(const QString &pixmapPath, QSize* size)
{
    if( !QFile(pixmapPath).exists() )
        return nullptr;

    QPixmap pixmap(pixmapPath);
    QIcon ButtonIcon(pixmap);

    QPushButton* btn = new QPushButton();
    btn->setIcon(ButtonIcon);
    if(size == nullptr)
        btn->setIconSize(pixmap.rect().size());
    else
        btn->setIconSize(*size);
    return btn;
}

QPushButton* StaticFunctions::PixmapButtonFromPicsResources(const QString &pixmapFileName, QSize *size)
{
   return PixmapButton(getPictureResourceFromFileName(pixmapFileName), size);
}

QString StaticFunctions::getPictureResourceFromFileName(const QString &fileName)
{
    return QString("%1%2%3")
            .arg("pics")
            .arg(QDir::separator())
            .arg(fileName);
}

void StaticFunctions::clearLayout_and_DeleteContainingWidgets(QLayout *layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout_and_DeleteContainingWidgets(childLayout, deleteWidgets);
        delete item;
    }
}

QColor StaticFunctions::generateAntiColor(const QColor &col)
{
    return QColor(255-col.red(),
                  255-col.green(),
                  255-col.blue(),
                  col.alpha());
}

void StaticFunctions::setBackgroundColorToWidget(QWidget* widget, QColor col)
{
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Background, col);
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
}




bool StaticFunctions::unmountDrive(const QDir &drive)
{
    QStringList arguments;
    arguments << drive.absolutePath();

    Process *myProcess = new Process();
    myProcess->execute("umount", arguments);
    return myProcess->waitForFinished();
}

void StaticFunctions::openFileWithAppcliation(const QFileInfo &fileToOpen, const QString& executionProgram)
{
    if( !fileToOpen.exists() )
        return;

    QStringList arguments;

    int id = executionProgram.indexOf("%");
    QString exec = id == -1 ? executionProgram : executionProgram.left(id);
    exec = exec.trimmed();

//    qDebug() << "StaticFunctions::openFileWithAppcliation: fileToOpen: " << fileToOpen.absoluteFilePath()
//             << "   executionProgram: " << executionProgram
//             << "   exec: " << exec;

    arguments << exec;

    arguments << fileToOpen.absoluteFilePath();

    Process *process = new Process();
    process->start( strinListToString(arguments, QString(" ")) );
}

QString StaticFunctions::strinListToString(const QStringList &list, QString delimiter)
{
    QString str;
    for(int i=0; i < list.size(); ++i)
    {
        str += list[i];
        if( i < list.size()-1)
            str += delimiter;
    }
    return str;
}


QString StaticFunctions::generateRandomFilePath(const QDir &dir, const QString &fileExtension)
{
    QString randomFactor;
    int counter = 0;
    QFileInfo targetFile;
    do{
        randomFactor = QString("%1_%2")
                            .arg(curDateTimeToFormattedString())
                            .arg(counter++);
        targetFile = QFileInfo(QString("%1%2%3%4").arg(getAbsoluteFilePathFromDir(dir))
                                                  .arg(QDir::separator())
                                                  .arg(randomFactor)
                                                  .arg(fileExtension));
    }while(targetFile.exists());

    return targetFile.absoluteFilePath();
}

QString StaticFunctions::curDateTimeToFormattedString()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
}

QString StaticFunctions::getAbsoluteFilePathFromDir(const QDir &dir)
{
    return dir.absolutePath();
}

bool StaticFunctions::launchProcess(const QString &program, const QStringList &arguments,
                                    const QString& workingDirectory,
                                    bool waitForFinished)
{
    Process* process = new Process();
    if( !workingDirectory.isEmpty() )
        process->setWorkingDirectory(workingDirectory);
    process->start(program, arguments);
    if(waitForFinished)
        return process->waitForFinished( -1 );
    return true;
}

QFileInfo StaticFunctions::getCommonParentDir(const QFileInfo& one,
                                              QFileInfo two)
{
    // rueckgabewert der funktion ueberpruefen!!! (siehe dazu kommentare weiter unten!)

    // one wird als refDir betrachtet, two als vglDir.
    // D.h. der refDir wird so lange in der ordnerstruktur nach oben gehangelt,
    // bis der refDir ein parentDir des vglDir ist oder die ordner identisch sind.
    // falls two der parentfolder von one ist, ist das zwar scheiss-ineffizient,
    // zurzeit aber keine bessere idee zur hand...
    QDir refDir = StaticFunctions::getDir(one.absoluteFilePath());
    QDir vglDir = StaticFunctions::getDir(two.absoluteFilePath());

    if(refDir.root() != vglDir.root())
        return QFileInfo(); // QFileInfo().absoluteFilePath() == "" | QFileInfo().exists() == false !!!

    two = QFileInfo(vglDir.absolutePath());

    do{
        QFileInfo refFiInfo(refDir.absolutePath());

        if(refDir == vglDir ||
                StaticFunctions::isSubDirectory(two, refFiInfo))
        {
            return refFiInfo;
        }
    }while(refDir.cdUp());

    return QFileInfo(); // QFileInfo().absoluteFilePath() == "" | QFileInfo().exists() == false !!!
}

QFileInfo StaticFunctions::getCommonParentDir(const QVector<QFileInfo>& entries)
{
    // rueckgabewert der funktion ueberpruefen!!! (siehe dazu kommentar ganz am ende!)
    if(entries.size() > 0)
    {
        QFileInfo ref = entries[0];
        for(int i=1; i < entries.size(); ++i)
        {
            QFileInfo parent = StaticFunctions::getCommonParentDir(ref, entries[i]);
            if( !(parent.absoluteFilePath().isEmpty()) && parent.exists() )
                ref = parent;
        }
        return ref;
    }
    return QFileInfo(); // QFileInfo().absoluteFilePath() == "" | QFileInfo().exists() == false !!! rueckgabewert der funktion ueberpruefen!!!
}

