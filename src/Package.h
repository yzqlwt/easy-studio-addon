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
	Napi::Env* _env;
	Napi::Function* _func;
	std::vector<nlohmann::json> resources;
public:
	void Package();
	void HandleImages();
	void HandleCSD();
	void HandleAssets();
	void Compress();
	nlohmann::json GetItemConfig(const QString& path);
	QString Tiny(const QString& path);
	void TexturePackage();
	void Emit(const QString& type, const QString& content);
	void initEnv(Napi::Env* env, Napi::Function* func) {
		this->_env = env;
		this->_func = func;
	}
	static Napi::Value package(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		Napi::Function func = info[0].As<Napi::Function>();
		PackageHelper::GetInstance().initEnv(&env, &func);
		PackageHelper::GetInstance().Package();
		return env.Null();
	}

};