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
	std::vector<nlohmann::json> resources;
public:
	void Init();
	void Package();
	void HandleImages();
	void HandleCSD();
	void HandleAssets();
	void Upload();
	QString Compress();
	nlohmann::json GetItemConfig(const QString& path);
	QString Tiny(const QString& path);
	void TexturePackage();

	static Napi::Value package(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		PackageHelper::GetInstance().Package();
		return env.Null();
	}

	static Napi::Value tiny(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		PackageHelper::GetInstance().Init();
		PackageHelper::GetInstance().HandleImages();
		return env.Null();
	}

	static Napi::Value upload(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		PackageHelper::GetInstance().Upload();
		return env.Null();
	}

};