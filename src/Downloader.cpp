#include "Downloader.h"
#include "http/RequestHelper.h"
#include "utils/DirHelper.h"
#include "utils/PlistParser.h"
#include "AppConfig.h"
#include <QtCore/QFile>
#include <nlohmann/json.hpp>
#include <quazip5/JlCompress.h>

const static QString StaticUrl = "https://gstatic-cn.oss-cn-hangzhou.aliyuncs.com";


QString Downloader::Download()
{
    qDebug() << "Download1";
	QDir dir(DirHelper::GetDownloadPath());
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
	auto path = RequestHelper::DownloadZip(uri, DirHelper::GetDownloadPath());
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
    QStringList dir_list = QDir(tempPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
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
	if (dir_list.size() > 0)
	{
		OldVersionParse();
	}
}

void Downloader::OldVersionParse()
{
	auto tempPath = DirHelper::GetTempDir();
	auto assetsPath = DirHelper::GetAssetsFullPath();
	auto files = DirHelper::GetFilesRecursive(tempPath, "*.plist");
	auto path = tempPath + "/pngs";
	QDir dir(path);
	if (!dir.exists()) {
		dir.mkpath(path);
	}
	for (auto i = 0; i < files.size(); i++) {
		auto file = files[i].absoluteFilePath();
		auto pngPath = file;
		pngPath = pngPath.replace(".plist", ".png");
		auto parser = new PlistParser();
		parser->load(pngPath.toStdString(),file.toStdString());
		parser->save(path.toStdString());
	}
	auto configPath = tempPath + "/ResConfig.json";
	QFileInfo info(configPath);
	if (!info.isFile()) {
		qDebug() << "������ResConfig";
		return;
	}
	auto content = Tools::ReadFile(configPath);
	auto config = nlohmann::json::parse(content.toStdString());
	auto resources = config["resource"].get<std::map<std::string, nlohmann::json>>();
	for each (auto item in resources)
	{
		auto itemConfig = item.second;
		auto extension = itemConfig["Extension"].get<std::string>();
		if (extension == ".png")
		{
			auto name = itemConfig["Name"].get<std::string>();
			auto md5 = itemConfig["Md5"].get<std::string>();
			auto imagePath = path + "/" + md5.c_str() + ".png";
			auto targetFile = DirHelper::GetSkinFullPath() + name.c_str();
			if (!QFile::exists(targetFile))
			{
				QFile::copy(imagePath, targetFile);
			}
		}
	}
}
