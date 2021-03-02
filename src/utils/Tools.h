//
// Created by yzqlwt on 2020/11/5.
//

#ifndef EASYSTUDIO_TOOLS_H
#define EASYSTUDIO_TOOLS_H
#include <string>
#include <vector>
#include <QtCore/QString>
#include <QtCore/QDebug>


class Tools {
public:
    static QString GetMd5(const QString& file);
    static QString ReadFile(QString path);
    static void WriteFile(const QString& path, const QString& content);
    static QString GetPlistName(const std::vector<QString>& files, const QString& md5);
    static QString Download(const QString& uri, const QString& path);
};


#endif //EASYSTUDIO_TOOLS_H
