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
#ifndef LWPR_AUTO_HANDLE_H__
#define LWPR_AUTO_HANDLE_H__

/**
 * AutoHandle主要用来管理动态申请的局部对象或者内存
 */
template <class T>
class AutoHandle
{
public:
	T *operator->() const
	{
		return m_p;
	}

	operator T*() const
	{
		return m_p;
	}

	AutoHandle() : m_p(0) {}
	AutoHandle(T *p) : m_p(p) {}
	~AutoHandle()
	{
		delete m_p;
	}

	void operator=(T *p)
	{
		m_p = p;
	}

	T *ptr()
	{
		return m_p;
	}

private:
	T   *m_p;
};

#endif
