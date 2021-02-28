#pragma once

#include "../AppConfig.h"
#include <napi.h>
#include <filesystem>
#include <QtCore/qdir.h>
#include <QtCore/qstring.h>
class DirHelper {
public:
	static QString GetUserPath() {
		return QDir::homePath();
	}

	static QString GetCachePath() {
		auto userPath = GetUserPath();
		auto path = QString("%1/%2/%3").arg(userPath, "EasyStudio");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetSkinPath() {
		return AppConfig::SKIN_PATH;
	}

	static QString GetCCSPath() {
		return AppConfig::CCS_PATH;
	}


	static QString GetSkinFullPath() {
		QDir projectPath(AppConfig::CCS_PATH);
		projectPath.cdUp();
		auto path = QString("%1/%2/%3").arg(projectPath.absolutePath(), "cocosstudio", AppConfig::SKIN_PATH);
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetAssetsFullPath() {
		QDir projectPath(AppConfig::CCS_PATH);
		projectPath.cdUp();
		auto path = QString("%1/%2/%3").arg(projectPath.absolutePath(), "easystudio", AppConfig::SKIN_PATH);
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QStringList GetFilesRecursive(const QString& path, const QString& extension = ".*") {
		QStringList list;
		for (auto& fe : std::filesystem::recursive_directory_iterator(path.toStdString()))
		{
			auto filePath = fe.path();
			if (std::filesystem::is_regular_file(filePath) && extension == ".*" || filePath.extension() == extension.toStdString())
				list.push_back(QString(filePath.string().c_str()));
		}

		return list;
	}

	static Napi::Value getSkinFullPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto path = DirHelper::GetSkinFullPath();
		return Napi::String::New(env, path.toStdString().c_str());
	}

	static Napi::Value getAssetsFullPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto path = DirHelper::GetAssetsFullPath();
		return Napi::String::New(env, path.toStdString().c_str());
	}
	
};

