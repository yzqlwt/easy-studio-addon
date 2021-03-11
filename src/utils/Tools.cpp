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

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}


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


QString Tools::GetPlistName(const QString &md5) {
    auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetOutputDir(), "*.plist");
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
		//��ʽ��AppendΪ׷�ӣ�WriteOnly��ReadOnly
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
	/* ��������ƥ�䡣 */
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

QString Tools::Download(const QString& uri, const QString& path) {
    FILE* fp = fopen(path.toStdString().c_str(), "wb");

    //curl��ʼ��
    CURL* curl = curl_easy_init();
    // curl����ֵ
    CURLcode res;
    if (curl)
    {
        //����curl������ͷ
        struct curl_slist* header_list = NULL;
        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

        //��������Ӧͷ����0�������� 1�������
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);

        //���������URL��ַ
        curl_easy_setopt(curl, CURLOPT_URL, uri.toStdString().c_str());

        //����ssl��֤
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        //CURLOPT_VERBOSE��ֵΪ1ʱ������ʾ��ϸ�ĵ�����Ϣ
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

        //�������ݽ��պ���
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        //���ó�ʱʱ��
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

        // ��������
        res = curl_easy_perform(curl);
    }
    // �ͷ�curl
    curl_easy_cleanup(curl);
    //�ͷ��ļ���Դ
    fclose(fp);
    return path;
}

void Tools::GotoFolder(const QString& path) {
    QFileInfo info(path);
    if (info.isDir()) {
        system(QString("start %1").arg(path).toStdString().c_str());
    }
}

