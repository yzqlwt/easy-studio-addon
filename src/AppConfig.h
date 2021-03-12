#pragma once

#include <napi.h>
#include <QtCore/qstring.h>
#include <QtCore/QDebug>
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

	void SetTemplateId(const QString& id) {
		_templateId = id;
	}

	void SetSkinId(const QString& id) {
		_skinId = id;
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

	QString GetTemplateId() {
		return _templateId;
	}

	QString GetSkinId() {
		return _skinId;
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

	static Napi::Value setTemplateId(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto id = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::GetInstance().SetTemplateId(id);
		return env.Null();
	}

	static Napi::Value setSkinId(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto id = info[0].As<Napi::Number>().Int32Value();
		AppConfig::GetInstance().SetSkinId(QString::number(id));
		return env.Null();
	}

private:
	QString _ccsPath;
	QString _skinPath;
	QString _tpPath;
	QString _templateId;
	QString _skinId;
	AppConfig() {
		std::cout << "AppConfig constructor called!" << std::endl;
	}
};