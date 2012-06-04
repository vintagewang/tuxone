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
#ifndef AUTO_CONFIG_H__
#define AUTO_CONFIG_H__
#include <lwpr.h>
#include <string>
#include <assert.h>
#include <limits.h>

using std::string;

class AutoConfig : public LWPR::ConfigProperty
{
public:
	typedef enum THREAD_MODE_E
	{
		MT,
		ST,
	} THREAD_MODE_E;


public:
	AutoConfig();
	virtual ~AutoConfig();
	static AutoConfig* GetInstance();
	virtual void PrintConfig();

public:
	inline const string& TUXONE_APP_GATE_ADDR() const
	{
		return m_TUXONE_APP_GATE_ADDR;
	}

	inline void TUXONE_APP_GATE_ADDR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_APP_GATE_ADDR = value;
	}

	inline const string& TUXONE_APP_SERVER_LISTENER_ADDR() const
	{
		return m_TUXONE_APP_SERVER_LISTENER_ADDR;
	}

	inline void TUXONE_APP_SERVER_LISTENER_ADDR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_APP_SERVER_LISTENER_ADDR = value;
	}

	inline const int& TUXONE_APP_SERVER_CONCURRENTMAX() const
	{
		return m_TUXONE_APP_SERVER_CONCURRENTMAX;
	}

	inline void TUXONE_APP_SERVER_CONCURRENTMAX(int value)
	{
		m_TUXONE_APP_SERVER_CONCURRENTMAX = value;
	}

	inline const int& TUXONE_APP_SERVER_MAXLONGCONNECTIONS() const
	{
		return m_TUXONE_APP_SERVER_MAXLONGCONNECTIONS;
	}

	inline void TUXONE_APP_SERVER_MAXLONGCONNECTIONS(int value)
	{
		m_TUXONE_APP_SERVER_MAXLONGCONNECTIONS = value;
	}

	inline const THREAD_MODE_E& TUXONE_APP_SERVER_THREADMODE() const
	{
		return m_TUXONE_APP_SERVER_THREADMODE;
	}

	inline void TUXONE_APP_SERVER_THREADMODE(THREAD_MODE_E value)
	{
		m_TUXONE_APP_SERVER_THREADMODE = value;
	}

	inline const int& TUXONE_APP_SERVER_THREADPOOL_THREADMIN() const
	{
		return m_TUXONE_APP_SERVER_THREADPOOL_THREADMIN;
	}

	inline void TUXONE_APP_SERVER_THREADPOOL_THREADMIN(int value)
	{
		m_TUXONE_APP_SERVER_THREADPOOL_THREADMIN = value;
	}

	inline const int& TUXONE_APP_SERVER_THREADPOOL_THREADMAX() const
	{
		return m_TUXONE_APP_SERVER_THREADPOOL_THREADMAX;
	}

	inline void TUXONE_APP_SERVER_THREADPOOL_THREADMAX(int value)
	{
		m_TUXONE_APP_SERVER_THREADPOOL_THREADMAX = value;
	}

	inline const int& TUXONE_APP_SERVER_THREADPOOL_THREADMAXIDLE() const
	{
		return m_TUXONE_APP_SERVER_THREADPOOL_THREADMAXIDLE;
	}

	inline void TUXONE_APP_SERVER_THREADPOOL_THREADMAXIDLE(int value)
	{
		m_TUXONE_APP_SERVER_THREADPOOL_THREADMAXIDLE = value;
	}

	inline const int& TUXONE_APP_CLIENT_CACHEDCONNECTIONS() const
	{
		return m_TUXONE_APP_CLIENT_CACHEDCONNECTIONS;
	}

	inline void TUXONE_APP_CLIENT_CACHEDCONNECTIONS(int value)
	{
		m_TUXONE_APP_CLIENT_CACHEDCONNECTIONS = value;
	}

	inline const int& TUXONE_APP_HEART_INTERVAL() const
	{
		return m_TUXONE_APP_HEART_INTERVAL;
	}

	inline void TUXONE_APP_HEART_INTERVAL(int value)
	{
		m_TUXONE_APP_HEART_INTERVAL = value;
	}

	inline const int& TUXONE_APP_BUFFER_CACHEDSIZE() const
	{
		return m_TUXONE_APP_BUFFER_CACHEDSIZE;
	}

	inline void TUXONE_APP_BUFFER_CACHEDSIZE(int value)
	{
		m_TUXONE_APP_BUFFER_CACHEDSIZE = value;
	}

	inline const int& TUXONE_APP_SOCKET_RWTIMEOUT() const
	{
		return m_TUXONE_APP_SOCKET_RWTIMEOUT;
	}

	inline void TUXONE_APP_SOCKET_RWTIMEOUT(int value)
	{
		m_TUXONE_APP_SOCKET_RWTIMEOUT = value;
	}

	inline const bool& TUXONE_APP_LOG_ENABLE() const
	{
		return m_TUXONE_APP_LOG_ENABLE;
	}

	inline void TUXONE_APP_LOG_ENABLE(bool value)
	{
		m_TUXONE_APP_LOG_ENABLE = value;
	}

	inline const int& TUXONE_APP_LOG_LEVEL() const
	{
		return m_TUXONE_APP_LOG_LEVEL;
	}

	inline void TUXONE_APP_LOG_LEVEL(int value)
	{
		m_TUXONE_APP_LOG_LEVEL = value;
	}

	inline const bool& TUXONE_APP_LOG_PRINTWHERE() const
	{
		return m_TUXONE_APP_LOG_PRINTWHERE;
	}

	inline void TUXONE_APP_LOG_PRINTWHERE(bool value)
	{
		m_TUXONE_APP_LOG_PRINTWHERE = value;
	}

	inline const bool& TUXONE_APP_LOG_PRINTFILE() const
	{
		return m_TUXONE_APP_LOG_PRINTFILE;
	}

	inline void TUXONE_APP_LOG_PRINTFILE(bool value)
	{
		m_TUXONE_APP_LOG_PRINTFILE = value;
	}

	inline const bool& TUXONE_APP_LOG_PRINTCONSOLE() const
	{
		return m_TUXONE_APP_LOG_PRINTCONSOLE;
	}

	inline void TUXONE_APP_LOG_PRINTCONSOLE(bool value)
	{
		m_TUXONE_APP_LOG_PRINTCONSOLE = value;
	}


protected:
	virtual void DoPropConstruct();

private:
	string m_TUXONE_APP_GATE_ADDR;
	string m_TUXONE_APP_SERVER_LISTENER_ADDR;
	int m_TUXONE_APP_SERVER_CONCURRENTMAX;
	int m_TUXONE_APP_SERVER_MAXLONGCONNECTIONS;
	THREAD_MODE_E m_TUXONE_APP_SERVER_THREADMODE;
	int m_TUXONE_APP_SERVER_THREADPOOL_THREADMIN;
	int m_TUXONE_APP_SERVER_THREADPOOL_THREADMAX;
	int m_TUXONE_APP_SERVER_THREADPOOL_THREADMAXIDLE;
	int m_TUXONE_APP_CLIENT_CACHEDCONNECTIONS;
	int m_TUXONE_APP_HEART_INTERVAL;
	int m_TUXONE_APP_BUFFER_CACHEDSIZE;
	int m_TUXONE_APP_SOCKET_RWTIMEOUT;
	bool m_TUXONE_APP_LOG_ENABLE;
	int m_TUXONE_APP_LOG_LEVEL;
	bool m_TUXONE_APP_LOG_PRINTWHERE;
	bool m_TUXONE_APP_LOG_PRINTFILE;
	bool m_TUXONE_APP_LOG_PRINTCONSOLE;

};

#define CONFIG() AutoConfig::GetInstance()

#endif // end of APP_CONFIG_H__
