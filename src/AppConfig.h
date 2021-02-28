#pragma once

#include <QtCore/qstring.h>

class AppConfig {
public:
	static QString CCS_PATH;
	static QString SKIN_PATH;
	static bool isTiny;

	static void SetSkinPath(const QString& path) {
		SKIN_PATH = path;
	}

	static void SetCCSPath(const QString& path) {
		CCS_PATH = path;
	}
	static Napi::Value setSkinPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto skinPath = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::SetSkinPath(skinPath);
		return env.Null();
	}

	static Napi::Value setCCSPath(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto skinPath = info[0].As<Napi::String>().Utf8Value().c_str();
		AppConfig::SetCCSPath(skinPath);
		return env.Null();
	}

};

QString AppConfig::CCS_PATH = "";
QString AppConfig::SKIN_PATH = "";
bool    AppConfig::isTiny = false;