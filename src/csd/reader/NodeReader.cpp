//
// Created by yzqlwt on 2020/12/1.
//

#include "NodeReader.h"
#include "../flatbuffers/flatbuffers.h"
#include "../CSParseBinary_generated.h"
#include "ccTypes.h"
using namespace flatbuffers;

const char* Layout_PositionPercentXEnabled = "PositionPercentXEnabled";
const char* Layout_PositionPercentYEnabled = "PositionPercentYEnabled";
const char* Layout_PercentWidthEnable = "PercentWidthEnabled";
const char* Layout_PercentHeightEnable = "PercentHeightEnabled";
const char* Layout_StretchWidthEnable = "StretchWidthEnable";
const char* Layout_StretchHeightEnable = "StretchHeightEnable";
const char* Layout_HorizontalEdge = "HorizontalEdge";
const char* Layout_VerticalEdge = "VerticalEdge";
const char* Layout_LeftMargin = "LeftMargin";
const char* Layout_RightMargin = "RightMargin";
const char* Layout_TopMargin = "TopMargin";
const char* Layout_BottomMargin = "BottomMargin";
const char* Layout_BothEdge = "BothEdge";
const char* Layout_LeftEdge = "LeftEdge";
const char* Layout_RightEdge = "RightEdge";
const char* Layout_TopEdge = "TopEdge";
const char* Layout_BottomEdge = "BottomEdge";

static NodeReader* _instanceNodeReader = nullptr;




NodeReader* NodeReader::getInstance()
{
    if (!_instanceNodeReader)
    {
        _instanceNodeReader = new (std::nothrow) NodeReader();
    }

    return _instanceNodeReader;
}

Offset<Table> NodeReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                       flatbuffers::FlatBufferBuilder *builder)
{
    std::string name = "";
    long actionTag = 0;
    Vec2 rotationSkew;
    int zOrder = 0;
    bool visible = true;
    unsigned char alpha = 255;
    int tag = 0;
    Vec2 position;
    Vec2 scale(1.0f, 1.0f);
    Vec2 anchorPoint;
    Color4B color(255, 255, 255, 255);

    Vec2 size;
    bool flipX = false;
    bool flipY = false;
    bool ignoreSize = false;
    bool touchEnabled = false;
    std::string frameEvent = "";
    std::string customProperty = "";

    bool positionXPercentEnabled = false;
    bool positionYPercentEnabled = false;
    float positionXPercent = 0;
    float positionYPercent = 0;
    bool sizeXPercentEnable = false;
    bool sizeYPercentEnable = false;
    float sizeXPercent = 0;
    float sizeYPercent = 0;
    bool stretchHorizontalEnabled = false;
    bool stretchVerticalEnabled = false;
    std::string horizontalEdge;
    std::string verticalEdge;
    float leftMargin = 0;
    float rightMargin = 0;
    float topMargin = 0;
    float bottomMargin = 0;

    // attributes
    const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
    while (attribute)
    {
        std::string attriname = attribute->Name();
        std::string value = attribute->Value();

        if (attriname == "Name")
        {
            name = value;
        }
        else if (attriname == "ActionTag")
        {
            actionTag = atol(value.c_str());
        }
        else if (attriname == "RotationSkewX")
        {
            rotationSkew.x = atof(value.c_str());
        }
        else if (attriname == "RotationSkewY")
        {
            rotationSkew.y = atof(value.c_str());
        }
        else if (attriname == "Rotation")
        {
            //            rotation = atoi(value.c_str());
        }
        else if (attriname == "FlipX")
        {
            flipX = (value == "True") ? true : false;
        }
        else if (attriname == "FlipY")
        {
            flipY = (value == "True") ? true : false;
        }
        else if (attriname == "ZOrder")
        {
            zOrder = atoi(value.c_str());
        }
        else if (attriname == "Visible")
        {
            //            visible = (value == "True") ? true : false;
        }
        else if (attriname == "VisibleForFrame")
        {
            visible = (value == "True") ? true : false;
        }
        else if (attriname == "Alpha")
        {
            alpha = atoi(value.c_str());
        }
        else if (attriname == "Tag")
        {
            tag = atoi(value.c_str());
        }
        else if (attriname == "TouchEnable")
        {
            touchEnabled = (value == "True") ? true : false;
        }
        else if (attriname == "UserData")
        {
            customProperty = value;
        }
        else if (attriname == "FrameEvent")
        {
            frameEvent = value;
        }
        else if (attriname == Layout_PositionPercentXEnabled)
        {
            positionXPercentEnabled = value == "True";
        }
        else if (attriname == Layout_PositionPercentYEnabled)
        {
            positionYPercentEnabled = value == "True";
        }
        else if (attriname == Layout_PercentWidthEnable)
        {
            sizeXPercentEnable = value == "True";
        }
        else if (attriname == Layout_PercentHeightEnable)
        {
            sizeYPercentEnable = value == "True";
        }
        else if (attriname == Layout_StretchWidthEnable)
        {
            stretchHorizontalEnabled = value == "True";
        }
        else if (attriname == Layout_StretchHeightEnable)
        {
            stretchVerticalEnabled = value == "True";
        }
        else if (attriname == Layout_HorizontalEdge)
        {
            horizontalEdge = value;
        }
        else if (attriname == Layout_VerticalEdge)
        {
            verticalEdge = value;
        }
        else if (attriname == Layout_LeftMargin)
        {
            leftMargin = atof(value.c_str());
        }
        else if (attriname == Layout_RightMargin)
        {
            rightMargin = atof(value.c_str());
        }
        else if (attriname == Layout_TopMargin)
        {
            topMargin = atof(value.c_str());
        }
        else if (attriname == Layout_BottomMargin)
        {
            bottomMargin = atof(value.c_str());
        }

        attribute = attribute->Next();
    }

    const tinyxml2::XMLElement* child = objectData->FirstChildElement();
    while (child)
    {
        std::string attriname = child->Name();
        if (attriname == "Position")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "X")
                {
                    position.x = atof(value.c_str());
                }
                else if (attriname == "Y")
                {
                    position.y = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "Scale")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "ScaleX")
                {
                    scale.x = atof(value.c_str());
                }
                else if (attriname == "ScaleY")
                {
                    scale.y = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "AnchorPoint")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "ScaleX")
                {
                    anchorPoint.x = atof(value.c_str());
                }
                else if (attriname == "ScaleY")
                {
                    anchorPoint.y = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "CColor")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "A")
                {
                    color.a = atoi(value.c_str());
                }
                else if (attriname == "R")
                {
                    color.r = atoi(value.c_str());
                }
                else if (attriname == "G")
                {
                    color.g = atoi(value.c_str());
                }
                else if (attriname == "B")
                {
                    color.b = atoi(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "Size")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "X")
                {
                    size.x = atof(value.c_str());
                }
                else if (attriname == "Y")
                {
                    size.y = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "PrePosition")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "X")
                {
                    positionXPercent = atof(value.c_str());
                }
                else if (attriname == "Y")
                {
                    positionYPercent = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        else if (attriname == "PreSize")
        {
            attribute = child->FirstAttribute();

            while (attribute)
            {
                attriname = attribute->Name();
                std::string value = attribute->Value();

                if (attriname == "X")
                {
                    sizeXPercent = atof(value.c_str());
                }
                else if (attriname == "Y")
                {
                    sizeYPercent = atof(value.c_str());
                }

                attribute = attribute->Next();
            }
        }
        child = child->NextSiblingElement();
    }

    RotationSkew f_rotationskew(rotationSkew.x, rotationSkew.y);
    Position f_position(position.x, position.y);
    Scale f_scale(scale.x, scale.y);
    AnchorPoint f_anchortpoint(anchorPoint.x, anchorPoint.y);
    Color f_color(color.a, color.r, color.g, color.b);
    FlatSize f_size(size.x, size.y);
    auto f_layoutComponent = CreateLayoutComponentTable(*builder,
                                                        positionXPercentEnabled,
                                                        positionYPercentEnabled,
                                                        positionXPercent,
                                                        positionYPercent,
                                                        sizeXPercentEnable,
                                                        sizeYPercentEnable,
                                                        sizeXPercent,
                                                        sizeYPercent,
                                                        stretchHorizontalEnabled,
                                                        stretchVerticalEnabled,
                                                        builder->CreateString(horizontalEdge),
                                                        builder->CreateString(verticalEdge),
                                                        leftMargin,
                                                        rightMargin,
                                                        topMargin,
                                                        bottomMargin);

    auto options = CreateWidgetOptions(*builder,
                                       builder->CreateString(name),
                                       (int)actionTag,
                                       &f_rotationskew,
                                       zOrder,
                                       visible,
                                       alpha,
                                       tag,
                                       &f_position,
                                       &f_scale,
                                       &f_anchortpoint,
                                       &f_color,
                                       &f_size,
                                       flipX,
                                       flipY,
                                       ignoreSize,
                                       touchEnabled,
                                       builder->CreateString(frameEvent),
                                       builder->CreateString(customProperty),
                                       0,
                                       0,
                                       f_layoutComponent);

    return *(Offset<Table>*)(&options);
}