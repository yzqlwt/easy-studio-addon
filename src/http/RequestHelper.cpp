//
// Created by yzqlwt on 2020/11/28.
//
#include "RequestHelper.h"
#include "httplib.h"
#include "../utils/Tools.h"
#include "../AppConfig.h"
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>

static QString BaseUrl = "https://gate2.betamath.com";
static QString ResUrl = "https://gstatic-cn.oss-cn-hangzhou.aliyuncs.com";

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}


QString RequestHelper::Get(const QString& uri){
    httplib::Client cli(BaseUrl.toStdString().c_str());
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().toStdString().c_str());
    auto res = cli.Get(uri.toStdString().c_str());
    return res->body.c_str();
}


/**
 * @brief             获取attachments
 * @param itemId      itemType是ActivityTemplate,为模板ID, itemType是Skin，为SkinId
 * @param itemType    ActivityTemplate模板所有skin共用, Skin为skin独享
 * @return            返回json字符串(list)
 */
QString RequestHelper::GetAttachments(const QString &itemId, const QString &itemType) {
    auto url = QString("/admin-course/item/attachment?itemType=%1&itemId=%2").arg(itemType, itemId);
    return RequestHelper::Get(url);
}


/**
 * @brief             获取attachments
 * @param itemId      itemType是ActivityTemplate,为模板ID, itemType是Skin，为SkinId
 * @param itemType    ActivityTemplate模板所有skin共用, Skin为skin独享
 * @return            返回json字符串(list)
 */
QString RequestHelper::GetResConfigId(const QString &itemId, const QString &itemType) {
    auto res = RequestHelper::GetAttachments(itemId, itemType);
    if (nlohmann::json::accept(res.toStdString())){
        auto json = nlohmann::json::parse(res.toStdString());
        auto arr = json.get<std::vector<std::map<std::string, nlohmann::json>>>();
        for (int i = 0; i < arr.size(); ++i) {
            auto name = QString(arr[i]["name"].get<std::string>().c_str());
            if (name.compare("ResConfig")==0){
                return QString::number(arr[i]["id"].get<int>());
            }
        }
    }
    return "";
}


/**
 * @brief      上传文件
 * @param path 文件路径
 */

void RequestHelper::UploadFile(const QString &path, const QString &itemId) {
    httplib::Client cli(BaseUrl.toStdString().c_str());
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().toStdString().c_str());
    std::ifstream ifs(path.toStdString(), std::ios::in | std::ios::binary);
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    std::string content(ss.str());
    httplib::MultipartFormDataItems items = {
            { "file", content, "res2-0.zip", "application/zip" },
    };
    auto res = cli.Post("/admin-course/asset/uploadSingle", items);
    std::cout << "文件上传:\t" << res->body << std::endl;
    if (nlohmann::json::accept(res->body)){
        auto json = nlohmann::json::parse(res->body);
        auto id = std::to_string(json["id"].get<int>());
        auto attachment_id = RequestHelper::GetResConfigId(itemId, "Skin");
        if (attachment_id == ""){
            RequestHelper::AddResConfig(itemId, id.c_str());
        }else {
            RequestHelper::ModifyResConfig(attachment_id, itemId, id.c_str());
        }
    }
}

/**
 * @brief 创建ResConfig
 * @param itemId: skin_id
 * @param attachmentId: 上传文件返回的id
 */

void RequestHelper::AddResConfig(const QString& itemId, const QString& attachmentId) {
    httplib::Client cli(BaseUrl.toStdString().c_str());
    char attachment[100];
    sprintf(attachment, "[%s]", attachmentId.toStdString().c_str());
    httplib::Params params{
            {"itemId",   itemId.toStdString()},
            {"attachmentId", attachmentId.toStdString()},
            {"itemType",    "Skin"},
            {"name",    "ResConfig"},
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().toStdString().c_str());
    auto res = cli.Post("/admin-course/item/attachment", params);
    std::cout << "add config:\t" << res->body << std::endl;
}

void RequestHelper::ModifyResConfig(const QString& id, const QString& itemId, const QString& attachmentId) {
    httplib::Client cli(BaseUrl.toStdString().c_str());
    char attachment[100];
    sprintf(attachment, "[%s]", attachmentId.toStdString().c_str());
    httplib::Params params{
            {"id",   id.toStdString()},
            {"itemId",   itemId.toStdString()},
            {"attachmentId", attachmentId.toStdString()},
            {"itemType",    "Skin"},
            {"name",    "ResConfig"},
    };
    cli.set_default_headers({
                                    {"Accept",     "*/*"},
                                    {"User-Agent", "Chrome/86.0.4240.198 Safari/537.36"}
                            });
    cli.set_bearer_token_auth(Tools::GetToken().toStdString().c_str());
	auto url = QString("/admin-course/item/attachment/%1").arg(id);
	auto res = cli.Put(url.toStdString().c_str(), params);
	std::cout << "modify config:\t" << res->body << std::endl;
}

QString RequestHelper::Download(const QString &uri, const QString&path) {
    FILE *fp = fopen(path.toStdString().c_str(), "wb");

    CURL *curl = curl_easy_init();
    // curl返回值
    CURLcode res;
    if (curl)
    {
        //设置curl的请求头
        struct curl_slist* header_list = NULL;
        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

        //不接收响应头数据0代表不接收 1代表接收
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);

        //设置请求的URL地址
        curl_easy_setopt(curl, CURLOPT_URL, uri.toStdString().c_str());

        //设置ssl验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

        //设置数据接收函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        //设置超时时间
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);

        // 开启请求
        res = curl_easy_perform(curl);
    }
    // 释放curl
    curl_easy_cleanup(curl);
    //释放文件资源
    fclose(fp);
    return path;
}

QString RequestHelper::DownloadZip(const QString &uri, const QString& zipPath) {
    auto url = ResUrl + uri;
    auto savePath = zipPath + "/" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()).c_str() + ".zip";
    return Download(url, savePath);
}

QString RequestHelper::GetResZipUri() {
    auto itemId = AppConfig::GetInstance().GetSkinId();
    auto attachments = RequestHelper::GetAttachments(itemId, "Skin");
    auto json = nlohmann::json::parse(attachments.toStdString());
    if (json.is_array()){
        for (auto& element : json) {
            QString name = element["name"].get<std::string>().c_str();
            if (name.compare("ResConfig") == 0){
                return element["attachments"]["uri"].get<std::string>().c_str();
            }
        }
    }
    return "";
}







