//
// Created by choiheungbae on 18. 3. 19.
//
#pragma once

#include "ILog.h"

namespace NServerNetLib
{
    class BasicLog: public ILog
    {
    public:
        BasicLog() {}
        virtual ~BasicLog() {}

    protected:
        virtual void Error(const char* pText)
        {
            printf("[Error]%s \n", pText);
        }

        virtual void Warn(const char* pText)
        {
            printf("[warn]%s \n", pText);
        }

        virtual void Debug(const char* pText)
        {
            printf("[Debug]%s \n", pText);
        }

        virtual void Trace(const char* pText)
        {
            printf("[Trace]%s \n", pText);
        }

        virtual void Info(const char* pText)
        {
            printf("[info]%s \n", pText);
        }
    };
}
