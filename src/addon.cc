#include <napi.h>

#include "AppConfig.h"
#include "Package.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include "utils/PlistParser.h"
#include "utils/DirHelper.h"



static Napi::Value getClipData(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	auto path = Tools::GetClipboardFiles();
	return Napi::String::New(env, path.toStdString().c_str());
}

static Napi::Value gotoFolder(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	auto path = info[0].As<Napi::String>().Utf8Value().c_str();
	Tools::GotoFolder(path);
	return env.Null();
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getClipboardFiles"), Napi::Function::New(env, getClipData));
  exports.Set(Napi::String::New(env, "gotoFolder"), Napi::Function::New(env, gotoFolder));
  exports.Set(Napi::String::New(env, "setCCSPath"), Napi::Function::New(env, AppConfig::setCCSPath));
  exports.Set(Napi::String::New(env, "setSkinPath"), Napi::Function::New(env, AppConfig::setSkinPath));
  exports.Set(Napi::String::New(env, "getSkinFullPath"), Napi::Function::New(env, DirHelper::getSkinFullPath));
  exports.Set(Napi::String::New(env, "getAssetsFullPath"), Napi::Function::New(env, DirHelper::getAssetsFullPath));
  exports.Set(Napi::String::New(env, "getFolder"), Napi::Function::New(env, DirHelper::getFolder));
  exports.Set(Napi::String::New(env, "package"), Napi::Function::New(env, PackageHelper::package));
  return exports;
}

NODE_API_MODULE(addon, Init)
