/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/



#include "./ListViewReader.h"
#include "../CSParseBinary_generated.h"
#include "../FlatBuffersSerialize.h"
#include "ccTypes.h"
#include "tinyxml2.h"
#include "../flatbuffers/flatbuffers.h"

using namespace flatbuffers;

    static const char* P_Direction = "direction";
    static const char* P_ItemMargin = "itemMargin";
    
    static ListViewReader* instanceListViewReader = nullptr;
    
    
    ListViewReader* ListViewReader::getInstance()
    {
        if (!instanceListViewReader)
        {
            instanceListViewReader = new (std::nothrow) ListViewReader();
        }
        return instanceListViewReader;
    }
    
    Offset<Table> ListViewReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                               flatbuffers::FlatBufferBuilder *builder)
    {
        auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
        auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);
        
        std::string path;
        std::string plistFile;
        int resourceType = 0;
        
        bool clipEnabled = false;
        Color3B bgColor;
        Color3B bgStartColor;
        Color3B bgEndColor;
        int colorType = 0;
        int bgColorOpacity = 255;
        Vec2 colorVector(0.0f, -0.5f);
        Rect capInsets;
        Size scale9Size;
        bool backGroundScale9Enabled = false;
        Size innerSize(200, 300);
        int direction = 0;
        bool bounceEnabled = false;
        int itemMargin = 0;
        std::string directionType;
        std::string horizontalType;
        std::string verticalType;
        
        // attributes
        auto attribute = objectData->FirstAttribute();
        while (attribute)
        {
            std::string name = attribute->Name();
            std::string value = attribute->Value();
            
            if (name == "ClipAble")
            {
                clipEnabled = (value == "True") ? true : false;
            }
            else if (name == "ComboBoxIndex")
            {
                colorType = atoi(value.c_str());
            }
            else if (name == "BackColorAlpha")
            {
                bgColorOpacity = atoi(value.c_str());
            }
            else if (name == "Scale9Enable")
            {
                if (value == "True")
                {
                    backGroundScale9Enabled = true;
                }
            }
            else if (name == "Scale9OriginX")
            {
                capInsets.origin.x = atof(value.c_str());
            }
            else if (name == "Scale9OriginY")
            {
                capInsets.origin.y = atof(value.c_str());
            }
            else if (name == "Scale9Width")
            {
                capInsets.size.width = atof(value.c_str());
            }
            else if (name == "Scale9Height")
            {
                capInsets.size.height = atof(value.c_str());
            }
            else if (name == "DirectionType")
            {
                directionType = value;
            }
            else if (name == "HorizontalType")
            {
                horizontalType = value;
            }
            else if (name == "VerticalType")
            {
                verticalType = value;
            }
            else if (name == "IsBounceEnabled")
            {
                bounceEnabled = (value == "True") ? true : false;
            }
            else if (name == "ItemMargin")
            {
                itemMargin = atoi(value.c_str());
            }
            
            attribute = attribute->Next();
        }
        
        // child elements
        const tinyxml2::XMLElement* child = objectData->FirstChildElement();
        while (child)
        {
            std::string name = child->Name();
            
            if (name == "InnerNodeSize")
            {
                auto attributeInnerNodeSize = child->FirstAttribute();
                while (attributeInnerNodeSize)
                {
                    name = attributeInnerNodeSize->Name();
                    std::string value = attributeInnerNodeSize->Value();
                    
                    if (name == "Width")
                    {
                        innerSize.width = atof(value.c_str());
                    }
                    else if (name == "Height")
                    {
                        innerSize.height = atof(value.c_str());
                    }
                    
                    attributeInnerNodeSize = attributeInnerNodeSize->Next();
                }
            }
            else if (name == "Size" && backGroundScale9Enabled)
            {
                auto attributeSize = child->FirstAttribute();
                
                while (attributeSize)
                {
                    name = attributeSize->Name();
                    std::string value = attributeSize->Value();
                    
                    if (name == "X")
                    {
                        scale9Size.width = atof(value.c_str());
                    }
                    else if (name == "Y")
                    {
                        scale9Size.height = atof(value.c_str());
                    }
                    
                    attributeSize = attributeSize->Next();
                }
            }
            else if (name == "SingleColor")
            {
                auto attributeSingleColor = child->FirstAttribute();
                
                while (attributeSingleColor)
                {
                    name = attributeSingleColor->Name();
                    std::string value = attributeSingleColor->Value();
                    
                    if (name == "R")
                    {
                        bgColor.r = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        bgColor.g = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        bgColor.b = atoi(value.c_str());
                    }
                    
                    attributeSingleColor = attributeSingleColor->Next();
                }
            }
            else if (name == "EndColor")
            {
                auto attributeEndColor = child->FirstAttribute();
                
                while (attributeEndColor)
                {
                    name = attributeEndColor->Name();
                    std::string value = attributeEndColor->Value();
                    
                    if (name == "R")
                    {
                        bgEndColor.r = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        bgEndColor.g = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        bgEndColor.b = atoi(value.c_str());
                    }
                    
                    attributeEndColor = attributeEndColor->Next();
                }
            }
            else if (name == "FirstColor")
            {
                auto attributeFirstColor = child->FirstAttribute();
                
                while (attributeFirstColor)
                {
                    name = attributeFirstColor->Name();
                    std::string value = attributeFirstColor->Value();
                    
                    if (name == "R")
                    {
                        bgStartColor.r = atoi(value.c_str());
                    }
                    else if (name == "G")
                    {
                        bgStartColor.g = atoi(value.c_str());
                    }
                    else if (name == "B")
                    {
                        bgStartColor.b = atoi(value.c_str());
                    }
                    
                    attributeFirstColor = attributeFirstColor->Next();
                }
            }
            else if (name == "ColorVector")
            {
                auto attributeColorVector = child->FirstAttribute();
                while (attributeColorVector)
                {
                    name = attributeColorVector->Name();
                    std::string value = attributeColorVector->Value();
                    
                    if (name == "ScaleX")
                    {
                        colorVector.x = atof(value.c_str());
                    }
                    else if (name == "ScaleY")
                    {
                        colorVector.y = atof(value.c_str());
                    }
                    
                    attributeColorVector = attributeColorVector->Next();
                }
            }
            else if (name == "FileData")
            {
                std::string texture;
                std::string texturePng;
                
                auto attributeFileData = child->FirstAttribute();
                
                while (attributeFileData)
                {
                    name = attributeFileData->Name();
                    std::string value = attributeFileData->Value();
                    
                    if (name == "Path")
                    {
                        path = value;
                    }
                    else if (name == "Type")
                    {
                        resourceType = getResourceType(value);
                    }
                    else if (name == "Plist")
                    {
                        plistFile = value;
                        texture = value;
                    }
                    
                    attributeFileData = attributeFileData->Next();
                }
                
                if (resourceType == 1)
                {
                    FlatBuffersSerialize* fbs = FlatBuffersSerialize::getInstance();
                    fbs->_textures.push_back(builder->CreateString(texture));                    
                }
            }
            
            child = child->NextSiblingElement();
        }
        
        Color f_bgColor(255, bgColor.r, bgColor.g, bgColor.b);
        Color f_bgStartColor(255, bgStartColor.r, bgStartColor.g, bgStartColor.b);
        Color f_bgEndColor(255, bgEndColor.r, bgEndColor.g, bgEndColor.b);
        ColorVector f_colorVector(colorVector.x, colorVector.y);
        CapInsets f_capInsets(capInsets.origin.x, capInsets.origin.y, capInsets.size.width, capInsets.size.height);
        FlatSize f_scale9Size(scale9Size.width, scale9Size.height);
        FlatSize f_innerSize(innerSize.width, innerSize.height);
        
        auto options = CreateListViewOptions(*builder,
                                             widgetOptions,
                                             CreateResourceData(*builder,
                                                                builder->CreateString(path),
                                                                builder->CreateString(plistFile),
                                                                resourceType),
                                             clipEnabled,
                                             &f_bgColor,
                                             &f_bgStartColor,
                                             &f_bgEndColor,
                                             colorType,
                                             bgColorOpacity,
                                             &f_colorVector,
                                             &f_capInsets,
                                             &f_scale9Size,
                                             backGroundScale9Enabled,
                                             &f_innerSize,
                                             direction,
                                             bounceEnabled,
                                             itemMargin,
                                             builder->CreateString(directionType),
                                             builder->CreateString(horizontalType), 
                                             builder->CreateString(verticalType));
        
        return *(Offset<Table>*)(&options);
    }
    
    int ListViewReader::getResourceType(std::string key)
    {
        if(key == "Normal" || key == "Default")
        {
            return 	0;
        }
        return 1;
    }

