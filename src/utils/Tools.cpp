//
// Created by yzqlwt on 2020/11/28.
//
#include "Tools.h"
#include "../http/httplib.h"
#include <curl/curl.h>
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

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


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
		QByteArray fileMsg = file.readAll();
		QByteArray md5 = QCryptographicHash::hash(fileMsg, QCryptographicHash::Md5).toHex();
		file.close();
		return md5;
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

QString Tools::Download(const QString& uri, const QString& path) {
    FILE* fp = fopen(path.toStdString().c_str(), "wb");

    //curl初始化
    CURL* curl = curl_easy_init();
    // curl返回值
    CURLcode res;
    if (curl)
    {
        //设置curl的请求头
        struct curl_slist* header_list = NULL;
        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

        //不接收响应头数据0代表不接收 1代表接收
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);

        //设置请求的URL地址
        curl_easy_setopt(curl, CURLOPT_URL, uri.toStdString().c_str());

        //设置ssl验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

        //设置数据接收函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        //设置超时时间
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

        // 开启请求
        res = curl_easy_perform(curl);
    }
    // 释放curl
    curl_easy_cleanup(curl);
    //释放文件资源
    fclose(fp);
    return path;
}

