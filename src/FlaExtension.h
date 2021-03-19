//
// Created by yzqlwt on 2021/1/10.
//

#ifndef EASYSTUDIO_FLAEXTENSION2_H
#define EASYSTUDIO_FLAEXTENSION2_H

#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <tinyxml2.h>
#include <napi.h>
#include <QtCore/QString>


class FlaExtension {

public:
    FlaExtension(){
        ReadTemplateXml();
        ReadConfig();
    }
    void ReadTemplateXml();
    void ReadConfig();
    void Write(QString dir_path);

	static Napi::Value generateCSD(const Napi::CallbackInfo& info) {
		Napi::Env env = info.Env();
        FlaExtension();
		return env.Null();
	}
private:
    tinyxml2::XMLDocument document;
    tinyxml2::XMLDocument component;
    tinyxml2::XMLElement* Children;
    tinyxml2::XMLElement* ImageView;
    tinyxml2::XMLElement* Button;
    tinyxml2::XMLElement* Node;
    QString path;
    std::vector<std::map<std::string, std::string>> symbols;

    tinyxml2::XMLElement* queryNodeByName(tinyxml2::XMLElement* children, QString name)
    {

        tinyxml2::XMLElement* node=children->FirstChildElement("AbstractNodeData");
        while(node!=NULL)
        {
            if(name.compare(node->Attribute("Name"))==0)
                break;
            node=node->NextSiblingElement();//下一个兄弟节点
        }
        return node;
    }
    tinyxml2::XMLNode *deepCopy( tinyxml2::XMLNode *src, tinyxml2::XMLDocument *destDoc )
    {
        tinyxml2::XMLNode *current = src->ShallowClone( destDoc );
        for( tinyxml2::XMLNode *child=src->FirstChild(); child; child=child->NextSibling() )
        {
            current->InsertEndChild( deepCopy( child, destDoc ) );
        }

        return current;
    }

};

#endif //EASYSTUDIO_FLAEXTENSION2_H
