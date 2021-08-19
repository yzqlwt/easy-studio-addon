//
// Created by yzqlwt on 2020/11/6.
//

#ifndef EASYSTUDIO_PLISTPARSER_H
#define EASYSTUDIO_PLISTPARSER_H
#include "Tools.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <regex>
#include <boost/lexical_cast.hpp>
#include <opencv2/opencv.hpp>

class Frame{
public:
    std::string key;
    int frame[4];
    int offset[2];
    bool rotated;
    int sourceColorRect[4];
    int sourceSize[2];

    Frame(std::map<std::string, std::string> map){
        this->key = map["key"];
        auto frames = Frame::GetNumbers(map["frame"]);
        auto offsets = Frame::GetNumbers(map["offset"]);
        auto sourceColorRects = Frame::GetNumbers(map["sourceColorRect"]);
        auto sourceSizes = Frame::GetNumbers(map["sourceSize"]);

        std::copy(frames.begin(), frames.end(), this->frame);
        std::copy(offsets.begin(), offsets.end(), this->offset);
        std::copy(sourceColorRects.begin(), sourceColorRects.end(), this->sourceColorRect);
        std::copy(sourceSizes.begin(), sourceSizes.end(), this->sourceSize);
        this->rotated = map["rotated"] == "true" ? true : false;
    }
public:
    cv::Mat convertToMat(cv::Mat& image){
        if (this->rotated){
            cv::Rect rect(this->frame[0], this->frame[1],this->frame[3],this->frame[2]);
            auto newImage = image(rect);
            cv::Mat temp,dst;
            cv::transpose(newImage, temp);
            cv::flip(temp,dst,0);
            return dst;
        }else{
            cv::Rect rect(this->frame[0], this->frame[1],this->frame[2],this->frame[3]);
            auto newImage = image(rect);
            return newImage;
        }
    }

    static std::vector<int> GetNumbers(std::string str){
        std::regex port("[0-9]+");
        std::sregex_token_iterator iter(str.begin(), str.end(), port);
        std::sregex_token_iterator end;
        std::vector<int> list;
        for(; iter != end; ++iter)
        {
            list.push_back(boost::lexical_cast<int>(iter->str()));
        }
        return list;
    }
};

class PlistParser {
private:
    std::string _imagePath;
    std::string _plistPath;
    std::vector<Frame> _frames;
public:
    void load(std::string imagePath, std::string plistPath);
    void save(const std::string& dirPath);
};


#endif //EASYSTUDIO_PLISTPARSER_H
