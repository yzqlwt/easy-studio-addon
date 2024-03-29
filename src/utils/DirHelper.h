#pragma once

#include "../AppConfig.h"
#include <napi.h>
#include <QtCore/qdir.h>
#include <QtCore/qstring.h>
#include <QtCore/QDebug>
#include <QtCore/QDirIterator>
#include <quazip5/JlCompress.h>
class DirHelper {
public:

	static void ClearDir(const QString& path) {
		QDir dir(path);
		if (dir.exists()) {
			dir.removeRecursively();
		}
		dir.mkpath(path);
	}

	static QString GetUserPath() {
		return QDir::homePath();
	}

	static QString GetCachePath() {
		auto userPath = GetUserPath();
		auto path = QString("%1/%2/%3/%4").arg(userPath, "AppData", "Local","EasyStudio");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetDownloadPath() {
		auto userPath = GetUserPath();
		auto path = QString("%1/%2/%3/%4/%5").arg(userPath, "AppData", "Local", "EasyStudio", "download");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetTokenPath() {
		auto cachePath = GetCachePath();
		auto path = QString("%1/%2").arg(cachePath, "token.json");
		QFileInfo info(path);
		if (info.isFile()) {
			return path;
		}
		return nullptr;
	}

	static QString GetTempDir() {
		auto cachePath = GetCachePath();
		auto path = QString("%1/%2").arg(cachePath, "temp");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetHistoryDir() {
		auto cachePath = GetCachePath();
		auto path = QString("%1/%2").arg(cachePath, "history");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}
	
	static QString GetImagesCachePath() {
		auto cachePath = GetCachePath();
		auto path = QString("%1/%2").arg(cachePath, "tiny");
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetSkinFullPath() {
		QDir projectPath(AppConfig::GetInstance().GetCCSPath());
		projectPath.cdUp();
		auto path = QString("%1/%2/%3").arg(projectPath.absolutePath(), "cocosstudio", AppConfig::GetInstance().GetSkinPath());
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		path.replace("\\","/"); 
		return path;
	}

	static QString GetAssetsFullPath() {
		QDir projectPath(AppConfig::GetInstance().GetCCSPath());
		projectPath.cdUp();
		auto path = QString("%1/%2/%3").arg(projectPath.absolutePath(), "easystudio", AppConfig::GetInstance().GetSkinPath());
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		path.replace("\\","/");
		return path;
	}

	static QString GetOutputFullPath() {
		QDir projectPath(AppConfig::GetInstance().GetCCSPath());
		projectPath.cdUp();
		auto path = QString("%1/%2/%3").arg(projectPath.absolutePath(), "output", AppConfig::GetInstance().GetSkinPath());
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		path.replace("\\","/");
		return path;
	}

	static QString GetFileDataPath(const QString& path) {
		QDir projectPath(AppConfig::GetInstance().GetCCSPath());
		projectPath.cdUp();
		auto file = QString("%1/%2/%3").arg(projectPath.absolutePath(), "cocosstudio", path);
		return file;
	}

	static QString GetMangoCachePath() {
		auto userPath = GetUserPath();
		auto path = QString("%1/%2/%3/%4/%5/%6").arg(userPath, "AppData", "Local", "mango", "qmath-master_dev", AppConfig::GetInstance().GetSkinPath());
		QDir dir(path);
		if (!dir.exists()) {
			dir.mkpath(path);
		}
		return path;
	}

	static QString GetFolder(const QString& path) {
		QDir dir(path);
		auto  list = dir.entryList(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		return list.join(",");
	}

	static QFileInfoList GetFilesRecursive(const QString& path, const QString& extension = "*.*")
	{
		QFileInfoList list;
		QDirIterator iter(path, QStringList() << extension,
			QDir::Files | QDir::NoSymLinks,
			QDirIterator::Subdirectories);
		while (iter.hasNext())
		{
			iter.next();
			list.push_back(iter.filePath());
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

	static Napi::Value getOutputFullPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto path = DirHelper::GetOutputFullPath();
		return Napi::String::New(env, path.toStdString().c_str());
	}


	static Napi::Value getFolder(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto list = DirHelper::GetFolder(info[0].As<Napi::String>().ToString().Utf8Value().c_str());
		return Napi::String::New(env, list.toStdString().c_str());
	}

	static Napi::Value setTexturePackerPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto list = DirHelper::GetFolder(info[0].As<Napi::String>().ToString().Utf8Value().c_str());
		return env.Null();
	}

	static Napi::Value gotoHistory(const Napi::CallbackInfo& info) {
		auto path = DirHelper::GetHistoryDir();
		system(QString("start %1").arg(path).toStdString().c_str());
		return info.Env().Null();
	}

	static Napi::Value gotoMangoCache(const Napi::CallbackInfo& info) {
		auto path = DirHelper::GetMangoCachePath();
		system(QString("start %1").arg(path).toStdString().c_str());
		return info.Env().Null();
	}
	
};

