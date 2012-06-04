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
#include "AtomicBoolean.h"

namespace LWPR
{
#define BOOL_2_INT(b)		((b) ? 1 : 0)
#define INT_2_BOOL(n)		((n != 0) ? true : false)

	AtomicBoolean::AtomicBoolean(bool initialValue)
		: m_nValue(BOOL_2_INT(initialValue))
	{
	}

	bool AtomicBoolean::CompareAndSet(bool expect, bool update)
	{
		return __sync_bool_compare_and_swap(&m_nValue, BOOL_2_INT(expect), BOOL_2_INT(update));
	}

	bool AtomicBoolean::Get() const
	{
		return INT_2_BOOL(m_nValue);
	}

	bool AtomicBoolean::GetAndSet(bool newValue)
	{
		while(true)
		{
			bool current = Get();
			if(CompareAndSet(current, newValue))
				return current;
		}
	}

	void AtomicBoolean::Set(bool newValue)
	{
		m_nValue = BOOL_2_INT(newValue);
	}

	AtomicBoolean::operator bool () const
	{
		return Get();
	}
};
