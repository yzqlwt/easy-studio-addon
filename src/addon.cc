#include <napi.h>

#include "AppConfig.h"
#include "Package.h"
#include "utils/Tools.h"
#include "http/httplib.h"
#include "utils/PlistParser.h"
#include "utils/DirHelper.h"

Napi::Value Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  double arg0 = info[0].As<Napi::Number>().DoubleValue();
  double arg1 = info[1].As<Napi::Number>().DoubleValue();
  Napi::Number num = Napi::Number::New(env, arg0 + arg1);

  return num;
}

Napi::Value Read(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 2) {
		Napi::TypeError::New(env, "Wrong number of arguments")
			.ThrowAsJavaScriptException();
		return env.Null();
	}

	if (!info[0].IsString() || !info[1].IsFunction()) {
		Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
		return env.Null();
	}

	auto arg0 = info[0].As<Napi::String>().ToString().Utf8Value().c_str();
	auto content = Tools::ReadFile(arg0);
	Napi::Function cb = info[1].As<Napi::Function>();
	cb.Call(env.Global(), { Napi::String::New(env, content.toStdString().c_str()) });
	return Napi::String::New(env, content.toStdString().c_str());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, Add));
  exports.Set(Napi::String::New(env, "readFile"), Napi::Function::New(env, Read));
  exports.Set(Napi::String::New(env, "setCCSPath"), Napi::Function::New(env, AppConfig::setCCSPath));
  exports.Set(Napi::String::New(env, "setSkinPath"), Napi::Function::New(env, AppConfig::setSkinPath));
  exports.Set(Napi::String::New(env, "getSkinFullPath"), Napi::Function::New(env, DirHelper::getSkinFullPath));
  exports.Set(Napi::String::New(env, "handleImages"), Napi::Function::New(env, Package::handleImages));
  return exports;
}

NODE_API_MODULE(addon, Init)
