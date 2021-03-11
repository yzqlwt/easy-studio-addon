//
// Created by yzqlwt on 2020/12/1.
//

#ifndef TESTCSD_OBJECTFACTORY_H
#define TESTCSD_OBJECTFACTORY_H


/****************************************************************************
Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#ifndef __TRIGGERFACTORY_H__
#define __TRIGGERFACTORY_H__

#include "NodeReaderProtocol.h"
#include <string>
#include <unordered_map>
#include <functional>

class ObjectFactory {
public:
    typedef NodeReaderProtocol *(*Instance)(void);

    typedef std::function<NodeReaderProtocol *(void)> InstanceFunc;

    struct TInfo {
        TInfo(void);

        TInfo(const std::string &type, Instance ins = nullptr);

        TInfo(const std::string &type, InstanceFunc ins = nullptr);

        TInfo(const TInfo &t);

        ~TInfo(void);

        TInfo &operator=(const TInfo &t);

        std::string _class;
        Instance _fun;
        InstanceFunc _func;
    };

    typedef std::unordered_map<std::string, TInfo> FactoryMap;

    static ObjectFactory *getInstance();

    static void destroyInstance();

    NodeReaderProtocol *createObject(const std::string &name);

    void registerType(const TInfo &t);

    void removeAll();

protected:
    ObjectFactory(void);

    virtual ~ObjectFactory(void);

private:
    static ObjectFactory *_sharedFactory;
    FactoryMap _typeMap;
};

#endif


#endif //TESTCSD_OBJECTFACTORY_H
