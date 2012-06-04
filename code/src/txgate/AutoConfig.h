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

public:
	AutoConfig();
	virtual ~AutoConfig();
	static AutoConfig* GetInstance();
	virtual void PrintConfig();

public:
	inline const string& TUXONE_GATE_LISTENER_ADDR() const
	{
		return m_TUXONE_GATE_LISTENER_ADDR;
	}

	inline void TUXONE_GATE_LISTENER_ADDR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_GATE_LISTENER_ADDR = value;
	}

	inline const string& TUXONE_GATE_OTHER_ADDR() const
	{
		return m_TUXONE_GATE_OTHER_ADDR;
	}

	inline void TUXONE_GATE_OTHER_ADDR(const char* value)
	{
		assert(value != NULL);
		m_TUXONE_GATE_OTHER_ADDR = value;
	}

	inline const int& TUXONE_GATE_SYNC_INTERVAL() const
	{
		return m_TUXONE_GATE_SYNC_INTERVAL;
	}

	inline void TUXONE_GATE_SYNC_INTERVAL(int value)
	{
		m_TUXONE_GATE_SYNC_INTERVAL = value;
	}

	inline const int& TUXONE_GATE_CONCURRENT_MAX() const
	{
		return m_TUXONE_GATE_CONCURRENT_MAX;
	}

	inline void TUXONE_GATE_CONCURRENT_MAX(int value)
	{
		m_TUXONE_GATE_CONCURRENT_MAX = value;
	}

	inline const int& TUXONE_GATE_CONCURRENT_LOWWATER() const
	{
		return m_TUXONE_GATE_CONCURRENT_LOWWATER;
	}

	inline void TUXONE_GATE_CONCURRENT_LOWWATER(int value)
	{
		m_TUXONE_GATE_CONCURRENT_LOWWATER = value;
	}

	inline const int& TUXONE_GATE_CONCURRENT_HIGHWATER() const
	{
		return m_TUXONE_GATE_CONCURRENT_HIGHWATER;
	}

	inline void TUXONE_GATE_CONCURRENT_HIGHWATER(int value)
	{
		m_TUXONE_GATE_CONCURRENT_HIGHWATER = value;
	}

	inline const int& TUXONE_GATE_HOUSEKEEPING_INTERVAL() const
	{
		return m_TUXONE_GATE_HOUSEKEEPING_INTERVAL;
	}

	inline void TUXONE_GATE_HOUSEKEEPING_INTERVAL(int value)
	{
		m_TUXONE_GATE_HOUSEKEEPING_INTERVAL = value;
	}

	inline const int& TUXONE_GATE_SOCKET_RWTIMEOUT() const
	{
		return m_TUXONE_GATE_SOCKET_RWTIMEOUT;
	}

	inline void TUXONE_GATE_SOCKET_RWTIMEOUT(int value)
	{
		m_TUXONE_GATE_SOCKET_RWTIMEOUT = value;
	}

	inline const int& TUXONE_GATE_BBL_BERTHMAX() const
	{
		return m_TUXONE_GATE_BBL_BERTHMAX;
	}

	inline void TUXONE_GATE_BBL_BERTHMAX(int value)
	{
		m_TUXONE_GATE_BBL_BERTHMAX = value;
	}

	inline const int& TUXONE_GATE_WSH_MIN() const
	{
		return m_TUXONE_GATE_WSH_MIN;
	}

	inline void TUXONE_GATE_WSH_MIN(int value)
	{
		m_TUXONE_GATE_WSH_MIN = value;
	}

	inline const int& TUXONE_GATE_WSH_MAX() const
	{
		return m_TUXONE_GATE_WSH_MAX;
	}

	inline void TUXONE_GATE_WSH_MAX(int value)
	{
		m_TUXONE_GATE_WSH_MAX = value;
	}

	inline const int& TUXONE_GATE_WSH_ACCESSMAX() const
	{
		return m_TUXONE_GATE_WSH_ACCESSMAX;
	}

	inline void TUXONE_GATE_WSH_ACCESSMAX(int value)
	{
		m_TUXONE_GATE_WSH_ACCESSMAX = value;
	}

	inline const int& TUXONE_GATE_WSH_THREADPOOL_THREADMIN() const
	{
		return m_TUXONE_GATE_WSH_THREADPOOL_THREADMIN;
	}

	inline void TUXONE_GATE_WSH_THREADPOOL_THREADMIN(int value)
	{
		m_TUXONE_GATE_WSH_THREADPOOL_THREADMIN = value;
	}

	inline const int& TUXONE_GATE_WSH_THREADPOOL_THREADMAX() const
	{
		return m_TUXONE_GATE_WSH_THREADPOOL_THREADMAX;
	}

	inline void TUXONE_GATE_WSH_THREADPOOL_THREADMAX(int value)
	{
		m_TUXONE_GATE_WSH_THREADPOOL_THREADMAX = value;
	}

	inline const int& TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE() const
	{
		return m_TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE;
	}

	inline void TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE(int value)
	{
		m_TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE = value;
	}

	inline const int& TUXONE_GATE_WSH_SERVERVALIDTIME() const
	{
		return m_TUXONE_GATE_WSH_SERVERVALIDTIME;
	}

	inline void TUXONE_GATE_WSH_SERVERVALIDTIME(int value)
	{
		m_TUXONE_GATE_WSH_SERVERVALIDTIME = value;
	}

	inline const int& TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL() const
	{
		return m_TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL;
	}

	inline void TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL(int value)
	{
		m_TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL = value;
	}

	inline const int& TUXONE_GATE_WSH_APP_HEART_INTERVAL() const
	{
		return m_TUXONE_GATE_WSH_APP_HEART_INTERVAL;
	}

	inline void TUXONE_GATE_WSH_APP_HEART_INTERVAL(int value)
	{
		m_TUXONE_GATE_WSH_APP_HEART_INTERVAL = value;
	}

	inline const bool& TUXONE_GATE_LOG_ENABLE() const
	{
		return m_TUXONE_GATE_LOG_ENABLE;
	}

	inline void TUXONE_GATE_LOG_ENABLE(bool value)
	{
		m_TUXONE_GATE_LOG_ENABLE = value;
	}

	inline const int& TUXONE_GATE_LOG_LEVEL() const
	{
		return m_TUXONE_GATE_LOG_LEVEL;
	}

	inline void TUXONE_GATE_LOG_LEVEL(int value)
	{
		m_TUXONE_GATE_LOG_LEVEL = value;
	}

	inline const bool& TUXONE_GATE_LOG_PRINTWHERE() const
	{
		return m_TUXONE_GATE_LOG_PRINTWHERE;
	}

	inline void TUXONE_GATE_LOG_PRINTWHERE(bool value)
	{
		m_TUXONE_GATE_LOG_PRINTWHERE = value;
	}

	inline const bool& TUXONE_GATE_LOG_PRINTFILE() const
	{
		return m_TUXONE_GATE_LOG_PRINTFILE;
	}

	inline void TUXONE_GATE_LOG_PRINTFILE(bool value)
	{
		m_TUXONE_GATE_LOG_PRINTFILE = value;
	}

	inline const bool& TUXONE_GATE_LOG_PRINTCONSOLE() const
	{
		return m_TUXONE_GATE_LOG_PRINTCONSOLE;
	}

	inline void TUXONE_GATE_LOG_PRINTCONSOLE(bool value)
	{
		m_TUXONE_GATE_LOG_PRINTCONSOLE = value;
	}


protected:
	virtual void DoPropConstruct();

private:
	string m_TUXONE_GATE_LISTENER_ADDR;
	string m_TUXONE_GATE_OTHER_ADDR;
	int m_TUXONE_GATE_SYNC_INTERVAL;
	int m_TUXONE_GATE_CONCURRENT_MAX;
	int m_TUXONE_GATE_CONCURRENT_LOWWATER;
	int m_TUXONE_GATE_CONCURRENT_HIGHWATER;
	int m_TUXONE_GATE_HOUSEKEEPING_INTERVAL;
	int m_TUXONE_GATE_SOCKET_RWTIMEOUT;
	int m_TUXONE_GATE_BBL_BERTHMAX;
	int m_TUXONE_GATE_WSH_MIN;
	int m_TUXONE_GATE_WSH_MAX;
	int m_TUXONE_GATE_WSH_ACCESSMAX;
	int m_TUXONE_GATE_WSH_THREADPOOL_THREADMIN;
	int m_TUXONE_GATE_WSH_THREADPOOL_THREADMAX;
	int m_TUXONE_GATE_WSH_THREADPOOL_THREADMAXIDLE;
	int m_TUXONE_GATE_WSH_SERVERVALIDTIME;
	int m_TUXONE_GATE_WSH_HOUSEKEEPING_INTERVAL;
	int m_TUXONE_GATE_WSH_APP_HEART_INTERVAL;
	bool m_TUXONE_GATE_LOG_ENABLE;
	int m_TUXONE_GATE_LOG_LEVEL;
	bool m_TUXONE_GATE_LOG_PRINTWHERE;
	bool m_TUXONE_GATE_LOG_PRINTFILE;
	bool m_TUXONE_GATE_LOG_PRINTCONSOLE;

};

#define CONFIG() AutoConfig::GetInstance()

#endif // end of APP_CONFIG_H__
