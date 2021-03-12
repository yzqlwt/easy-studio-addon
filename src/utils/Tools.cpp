//
// Created by yzqlwt on 2020/11/28.
//
#include "Tools.h"
#include "DirHelper.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <openssl/md5.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <fstream>
#include <regex>
#include <tchar.h>
#include <cstdlib>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QCryptographicHash>
#include <QtCore/qurl.h>
#include <QtCore/QRegularExpression>


QString Tools::GetMd5(const QString& path){
	QFile theFile(path);
	theFile.open(QIODevice::ReadOnly);
	QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
	theFile.close();
	return ba.toHex().constData();
}


QString Tools::ReadFile(QString path) {
	QFileInfo fileInfo(path);
	if (fileInfo.isFile()) {
		QFile file(path);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
        auto content =  file.readAll();
        file.close();
        return content;
	}
    return "";
}

QString Tools::GetToken()
{
    auto path = DirHelper::GetTokenPath();
    auto token = QString();
    if (path!=nullptr)
    {
        auto content = Tools::ReadFile(path);
		nlohmann::json json = nlohmann::json::parse(content.toStdString());
		auto time = json["expires_in"].get<long long>();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
			);
		if (time - ms.count() > 36000) {
			return json["access_token"].get<std::string>().c_str();
		}
		else {
			std::cout << "token expired" << std::endl;
		}
    }
    return token;
}


QString Tools::GetPlistName(const QString &md5) {
    auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetOutputFullPath(), "*.plist");
    for (int i = 0; i < filesInfo.size(); ++i) {
        auto info = filesInfo[i];
        auto content = Tools::ReadFile(info.absoluteFilePath());
        if (content.contains(md5))
        {
            return info.baseName();
        }
    }
    return QString();
}

void Tools::WriteFile(const QString &path, const QString &content) {
    QFile file(path);
    try {
		//方式：Append为追加，WriteOnly，ReadOnly
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(content.toStdString().c_str());
        }
        else {
            qDebug() << "Write File Error:" << path;
        }
        
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    file.close();
}

QStringList Tools::GetAllMatchResults(const QString& content, const QString& regexp)
{
    QRegularExpression re(regexp);
	QRegularExpressionMatchIterator iter = re.globalMatch(content);

    QStringList list;
	/* 遍历查找匹配。 */
	while (iter.hasNext()) {
		QRegularExpressionMatch match = iter.next();
        list.push_back(match.captured(0));
	}
    return list;
}

QString Tools::GetClipboardFiles() {
    QStringList list;
	if (OpenClipboard(NULL)) // open clipboard
	{
		HDROP hDrop = HDROP(::GetClipboardData(CF_HDROP)); // get the file path hwnd of clipboard
		if (hDrop != NULL)
		{
			char szFilePathName[MAX_PATH + 1] = { 0 };
			UINT nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // get the count of files
			for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
			{
				memset(szFilePathName, 0, MAX_PATH + 1);
				DragQueryFile(hDrop, nIndex, szFilePathName, MAX_PATH); // get file name
                list.push_back(QString::fromLocal8Bit(szFilePathName));
			}
		}
		CloseClipboard(); // close clipboard	
	}
	//}
    return list.join(",");
}


void Tools::GotoFolder(const QString& path) {
    QFileInfo info(path);
    if (info.isDir()) {
        system(QString("start %1").arg(path).toStdString().c_str());
    }
}

