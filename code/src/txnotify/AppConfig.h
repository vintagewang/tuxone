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
#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__
#include <lwpr.h>
#include <string>
#include <assert.h>

using std::string;

class AppConfig : public LWPR::ConfigProperty
{
public:
	typedef enum DELIVER_MODE_E
	{
		INTURN,
		FIXED,
		ALL,
	} DELIVER_MODE_E;


public:
	AppConfig();
	virtual ~AppConfig();
	static AppConfig* GetInstance();
	virtual void PrintConfig();

public:
	inline const string& TUXONE_NOTIFY_ADDR() const
	{
		return m_TUXONE_NOTIFY_ADDR;
	}

	inline void TUXONE_NOTIFY_ADDR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_NOTIFY_ADDR = value;
	}

	inline const int& TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN() const
	{
		return m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN;
	}

	inline void TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN(int value)
	{
		m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN = value;
	}

	inline const int& TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX() const
	{
		return m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX;
	}

	inline void TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX(int value)
	{
		m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX = value;
	}

	inline const int& TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE() const
	{
		return m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE;
	}

	inline void TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE(int value)
	{
		m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE = value;
	}

	inline const int& TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN() const
	{
		return m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN;
	}

	inline void TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN(int value)
	{
		m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN = value;
	}

	inline const int& TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX() const
	{
		return m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX;
	}

	inline void TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX(int value)
	{
		m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX = value;
	}

	inline const int& TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE() const
	{
		return m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE;
	}

	inline void TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE(int value)
	{
		m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE = value;
	}

	inline const bool& TUXONE_NOTIFY_LOG_ENABLE() const
	{
		return m_TUXONE_NOTIFY_LOG_ENABLE;
	}

	inline void TUXONE_NOTIFY_LOG_ENABLE(bool value)
	{
		m_TUXONE_NOTIFY_LOG_ENABLE = value;
	}

	inline const int& TUXONE_NOTIFY_LOG_LEVEL() const
	{
		return m_TUXONE_NOTIFY_LOG_LEVEL;
	}

	inline void TUXONE_NOTIFY_LOG_LEVEL(int value)
	{
		m_TUXONE_NOTIFY_LOG_LEVEL = value;
	}

	inline const bool& TUXONE_NOTIFY_LOG_PRINTFILE() const
	{
		return m_TUXONE_NOTIFY_LOG_PRINTFILE;
	}

	inline void TUXONE_NOTIFY_LOG_PRINTFILE(bool value)
	{
		m_TUXONE_NOTIFY_LOG_PRINTFILE = value;
	}

	inline const bool& TUXONE_NOTIFY_LOG_PRINTCONSOLE() const
	{
		return m_TUXONE_NOTIFY_LOG_PRINTCONSOLE;
	}

	inline void TUXONE_NOTIFY_LOG_PRINTCONSOLE(bool value)
	{
		m_TUXONE_NOTIFY_LOG_PRINTCONSOLE = value;
	}

	inline const string& TUXONE_NOTIFY_DBDIR() const
	{
		return m_TUXONE_NOTIFY_DBDIR;
	}

	inline void TUXONE_NOTIFY_DBDIR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_NOTIFY_DBDIR = value;
	}

	inline const string& TUXONE_NOTIFY_DEFAULTQUEUENAME() const
	{
		return m_TUXONE_NOTIFY_DEFAULTQUEUENAME;
	}

	inline void TUXONE_NOTIFY_DEFAULTQUEUENAME(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_NOTIFY_DEFAULTQUEUENAME = value;
	}


protected:
	virtual void DoPropConstruct();

private:
	string m_TUXONE_NOTIFY_ADDR;
	int m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMIN;
	int m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAX;
	int m_TUXONE_NOTIFY_RECEIVER_THREADPOOL_THREADMAXIDLE;
	int m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMIN;
	int m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAX;
	int m_TUXONE_NOTIFY_SENDER_THREADPOOL_THREADMAXIDLE;
	bool m_TUXONE_NOTIFY_LOG_ENABLE;
	int m_TUXONE_NOTIFY_LOG_LEVEL;
	bool m_TUXONE_NOTIFY_LOG_PRINTFILE;
	bool m_TUXONE_NOTIFY_LOG_PRINTCONSOLE;
	string m_TUXONE_NOTIFY_DBDIR;
	string m_TUXONE_NOTIFY_DEFAULTQUEUENAME;

};

#define CONFIG() AppConfig::GetInstance()

#endif // end of APP_CONFIG_H__
