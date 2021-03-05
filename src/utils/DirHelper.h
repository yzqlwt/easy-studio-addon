#pragma once

#include "../AppConfig.h"
#include <napi.h>
#include <QtCore/qdir.h>
#include <QtCore/qstring.h>
#include <QtCore/QDebug>
class DirHelper {
public:
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

	static QString GetTempDir() {
		auto cachePath = GetCachePath();
		auto path = QString("%1/%2").arg(cachePath, "temp");
		QDir dir(path);
		if (dir.exists()) {
			dir.removeRecursively();
		}
		dir.mkpath(path);
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

	static QString GetFolder(const QString& path) {
		QDir dir(path);
		auto  list = dir.entryList(QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		return list.join(",");
	}

	static QFileInfoList GetFilesRecursive(const QString& path, const QString& extension = ".*")
	{
		QDir dir(path);
		QStringList filter;
		filter << extension;
		dir.setNameFilters(filter);
		QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
		QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (int i = 0; i != folder_list.size(); i++)
		{
			QString name = folder_list.at(i).absoluteFilePath();
			QFileInfoList child_file_list = GetFilesRecursive(name, extension);
			file_list.append(child_file_list);
		}

		return file_list;
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

	static Napi::Value getFolder(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto list = DirHelper::GetFolder(info[0].As<Napi::String>().ToString().Utf8Value().c_str());
		return Napi::String::New(env, list.toStdString().c_str());
	}
	
};

