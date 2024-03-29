#include "Package.h"
#include "utils/DirHelper.h"
#include "http/RequestHelper.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include "csd/FlatBuffersSerialize.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <QtCore/QDateTime>
#include <QtCore/QProcess>


static std::vector<QString> Keys = {
		"q3ZYnPZ9BcBcf3T91X5yppdvW0qMSBhk",
		"shsEABw40_1DasWNfxDhtqvb-fpwVnEG",
		"tSeWVzvsLPuBXXnIUe9vVGJhMW0VcoVT",
		"g8EyUYESWC15bPc8n4XozCfhp1Aj-jeH",
		"_gUJq__T2WqRuaxMqicsQ_haR4tVyoot",
		"5uHc16uIMwZQQhbqWIpWfsYe2wKjQZTl",
		"erJdW_5ulXIUfqVQDYT9lBPkuxUT3l9p",
		"tTmNjy02hfe6zraAbagT_9HRqUZIK08l",
		"wcQ9z-XPp9HWx7ORkR35VuJ8T31bMlSQ",
		"05n6dNVJz0d9xTHMl3hp1l8SKtmwDdVx",
		"ufmUtYfhFQ2wEzfWxHKBldV8JLUfp9yk",
		"7KH0JJvMX1EoasyqzXS-hNRfW87wmv-n",
		"c0wF8VFyBMs8nd6lmCnTsBTqSlL79ZV0",
		"yCCZ0DwGY722kGkbnFH9TfFSfZLsRKtK",
		"7x4fOqyd2QQ7LD2rmozYqN55C7XHedYY",
		"M3_KkLtnxcByk9f_WeUGc-rW-Th66b9Z",
		"hEbz1r41yTP_CaKx8QXx0j5OAKZJg9CL",
		"YwWQqTbp7ezihjM8DJ2wzgpEDRRmkuap",
		"MGgJYCh4aZ0hga7rU7coWmJwT4_-lFON",
		"f3yJMWP6X3RMNr0s2cGsLTgRj9q9smhK",
};

void PackageHelper::Init() {
	resources.clear();
	auto tempPath = DirHelper::GetTempDir();
	DirHelper::ClearDir(tempPath);
	auto outputDir = DirHelper::GetOutputFullPath();
	DirHelper::ClearDir(outputDir);
}

QString PackageHelper::Package() {
	Init();
	HandleImages();
	auto error = TexturePackage();
	if (error != "") {
		return error;
	}
	HandleCSD();
	HandleAssets();
	auto outputDir = DirHelper::GetOutputFullPath();
	auto files = DirHelper::GetFilesRecursive(outputDir, "*.plist");
	auto configPath = outputDir + "/ResConfig.json";
	nlohmann::json config;
	config["resource"] = resources;
	config["version"] = 3;
	nlohmann::json plists;
	for (auto file : files) {
		QFileInfo info(file);
		plists.push_back(info.baseName().toStdString());
	}
	config["plist"] = plists;
	Tools::WriteFile(configPath, config.dump().c_str());
	return "success";
}

void PackageHelper::HandleImages() {
	auto tempPath = DirHelper::GetTempDir();
	auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), "*.png");
	auto size = filesInfo.size();
	nlohmann::json json;
	for (auto i = 0; i < size; i++)
	{
		auto info = filesInfo[i];
		auto fullPath = info.absoluteFilePath();
		auto md5 = Tools::GetMd5(fullPath);
		auto path = Tiny(fullPath);
		QFileInfo tinyinfo(QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
		if (!tinyinfo.isFile()) {
			std::cout << "file not found in cache:" << tinyinfo.fileName().toStdString() << std::endl;
			QFile::copy(fullPath, QString("%1/%2.png").arg(tempPath, md5));
		}
		else {
			QFile::copy(path, QString("%1/%2.png").arg(tempPath, md5));
		}
		auto config = this->GetItemConfig(fullPath);
		this->resources[config.first] = config.second;
	}
}

void PackageHelper::HandleCSD()
{
	auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), "*.csd");
	auto size = filesInfo.size();
	for (auto i = 0; i < size; i++)
	{
		auto info = filesInfo[i];
		auto content = Tools::ReadFile(info.absoluteFilePath());
		auto results = Tools::GetAllMatchResults(content, "Type=\"Normal\" Path=\"res.*png\" Plist=\"\"");
		for (auto result : results) {
			auto file = Tools::GetAllMatchResults(result, "res/ui.*png")[0];
			auto path = DirHelper::GetFileDataPath(file);
			QFileInfo info(path);
			if (info.isFile())
			{
				auto md5 = Tools::GetMd5(info.absoluteFilePath());
				auto newPath = QString("Type=\"PlistSubImage\" Path=\"%1\" Plist=\"%3%2.plist\"").arg(md5 + ".png").arg(Tools::GetPlistName(md5)).arg(AppConfig::GetInstance().GetSkinPath());
				content.replace(result, newPath);
			}
		}
		auto newPath = info.absolutePath() + "/" + info.baseName() + ".easy";
		Tools::WriteFile(newPath, content);
		auto outputDir = DirHelper::GetOutputFullPath();
		auto csbPath = outputDir + "/" + info.baseName() + ".csb";
		FlatBuffersSerialize::getInstance()->serializeFlatBuffersWithXML(content.toStdString(), csbPath.toStdString());
		auto config = this->GetItemConfig(csbPath);
		this->resources[config.first] = config.second;
	}
}

void PackageHelper::HandleAssets()
{
	auto outputDir = DirHelper::GetOutputFullPath();
	auto assetsPath = DirHelper::GetAssetsFullPath();
	auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetAssetsFullPath());
	for (auto file : filesInfo) {
		auto fullPath = file.absoluteFilePath();
		auto config = this->GetItemConfig(fullPath);
		this->resources[config.first] = config.second;
		auto targetPath = QString("%1/%2").arg(outputDir, file.fileName());
		if (QFile(targetPath).exists()) {
			QFile::remove(targetPath);
		}
		QFile::copy(fullPath, targetPath);
	}
}

void PackageHelper::Upload()
{
	auto path = PackageHelper::GetInstance().Compress();
	RequestHelper::UploadFile(path, AppConfig::GetInstance().GetSkinId());
}


QString PackageHelper::Compress()
{
	auto historyDir = DirHelper::GetHistoryDir();
	auto outputDir = DirHelper::GetOutputFullPath();
	auto skinPath = DirHelper::GetSkinFullPath();
	auto gameResults = Tools::GetAllMatchResults(skinPath, "(?<=games/)game\\d{2,4}");
	auto skinResults = Tools::GetAllMatchResults(skinPath, "skin\\d{1,100}");
	auto gameid = QString();
	auto skinId = QString();
	if (gameResults.size() > 0) {
		gameid = gameResults[0]+"_";
	}
	if (skinResults.size() > 0) {
		skinId = skinResults[0]+ "_";
	}
	QDateTime dateTime(QDateTime::currentDateTime());
	QString time = dateTime.toString("yyyy-MM-dd--hh-mm-ss");
	auto path = QString("%1/%2%3%4.zip").arg(historyDir, gameid, skinId, time);
	JlCompress::compressDir(path, outputDir);
	return path;
}

QString PackageHelper::GetNeedTinyFiles()
{
	auto tempPath = DirHelper::GetTempDir();
	auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), "*.png");
	auto size = filesInfo.size();
	QStringList list;
	for (auto i = 0; i < size; i++)
	{
		auto info = filesInfo[i];
		auto fullPath = info.absoluteFilePath();
		auto md5 = Tools::GetMd5(fullPath);
		QFileInfo tinyinfo(QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
		if (!tinyinfo.isFile()) {
			list.push_back(fullPath);
		}
		else {
			auto fileFullPath = tinyinfo.absoluteFilePath();
			cv::Mat img = cv::imread(tinyinfo.absoluteFilePath().toStdString());
			if (img.empty() || !img.data) {
				list.push_back(fullPath);
				QFile::remove(fileFullPath);
			}
		}
	}
	return list.join(",");
}


std::pair<std::string, nlohmann::json> PackageHelper::GetItemConfig(const QString& path)
{
	nlohmann::json config;
	QFileInfo info(path);
	auto extension = "."+info.suffix();
	auto name = QString("");
	if (extension == ".png") {
		config["Md5"] = Tools::GetMd5(path).toStdString();
		auto skinFullPath = DirHelper::GetSkinFullPath();
		auto assetsFullPath = DirHelper::GetAssetsFullPath();
		name = QString(path).replace(assetsFullPath, "");
		name = QString(name).replace(skinFullPath, "");
	}
	else if (extension == ".csb") {
		auto outputFullPath = DirHelper::GetOutputFullPath();
		name = info.fileName();
	}
	else {
		auto assetsFullPath = DirHelper::GetAssetsFullPath();
		name = QString(path).replace(assetsFullPath, "");
	}
	config["Extension"] = extension.toStdString();
	config["Name"] = name.toStdString();
	return std::make_pair(name.toStdString(), config);
}

//改为node请求
 QString PackageHelper::Tiny(const QString& path) {
	 auto md5 = Tools::GetMd5(path);
	QFileInfo info(QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
	if (info.isFile()) {
		return info.absoluteFilePath();
	}
	httplib::Client cli("https://api.tinify.com");
	auto index = rand() % Keys.size();
	auto token = QString("Basic ") + ((QString("api:") + Keys[index])).toUtf8().toBase64();
	cli.set_default_headers({
		{ "User-Agent", "Chrome/86.0.4240.198 Safari/537.36" },
		{ "Authorization", token.toStdString() }
		});
	QFileInfo fileInfo(path);
	if (fileInfo.isFile()) {
		auto md5 = Tools::GetMd5(path);
		std::ifstream ifs(path.toStdString(), std::ios::in | std::ios::binary);
		std::stringstream ss;
		ss << ifs.rdbuf();
		ifs.close();
		std::string content(ss.str());
		auto res = cli.Post("/shrink", content, "image/png");
		if (nlohmann::json::accept(res->body)) {
			auto json = nlohmann::json::parse(res->body);
			if (res->body.find("error") != std::string::npos) {
				//tiny png error retry!
				//qDebug() << "ѹ������" << res->body;
				return Tiny(path);
			}
			else {
				auto url = json["output"]["url"].get<std::string>();
				//����
				return RequestHelper::Download(url.c_str(), QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
			}
		}
	}
	return "";
}

QString PackageHelper::TexturePackage() {
	//auto outputDir = DirHelper::GetOutputFullPath();
	//auto tempDir = DirHelper::GetTempDir();
	//auto appPath = AppConfig::GetInstance().GetTPPath();
	//auto cmdStr = QString("%1 %2 --sheet %3/__frame_{n1}.png --data %3/__frame_{n1}.plist --allow-free-size --no-trim --max-size 2048 --format cocos2d --multipack --extrude 2").arg(appPath, tempDir, outputDir);
	//qDebug() << cmdStr;
	//system(cmdStr.toStdString().c_str());

	auto outputDir = DirHelper::GetOutputFullPath();
	auto tempDir = DirHelper::GetTempDir();
	auto appPath = AppConfig::GetInstance().GetTPPath();
	auto args2 = QString(" %1 --sheet %2/__frame_{n1}.png --data %2/__frame_{n1}.plist --allow-free-size --no-trim --max-size 2048 --format cocos2d --multipack --extrude 2").arg(tempDir, outputDir);
	QStringList args;
	args.append(tempDir);
	args.append("--sheet");
	args.append(outputDir+"/__frame_{n1}.png");
	args.append("--data");
	args.append(outputDir + "/__frame_{n1}.plist");
	args.append("--allow-free-size");
	args.append("--no-trim");
	args.append("--max-size");
	args.append("2048");
	args.append("--format");
	args.append("cocos2d");
	args.append("--multipack");
	args.append("--extrude");
	args.append("2");
	QProcess p(0);
	qDebug() << args;
	p.start(appPath, args);
	p.waitForFinished();//等待完成
	auto error = QString::fromLocal8Bit(p.readAllStandardError());
	auto output = QString::fromLocal8Bit(p.readAllStandardOutput());
	qDebug() << "TexturePackager Error:"<< error;
	qDebug() << "TexturePackager Log:" << output;
	return error;
}


