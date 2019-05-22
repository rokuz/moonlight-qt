#include "path.h"

#include <QtDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QCoreApplication>

QString Path::s_LogDir;
QString Path::s_BoxArtCacheDir;

QString Path::getLogDir()
{
    Q_ASSERT(!s_LogDir.isEmpty());
    return s_LogDir;
}

QString Path::getBoxArtCacheDir()
{
    Q_ASSERT(!s_BoxArtCacheDir.isEmpty());
    return s_BoxArtCacheDir;
}

QByteArray Path::readDataFile(QString fileName)
{
    QFile dataFile(getDataFilePath(fileName));
    dataFile.open(QIODevice::ReadOnly);
    return dataFile.readAll();
}

QString Path::getDataFilePath(QString fileName)
{
    QString candidatePath;

    // Check the current directory first
    candidatePath = QDir(QDir::currentPath()).absoluteFilePath(fileName);
    if (QFile::exists(candidatePath)) {
        qInfo() << "Found" << fileName << "at" << candidatePath;
        return candidatePath;
    }

    // Now check the data directories (for Linux, in particular)
    candidatePath = QStandardPaths::locate(QStandardPaths::DataLocation, fileName);
    if (!candidatePath.isEmpty() && QFile::exists(candidatePath)) {
        qInfo() << "Found" << fileName << "at" << candidatePath;
        return candidatePath;
    }

    // Now try the directory of our app installation (for Windows, if current dir doesn't find it)
    candidatePath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(fileName);
    if (QFile::exists(candidatePath)) {
        qInfo() << "Found" << fileName << "at" << candidatePath;
        return candidatePath;
    }

    // Return the QRC embedded copy
    candidatePath = ":/data/" + fileName;
    qInfo() << "Found" << fileName << "at" << candidatePath;
    return QString(candidatePath);
}

void Path::initialize(bool portable)
{
    if (portable) {
        initializeWithPath(QDir::currentPath());
    }
    else {
        s_LogDir = QDir::tempPath();
        s_BoxArtCacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/boxart";
    }
}

void Path::initializeWithPath(const QString & path)
{
    s_LogDir = path + "/log";
    s_BoxArtCacheDir = path + "/boxart";
}
