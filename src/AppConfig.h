#pragma once

#include <napi.h>
#include <QtCore/qstring.h>
#include <iostream>

class AppConfig
{
public:
	~AppConfig() {
		std::cout << "AppConfig destructor called!" << std::endl;
	}
	AppConfig(const AppConfig&) = delete;
	AppConfig& operator=(const AppConfig&) = delete;
	static AppConfig& GetInstance() {
		static AppConfig instance;
		return instance;
	}
public:
	void SetSkinPath(const QString& path) {
		_skinPath = path;
	}

	void SetCCSPath(const QString& path) {
		_ccsPath = path;
	}

	void SetTPPath(const QString& path) {
		_tpPath = path;
	}

	QString GetSkinPath() {
		return _skinPath;
	}

	QString GetCCSPath() {
		return _ccsPath;
	}

	QString GetTPPath() {
		return _tpPath;
	}

	static Napi::Value setSkinPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto skinPath = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::GetInstance().SetSkinPath(skinPath);
		return env.Null();
	}

	static Napi::Value setCCSPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto skinPath = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::GetInstance().SetCCSPath(skinPath);
		return env.Null();
	}

	static Napi::Value setTPPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto skinPath = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::GetInstance().SetTPPath(skinPath);
		return env.Null();
	}
private:
	QString _ccsPath;
	QString _skinPath;
	QString _tpPath;
	AppConfig() {
		std::cout << "AppConfig constructor called!" << std::endl;
	}
};