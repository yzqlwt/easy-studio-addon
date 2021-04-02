#pragma once

#include <napi.h>
#include <map>
#include <nlohmann/json.hpp>
#include <QtCore/QString>
#include <iostream>



class PackageHelper
{
public:
	~PackageHelper() {
		std::cout << "PackageHelper destructor called!" << std::endl;
	}
	PackageHelper(const PackageHelper&) = delete;
	PackageHelper& operator=(const PackageHelper&) = delete;
	static PackageHelper& GetInstance() {
		static PackageHelper instance;
		return instance;
	}
private:
	PackageHelper() {
		std::cout << "PackageHelper constructor called!" << std::endl;
	}
	nlohmann::json resources;
public:
	void Init();
	QString Package();
	void HandleImages();
	void HandleCSD();
	void HandleAssets();
	void Upload();
	QString Compress();
	QString GetNeedTinyFiles();
	std::pair<std::string, nlohmann::json> GetItemConfig(const QString& path);
	QString Tiny(const QString& path);
	QString TexturePackage();

	static Napi::Value package(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto result = PackageHelper::GetInstance().Package();
		return Napi::String::New(env, result.toStdString().c_str());
	}

	static Napi::Value upload(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		PackageHelper::GetInstance().Upload();
		return env.Null();
	}

	static Napi::Value getNeedTinyFiles(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto path = PackageHelper::GetInstance().GetNeedTinyFiles();
		return Napi::String::New(env, path.toStdString().c_str());
	}

};