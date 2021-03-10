#include "Package.h"
#include "utils/DirHelper.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include <iostream>


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

void PackageHelper::Package() {
	HandleImages();
}

void PackageHelper::HandleImages() {
	auto maps = std::map<QString, QString>();
	std::cout << DirHelper::GetSkinFullPath().toStdString() << std::endl;
	auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), "*.png");
	auto needTinyFiles = QStringList();
	for (auto info : filesInfo) {
		auto md5 = Tools::GetMd5(info.absoluteFilePath());
		maps[md5] = info.absoluteFilePath();
		QFileInfo info(QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
		if (!info.isFile()) {
			needTinyFiles.push_back(info.absoluteFilePath());
		}
	}
	auto size = needTinyFiles.size();
	nlohmann::json json;
	for (auto i = 0; i < size; i++)
	{
		auto file = needTinyFiles[i];
		QFileInfo info(file);
		auto name = info.fileName();
		json["status"] = "process";
		json["title"] = QString("Compress:%1/%2").arg(i + 1).arg(size).toStdString();
		json["description"] = file.toStdString();
		Emit("tiny", json.dump().c_str());
		Tiny(file);
	}
	json["status"] = "finish";
	json["title"] = QString("Compress: success").toStdString();
	Emit("tiny", json.dump().c_str());
	auto tempPath = DirHelper::GetTempDir();
	for (auto item : maps) {
		auto md5 = item.first;
		auto file = item.second;
		QFile::copy(QString("%1/%2.png").arg(DirHelper::GetImagesCachePath(), md5), QString("%1/%2.png").arg(tempPath, md5));
	}
}

 QString PackageHelper::Tiny(const QString& path) {
	 std::cout << path.toStdString() << std::endl;
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
				//qDebug() << "—πÀı¥ÌŒÛ£∫" << res->body;
				return Tiny(path);
			}
			else {
				auto url = json["output"]["url"].get<std::string>();
				//œ¬‘ÿ
				return Tools::Download(url.c_str(), QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
			}
		}
	}
	return "";
}

void PackageHelper::TexturePackage(const QString& res_path, const QString& targetPath) {
	char str[2048];
	auto appPath = AppConfig::GetInstance().GetTPPath();
	auto cmdStr = QString("%1 %2 --sheet %3/__frame_{n1}.png --data %3/__frame_{n1}.plist --allow-free-size --no-trim --max-size 2048 --format cocos2d --multipack --extrude 4").arg(appPath, res_path, targetPath);
	qDebug() << cmdStr;
	system(str);
}

void PackageHelper::Emit(const QString& type, const QString& content)
{
	this->_func->Call(
		{ Napi::String::New(*this->_env, type.toStdString().c_str()), Napi::String::New(*this->_env, content.toStdString().c_str()) });

}
