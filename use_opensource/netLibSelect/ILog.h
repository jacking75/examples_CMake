#pragma once

#include <stdio.h>
#include <stdarg.h>

//#pragma warning (disable: 4100)

namespace NServerNetLib
{
	const int MAX_LOG_STRING_LENGTH = 256;

	enum class LOG_TYPE : short
	{
		L_TRACE = 1
		, L_DEBUG = 2
		, L_WARN = 3
		, L_ERROR = 4
		, L_INFO = 5, 
	};


	class ILog
	{
	public:
		ILog() {}
		virtual ~ILog() {}

		virtual void Write(const LOG_TYPE nType, const char* pFormat, ...)
		{
			char szText[MAX_LOG_STRING_LENGTH];

			va_list args;
			va_start(args, pFormat);
            vsnprintf(szText, MAX_LOG_STRING_LENGTH, pFormat, args);
			va_end(args);

			switch (nType)
			{
			case LOG_TYPE::L_INFO:
				Info(szText);
				break;
			case LOG_TYPE::L_ERROR:
				Error(szText);
				break;
			case LOG_TYPE::L_WARN:
				Warn(szText);
				break;
			case LOG_TYPE::L_DEBUG:
				Debug(szText);
				break;
			case LOG_TYPE::L_TRACE:
				Info(szText);
				break;
			default:
				break;
			}
		}


	protected:
		virtual void Error(const char* pText) = 0;
		virtual void Warn(const char* pText) = 0;
		virtual void Debug(const char* pText) = 0;
		virtual void Trace(const char* pText) = 0;
		virtual void Info(const char* pText) = 0;

	};
}