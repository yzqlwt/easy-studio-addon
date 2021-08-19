#pragma once

#include <napi.h>
#include <map>
#include <nlohmann/json.hpp>
#include <QtCore/QString>
#include <iostream>


class Downloader
{
public:
	~Downloader() {
		std::cout << "Downloader destructor called!" << std::endl;
	}
	Downloader(const Downloader&) = delete;
	Downloader& operator=(const Downloader&) = delete;
	static Downloader& GetInstance() {
		static Downloader instance;
		return instance;
	}
private:
	QString _zipPath;
	Downloader() {
		std::cout << "Downloader constructor called!" << std::endl;
	}
	void OldVersionParse();

public:
	QString Download();
	void Parse();
	static Napi::Value download(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto path = Downloader::GetInstance().Download();
		return Napi::String::New(env, path.toStdString().c_str());
	}

	static Napi::Value parse(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		Downloader::GetInstance().Parse();
		return env.Null();
	}

};