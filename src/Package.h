#pragma once
#include "utils/DirHelper.h"

class Package {
public:
	static QString HandleImages() {
		auto files = DirHelper::GetFilesRecursive(DirHelper::GetSkinFullPath(), ".png");
		return files.join("\n");
	}
	static Napi::Value handleImages(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
		auto files = Package::HandleImages();
		return Napi::String::New(env, files.toStdString().c_str());
	}
};