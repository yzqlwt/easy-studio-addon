#pragma once
#include "utils/DirHelper.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include <map>
#include <nlohmann/json.hpp>

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

static Napi::Env* mEnv;
static Napi::Function* mFunc;

class PackageHelper {
public:

	static Napi::Value package(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		Napi::Function func = info[0].As<Napi::Function>();
		mEnv = &env;
		mFunc = &func;
		Package();
		return env.Null();
	}

	static void EmitWarn(const QString& content) {
		mFunc->Call(
			{ Napi::String::New(*mEnv, "warn"), Napi::String::New(*mEnv, content.toStdString().c_str()) });
	}

	static void EmitSteps(const QString& content) {
		mFunc->Call(
			{ Napi::String::New(*mEnv, "steps"), Napi::String::New(*mEnv, content.toStdString().c_str()) });
	}

	static void EmitError(const QString& content) {
		mFunc->Call(
			{ Napi::String::New(*mEnv, "error"), Napi::String::New(*mEnv, content.toStdString().c_str()) });
	}

	static void EmitTiny(const QString& content) {
		mFunc->Call(
			{ Napi::String::New(*mEnv, "tiny"), Napi::String::New(*mEnv, content.toStdString().c_str()) });
	}

	static void Package() {
		HandleImages();
	}

	static void HandleImages() {
		auto maps = std::map<QString, QString>();
		auto filesInfo = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), ".png");
		auto needTinyFiles = QStringList();
		for (auto info : filesInfo) {
			auto md5 = Tools::GetMd5(info.absolutePath());
			maps[md5] = info.absolutePath();
			QFileInfo info(QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
			if (!info.isFile()) {
				needTinyFiles.push_back(info.absolutePath());
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
			json["title"] = QString("Compress:%1/%2").arg(i+1).arg(size).toStdString();
			json["description"] = name.toStdString();
			EmitTiny(json.dump().c_str());
			Tiny(file);
		}
		json["status"] = "finish";
		json["title"] = QString("Compress: success").toStdString();
		EmitTiny(json.dump().c_str());
		auto tempPath = DirHelper::GetTempDir();
		for (auto item : maps) {
			auto md5 = item.first;
			auto file = item.second;
			QFile::copy(QString("%1/%2.png").arg(DirHelper::GetImagesCachePath(), md5), QString("%1/%2.png").arg(tempPath, md5));
		}
	}

	static QString Tiny(const QString& path) {
		httplib::Client cli("https://api.tinify.com");
		auto index = rand() % Keys.size();
		auto token = QString("Basic ") +((QString("api:")+Keys[index])).toUtf8().toBase64() ;
		
		cli.set_default_headers({
										{"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"},
										{"Authorization", token.toStdString()}
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
					return Tools::Download(url.c_str(), QString("%1/%2%3").arg(DirHelper::GetImagesCachePath(), md5, ".png"));
				}
			}
		}
		return "";
	}

	void TexturePackage(const QString& res_path, const QString& targetPath) {
		char str[2048];
		auto appPath = QDir::currentPath();
		auto path = QString("%1/%2/%3").arg(appPath, "TexturePacker", "TexturePacker.exe");
		auto cmdStr = QString("%1 %2 --sheet %3/__frame_{n1}.png --data %3/__frame_{n1}.plist --allow-free-size --no-trim --max-size 2048 --format cocos2d --multipack --extrude 4").arg(path, res_path, targetPath);
		system(str);
	}

	static Napi::Value handleImages(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		PackageHelper::HandleImages();
		return env.Null();
	}
};