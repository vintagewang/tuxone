/*
 * $Id: test1.cpp 51 2010-10-24 04:51:05Z vintage $
 */
/*
 * ≤‚ ‘ConfigPropertyµƒ π”√
 */
#include "ConfigProperty.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class MyConfig : public LWPR::ConfigProperty
{
public:
	MyConfig()
	{
	}

	~MyConfig()
	{
	}

	void DoPropConstruct()
	{
		cout << __FUNCTION__ << endl;

		LWPR::OptionProperty::iterator it = m_OptionProperty.begin();
		for(; it != m_OptionProperty.end(); it++)
		{
			cout << it->first << "=" << it->second << endl;
		}
	}

private:

	static int TUX_GATE_SIZE;
};

int MyConfig::TUX_GATE_SIZE = 100;

int main(int argc, char *argv[])
{
	try
	{
		MyConfig config;
		config.ConfigInit(argc, argv);
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}
