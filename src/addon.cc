#include <napi.h>

#include "Package.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include "utils/DirHelper.h"
#include "Downloader.h"
#include "FlaExtension.h"

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
  exports.Set(Napi::String::New(env, "setTPPath"), Napi::Function::New(env, AppConfig::setTPPath));
  exports.Set(Napi::String::New(env, "setSkinId"), Napi::Function::New(env, AppConfig::setSkinId));
  exports.Set(Napi::String::New(env, "setTemplateId"), Napi::Function::New(env, AppConfig::setTemplateId));
  exports.Set(Napi::String::New(env, "getSkinFullPath"), Napi::Function::New(env, DirHelper::getSkinFullPath));
  exports.Set(Napi::String::New(env, "getAssetsFullPath"), Napi::Function::New(env, DirHelper::getAssetsFullPath));
  exports.Set(Napi::String::New(env, "getOutputFullPath"), Napi::Function::New(env, DirHelper::getOutputFullPath));
  exports.Set(Napi::String::New(env, "getFolder"), Napi::Function::New(env, DirHelper::getFolder));
  exports.Set(Napi::String::New(env, "gotoHistory"), Napi::Function::New(env, DirHelper::gotoHistory));
  exports.Set(Napi::String::New(env, "gotoMangoCache"), Napi::Function::New(env, DirHelper::gotoMangoCache));
  exports.Set(Napi::String::New(env, "package"), Napi::Function::New(env, PackageHelper::package));
  exports.Set(Napi::String::New(env, "upload"), Napi::Function::New(env, PackageHelper::upload));
  exports.Set(Napi::String::New(env, "getNeedTinyFiles"), Napi::Function::New(env, PackageHelper::getNeedTinyFiles));
  exports.Set(Napi::String::New(env, "download"), Napi::Function::New(env, Downloader::download));
  exports.Set(Napi::String::New(env, "parse"), Napi::Function::New(env, Downloader::parse));
  exports.Set(Napi::String::New(env, "generateCSD"), Napi::Function::New(env, FlaExtension::generateCSD));
  return exports;
}

NODE_API_MODULE(addon, Init)
