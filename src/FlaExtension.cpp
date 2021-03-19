//
// Created by yzqlwt on 2021/1/23.
//

#include "FlaExtension.h"
#include "utils/Tools.h"
#include "utils/DirHelper.h"
#include <QtCore/QDir>
#include <QtCore/QByteArray>
#include <QtCore/QURL>


void FlaExtension::ReadTemplateXml(){
	QString content = QString()+"<GameFile><PropertyGroup Name=\"Layer\" Type=\"Layer\" ID=\"qtos9-7px2blv15j0y84gkdzherfna6iwmc3\" "+
		"Version=\"3.10.0.0\" /><Content ctype=\"GameProjectContent\"><Content><Animation Duration=\"0\" "+
		"Speed=\"1.000000\"></Animation><ObjectData Name=\"Layer\" ctype=\"GameLayerObjectData\"><Size X=\"1440.000000\" "+
		" Y=\"680.000000\" /><Children><AbstractNodeData Name=\"NodeTemplate\" ActionTag=\"522191067\" Tag=\"36\" "+
		"IconVisible=\"True\" RightMargin=\"1440.0000\" TopMargin=\"680.0000\" ctype=\"SingleNodeObjectData\"><Size "+
		"X=\"0.0000\" Y=\"0.0000\" /><Position X=\"585\" Y=\"455\" /></AbstractNodeData><AbstractNodeData "+
		"Name=\"ImageViewTemplate\" ActionTag=\"-655436352\" Tag=\"20\" LeftMargin=\"446.658691406\" RightMargin=\"-724.658691406\""+
		" TopMargin=\"-592.68182373\" BottomMargin=\"318.681793213\" LeftEage=\"91\" RightEage=\"91\" TopEage=\"90\" "+
		"BottomEage=\"90\" Scale9OriginX=\"91\" Scale9OriginY=\"90\" Scale9Width=\"96\" Scale9Height=\"94\" "+
		"ctype=\"ImageViewObjectData\"><Size Y=\"274\" X=\"278\" /><AnchorPoint ScaleX=\"0.5\" ScaleY=\"0.5\" /><Position "+
		"X=\"585\" Y=\"455\" /><Scale ScaleX=\"1\" ScaleY=\"1\" /><CColor A=\"255\" B=\"255\" G=\"255\" R=\"255\" /><FileData "+
		"Type=\"Normal\" Path=\"res/ui/games/game146/skin01/panel.png\" Plist=\"\" /></AbstractNodeData><AbstractNodeData "+
		"Name=\"ButtonTemplate\" ActionTag=\"30262198\" Tag=\"47\" IconVisible=\"False\" LeftMargin=\"912.9999\" "+
		"RightMargin=\"391.0001\" TopMargin=\"540.0000\" BottomMargin=\"14.0000\" TouchEnable=\"True\" FontSize=\"14\" "+
		"Scale9Enable=\"True\" LeftEage=\"15\" RightEage=\"15\" TopEage=\"11\" BottomEage=\"11\" Scale9OriginX=\"15\" "+
		"Scale9OriginY=\"11\" Scale9Width=\"106\" Scale9Height=\"104\" ShadowOffsetX=\"2.0000\" ShadowOffsetY=\"-2.0000\" "+
		"ctype=\"ButtonObjectData\"><Size X=\"136.0000\" Y=\"126.0000\" /><AnchorPoint ScaleX=\"0.5000\" ScaleY=\"0.5000\" "+
		"/><Position X=\"980.9999\" Y=\"77.0000\" /><Scale ScaleX=\"1.0000\" ScaleY=\"1.0000\" /><CColor A=\"255\" R=\"255\" "+
		"G=\"255\" B=\"255\" /><PrePosition X=\"0.6812\" Y=\"0.1132\" /><PreSize X=\"0.0944\" Y=\"0.1853\" /><TextColor A=\"255\" "+
		"R=\"65\" G=\"65\" B=\"70\" /><DisabledFileData Type=\"Normal\" Path=\"\" Plist=\"\" /><PressedFileData Type=\"Normal\" "+
		"Path=\"\" Plist=\"\" /><NormalFileData Type=\"Normal\" Path=\"\" Plist=\"\" /><OutlineColor A=\"255\" R=\"255\" G=\"0\" "+
		"B=\"0\" /><ShadowColor A=\"255\" R=\"110\" G=\"110\" B=\"110\" /></AbstractNodeData></Children></ObjectData></Content>"+
		"</Content></GameFile>";
    document.Parse(content.toStdString().c_str());
    Children = document.FirstChildElement("GameFile")->FirstChildElement("Content")->FirstChildElement("Content")->FirstChildElement("ObjectData")->FirstChildElement("Children");
    ImageView = queryNodeByName(Children, "ImageViewTemplate");
    Node = queryNodeByName(Children, "NodeTemplate");
    Button = queryNodeByName(Children, "ButtonTemplate");
}

void FlaExtension::ReadConfig() {
    auto path = QString("%1/fla.txt").arg(DirHelper::GetCachePath());
    QFileInfo info(path);
    if (info.isFile()) {
        auto content = Tools::ReadFile(path);
        content.replace("file:///", "");
        content.replace("|", ":");
		QString jsonPath = QByteArray::fromPercentEncoding(content.toStdString().c_str());
		auto jsonData = Tools::ReadFile(jsonPath);
		if (nlohmann::json::accept(jsonData.toStdString())){
			nlohmann::json json = nlohmann::json::parse(jsonData.toStdString());
			this->path = json["path"].get<std::string>().c_str();
			symbols = json["symbols"].get<std::vector<std::map<std::string, std::string>>>();
			this->Write(QFileInfo(jsonPath).absolutePath());
			QFile::remove(jsonPath);
        }
		QFile::remove(path);
    }
}

void FlaExtension::Write(QString dir_path){
	QFileInfo info(this->path);
	auto csd_path = dir_path + "/" + info.fileName() + ".csd";
	for (auto item : symbols) {
		auto name = item["name"];
		auto image = item["image"];
		auto type = item["type"];
		auto x = item["x"];
		auto y = item["y"];
		auto file_path = this->path + "/" + image.c_str();
		if (type.compare("ImageView") == 0) {
			tinyxml2::XMLElement* imageview = (tinyxml2::XMLElement*)deepCopy(this->ImageView, &document);
			imageview->SetAttribute("Name", name.c_str());
			tinyxml2::XMLElement* FileData = imageview->FirstChildElement("FileData");
			FileData->SetAttribute("Path", file_path.toStdString().c_str());
			tinyxml2::XMLElement* Position = imageview->FirstChildElement("Position");
			Position->SetAttribute("X", x.c_str());
			Position->SetAttribute("Y", y.c_str());
			tinyxml2::XMLElement* Size = imageview->FirstChildElement("Size");
			cv::Mat mat = cv::imread((dir_path + "/" + image.c_str()).toStdString(), cv::IMREAD_UNCHANGED);
			Size->SetAttribute("X", mat.cols);
			Size->SetAttribute("Y", mat.rows);
			Children->LinkEndChild(imageview);
		}
		else if (type.compare("Node") == 0) {
			tinyxml2::XMLElement* node = (tinyxml2::XMLElement*)deepCopy(this->Node, &document);
			node->SetAttribute("Name", name.c_str());
			tinyxml2::XMLElement* Position = node->FirstChildElement("Position");
			Position->SetAttribute("X", x.c_str());
			Position->SetAttribute("Y", y.c_str());
			Children->LinkEndChild(node);
		}
		else if (type.compare("Button") == 0) {
			tinyxml2::XMLElement* button = (tinyxml2::XMLElement*)deepCopy(this->Button, &document);
			button->SetAttribute("Name", name.c_str());
			tinyxml2::XMLElement* NormalFileData = button->FirstChildElement("NormalFileData");
			NormalFileData->SetAttribute("Path", file_path.toStdString().c_str());
			tinyxml2::XMLElement* Position = button->FirstChildElement("Position");
			Position->SetAttribute("X", x.c_str());
			Position->SetAttribute("Y", y.c_str());
			cv::Mat mat = cv::imread((dir_path + "/" + image.c_str()).toStdString(), cv::IMREAD_UNCHANGED);
			tinyxml2::XMLElement* Size = button->FirstChildElement("Size");
			Size->SetAttribute("X", mat.cols);
			Size->SetAttribute("Y", mat.rows);
			Children->LinkEndChild(button);
		}
	}
	Children->DeleteChild(this->ImageView);
	Children->DeleteChild(this->Node);
	Children->DeleteChild(this->Button);
	document.SaveFile(csd_path.toLocal8Bit().toStdString().c_str());
	qDebug() << csd_path.toLocal8Bit().toStdString().c_str();
}