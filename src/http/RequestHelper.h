//
// Created by yzqlwt on 2020/11/28.
//

#ifndef EASYSTUDIO_REQUESTHELPER_H
#define EASYSTUDIO_REQUESTHELPER_H

#include <QtCore/QString>
#include <string>

class RequestHelper {
public:
    static QString GetAttachments(const QString& itemId, const QString& itemType);
    static QString DownloadZip(const QString& uri, const QString& savePath);
    static QString GetResZipUri(QString& attachments);
    static void UploadFile(const QString &path, const QString &itemId);
    static void        AddResConfig(const QString& itemId, const QString& attachmentId);
    static void        ModifyResConfig(const QString& id, const QString& itemId, const QString& attachmentId);
    static QString GetResConfigId(const QString &itemId, const QString &itemType);
	static QString Download(const QString& uri, const QString& path);

private:
    static QString Get(const QString& uri);
};

#endif //EASYSTUDIO_REQUESTHELPER_H
