/**
 * Copyright 2012 Wangxr, vintage.wang@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "Logger.h"
#include "StringUtil.h"
#include "StringBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace LWPR
{
#define LOG_FUNCTION_C(name, level) \
	void Logger::name(const char* file, int line, const std::string& message) \
	{                                                                         \
		if(IsPriorityEnabled(level))                                          \
		{                                                                     \
			StringBuffer buf(LOG_DEFAULT_BUF_SIZE);                           \
			FillFrontLogLine(file, line, level, buf);                         \
			buf.Append(message.c_str());                                      \
			buf.Append("\n");                                                 \
			DoLog(buf.str(), buf.Length());                                   \
		}                                                                     \
	}

#define LOG_FUNCTION_CPP(name, level) \
	void Logger::name(const char* file, int line, const char* stringFormat, ...) \
	{                                                                            \
		if(IsPriorityEnabled(level))                                             \
		{                                                                        \
			va_list va;                                                          \
			StringBuffer buf(LOG_DEFAULT_BUF_SIZE);                              \
			FillFrontLogLine(file, line, level, buf);                            \
                                                                                 \
			va_start(va, stringFormat);                                          \
			StringUtil::VForm(buf, stringFormat, va);                            \
			va_end(va);                                                          \
                                                                                 \
			buf.Append("\n");                                                    \
			DoLog(buf.str(), buf.Length());                                      \
		}                                                                        \
	}

	//----------------------------------------------------------------------------------
	// struct logger_option
	//----------------------------------------------------------------------------------
	logger_option::logger_option() :
		strLogFileName(""),
		nLogLevel(LOGGER_LEVEL_WARN),
		bPrintFile(false),
		bPrintConsole(false),
		bPrintWhere(false)
	{
	}

	//----------------------------------------------------------------------------------
	// class Logger
	//----------------------------------------------------------------------------------
	Logger::Logger():
		m_nfd(-1)
	{
		const char* p = getenv("LWPR_LOGGER_LEVEL");
		if(p != NULL && strlen(p) > 0)
		{
			m_LoggerOption.nLogLevel = atoi(p);
		}

		p = getenv("LWPR_LOGGER_WHERE");
		if(p != NULL && !strcmp(p, "true"))
		{
			m_LoggerOption.bPrintWhere = true;
		}
	}

	Logger::~Logger()
	{
		if(m_nfd != -1)
		{
			close(m_nfd);
		}
	}

	Logger* Logger::GetInstance()
	{
		static Logger *singleton = new Logger();
		return singleton;
	}

	bool Logger::Initialize(const LOGGER_OPTION_T& opt)
	{
		bool bResult = true;

		// 允许重复初始化（为多进程准备）
		if(m_nfd != -1)
		{
			close(m_nfd);
		}

		if(opt.bPrintFile)
		{
			bResult = opt.strLogFileName.length() > 0;
			if(bResult)
			{
				m_nfd = open(opt.strLogFileName.c_str(), O_RDWR | O_APPEND | O_CREAT, 0644);
				bResult = (m_nfd != -1);
			}
		}

		m_LoggerOption = opt;

		return bResult;
	}

	void Logger::FillFrontLogLine(const char* file, int line, LOGGER_LEVEL_T level, Buffer& buf)
	{
		static const char* LEVEL_DESC[] =
		{
			" NONE",
			"FATAL",
			"ERROR",
			" WARN",
			" INFO",
			"DEBUG",
			"TRACE"
		};

		assert(buf.Capacity() > 64);

		if(m_LoggerOption.bPrintWhere)
		{
			snprintf(buf.Inout(), 64, "%s %s %24s:%04d, ",
			         Utility::DateTimeNow().c_str(), LEVEL_DESC[level],
			         FixFileString(file).c_str(), line);
			buf.Size(57);
		}
		else
		{
			snprintf(buf.Inout(), 64, "%s %s ", Utility::DateTimeNow().c_str(), LEVEL_DESC[level]);
			buf.Size(26);
		}
	}

	bool Logger::IsPriorityEnabled(LOGGER_LEVEL_T level)
	{
		return (m_LoggerOption.nLogLevel >= level);
	}


	void Logger::DoLog(const char* content, int size)
	{
		assert(content != NULL);
		assert(size != 0);

		if(m_LoggerOption.bPrintConsole)
		{
			write(STDOUT_FILENO, content, size);
		}

		if(m_LoggerOption.bPrintFile)
		{
			write(m_nfd, content, size);
		}
	}

	std::string Logger::FixFileString(const char* file)
	{
		assert(file != NULL);

		std::string strFile = file;

#ifdef WIN32
		std::string::size_type pos = strFile.find_last_of('\\');
		strFile = strFile.substr(pos + 1);
#endif

		if(strFile.length() > 24)
		{
			strFile = strFile.substr(strFile.length() - 24);
		}

		return strFile;
	}

	LOG_FUNCTION_C(trace, LOGGER_LEVEL_TRACE)
	LOG_FUNCTION_CPP(trace, LOGGER_LEVEL_TRACE)

	LOG_FUNCTION_C(debug, LOGGER_LEVEL_DEBUG)
	LOG_FUNCTION_CPP(debug, LOGGER_LEVEL_DEBUG)

	LOG_FUNCTION_C(info, LOGGER_LEVEL_INFO)
	LOG_FUNCTION_CPP(info, LOGGER_LEVEL_INFO)

	LOG_FUNCTION_C(warn, LOGGER_LEVEL_WARN)
	LOG_FUNCTION_CPP(warn, LOGGER_LEVEL_WARN)

	LOG_FUNCTION_C(error, LOGGER_LEVEL_ERROR)
	LOG_FUNCTION_CPP(error, LOGGER_LEVEL_ERROR)

	LOG_FUNCTION_C(fatal, LOGGER_LEVEL_FATAL)
	LOG_FUNCTION_CPP(fatal, LOGGER_LEVEL_FATAL)

	//----------------------------------------------------------------------------------
	// class LogFunction
	//----------------------------------------------------------------------------------
	LogFunction::LogFunction(const char* file, int line, const char* pfun)
		: m_strFileName(file),
		  m_strFunctionName(pfun),
		  m_nLine(line)
	{
		assert(file != NULL);
		assert(pfun != NULL);
		logger->trace(file, line, "Enter into %s", m_strFunctionName.c_str());
	}

	LogFunction::~LogFunction()
	{
		logger->trace(m_strFileName.c_str(), m_nLine, "Leave from %s", m_strFunctionName.c_str());
	}

};
