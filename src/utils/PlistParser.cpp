#include "PlistParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <tinyxml2.h>
#include <regex>
#include <nlohmann/json.hpp>

namespace pt = boost::property_tree;


void PlistParser::load(std::string imagePath, std::string plistPath) {
    pt::ptree tree;
    std::ifstream wif(plistPath, std::ios::in);
	pt::read_xml(wif, tree);
	auto dict = tree.get_child("plist.dict.dict");
	std::vector<Frame> frames;
	for (auto pos = dict.begin(); pos != dict.end();)
	{
		auto key = pos->second.get_value<std::string>();
		pos++;
		std::vector<std::string> list;
		std::map<std::string, std::string> map;
		auto child = pos->second;
		for (auto index = child.begin(); index != child.end();) {
			auto childKey = index->second.get_value<std::string>();
			index++;
			if (childKey == "rotated") {
				map[childKey] = index->first;
			}
			else {
				map[childKey] = index->second.get_value<std::string>();
			}
			index++;
		}
		map["key"] = key;
		this->_frames.push_back(Frame(map));
		pos++;
	}
	this->_imagePath = std::move(imagePath);
	this->_plistPath = std::move(plistPath);

}

void PlistParser::save(const std::string& dirPath) {
    cv::Mat origin = cv::imread(this->_imagePath, cv::IMREAD_UNCHANGED);
    for (auto frame : this->_frames) {
        auto region = frame.convertToMat(origin);
        cv::imwrite(dirPath + "/" + frame.key, region);
    }
}
