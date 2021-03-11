//
// Created by yzqlwt on 2020/11/5.
//

#ifndef EASYSTUDIO_TOOLS_H
#define EASYSTUDIO_TOOLS_H
#include <string>
#include <vector>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QVector>


class Tools {
public:
    static QString GetMd5(const QString& file);
    static QString ReadFile(QString path);
    static void WriteFile(const QString& path, const QString& content);
    static QStringList GetAllMatchResults(const QString& text, const QString& regexp);
    static QString GetPlistName(const QString& md5);
    static QString Download(const QString& uri, const QString& path);
    static QString GetClipboardFiles();
    static void GotoFolder(const QString& path);
};


#endif //EASYSTUDIO_TOOLS_H
