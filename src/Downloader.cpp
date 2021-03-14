#include "Downloader.h"
#include "http/RequestHelper.h"
#include "utils/DirHelper.h"
#include "AppConfig.h"
#include <QtCore/QFile>
#include <quazip5/JlCompress.h>

const static QString StaticUrl = "https://gstatic-cn.oss-cn-hangzhou.aliyuncs.com";

QString Downloader::Download()
{
    QDir dir(DirHelper::GetCachePath());
    QStringList nameFilters;
    nameFilters << "*.zip";
    auto files = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for (auto file : files) {
        QFile::remove(file.absoluteFilePath());
    }
    auto uri = RequestHelper::GetResZipUri();
    if (uri == "") {
        return "";
    }
    auto path = RequestHelper::DownloadZip(uri, DirHelper::GetCachePath());
    this->_zipPath = path;
    return path;
}

void Downloader::Parse()
{
    QFileInfo info(this->_zipPath);
    if (!info.isFile()) return;
    auto tempPath = DirHelper::GetTempDir();
    DirHelper::ClearDir(tempPath);
    JlCompress::extractDir(info.absoluteFilePath(), tempPath);
    auto assetsPath = DirHelper::GetAssetsFullPath();
    auto files = DirHelper::GetFilesRecursive(tempPath);
    for (auto i = 0; i < files.size(); i++) {
        auto file = files[i];
        auto extension = file.suffix();
        if (extension == "png" || extension == "plist" || extension == "csb") continue;
        if (file.fileName() == "ResConfig.json") continue;
        auto targetPath = QString("%1/%2").arg(assetsPath, file.fileName());
        if (QFile(targetPath).exists()) {
            QFile::remove(targetPath);
        }
        QFile::copy(file.absoluteFilePath(), targetPath);
    }
}
