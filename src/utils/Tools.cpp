//
// Created by yzqlwt on 2020/11/28.
//
#include "Tools.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <openssl/md5.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <fstream>
#include <regex>
#include <tchar.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QCryptographicHash>


QString Tools::GetMd5(const QString& path){
    auto content = Tools::ReadFile(path);
	auto bb = QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5);
	return QString(bb.toHex());
}


QString Tools::ReadFile(QString path) {
	QFileInfo fileInfo(path);
	if (fileInfo.isFile()) {
		QFile file(path);
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		return QString(file.readAll());
	}
    return "";
}


QString Tools::GetPlistName(const std::vector<QString> &files, const QString &md5) {
    for (int i = 0; i < files.size(); ++i) {
        auto file = files[i];
        auto content = Tools::ReadFile(file);
        if (content.contains(md5))
        {
            QFileInfo fileInfo(file);
            return fileInfo.baseName();
        }
    }
    return QString();
}

void Tools::WriteFile(const QString &path, const QString &content) {
    try {

		QFile file(path);
		//方式：Append为追加，WriteOnly，ReadOnly
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out.flush();
			file.close();
        }
        else {
            qDebug() << "Write File Error:" << path;
        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}

