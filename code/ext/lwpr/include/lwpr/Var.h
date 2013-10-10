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
#ifndef LWPR_VAR_H__
#define LWPR_VAR_H__

template <class T>
class Var
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

	Var() : m_p(0) {}
	Var(T *p) : m_p(p) {}
	Var(const Var<T>& v) : m_p(static_cast<T*>(T::Duplicate(v))) {}
	~Var()
	{
		T::Release(m_p);
	}

	void operator=(T *p)
	{
		T::Release(m_p);
		m_p = p;
	}

	void operator=(const Var<T>& v)
	{
		T::Release(m_p);
		m_p = static_cast<T*>(T::Duplicate(v));
	}

	void operator+=(T *p)
	{
		T::Release(m_p);
		m_p = static_cast<T*>(T::Duplicate(p));
	}

	static T *Duplicate(T *p)
	{
		return static_cast<T*>(T::Duplicate(p));
	}

	T *ptr()
	{
		return m_p;
	}

	T *retn()
	{
		T *p = m_p;
		m_p = 0;
		return p;
	}
private:
	T   *m_p;
};

#define DECLAREVAR(T)   typedef Var<T> T ## _var;   \
                        typedef T* T ## _ptr;

#define DECLAREPTR(T)   typedef T* T ## _ptr;

#endif
