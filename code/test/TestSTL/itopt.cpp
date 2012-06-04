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
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <list>

using std::cout;
using std::cerr;
using std::endl;

template< class T2 = std::list<T> >
inline T& LWPR_LIST_AT(T2& s, size_t index)
{
	assert(index < s.size());

	T2::iterator it = s.begin();

	for(size_t i = 0; i < index; i++, it++);
	return *it;
}

static int run(int argc, char** argv)
{
	std::list <int> nums;

	nums.push_back(1);
	nums.push_back(2);
	nums.push_back(4);
	nums.push_back(6);
	nums.push_back(8);

	//cout << LWPR_LIST_AT(nums, 1) << endl;
	sort

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
