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
#include "AtomicInteger.h"

namespace LWPR
{

	AtomicInteger::AtomicInteger(int initialValue)
		: m_nValue(initialValue)
	{
	}

	AtomicInteger::~AtomicInteger()
	{
	}

	int AtomicInteger::Get() const
	{
		return m_nValue;
	}

	void AtomicInteger::Set(int newValue)
	{
		m_nValue = newValue;
	}

	bool AtomicInteger::CompareAndSet(int expect, int update)
	{
		return CompareAndSet(&m_nValue, expect, update);
	}

	int AtomicInteger::GetAndIncrement()
	{
		return GetAndIncrement(&m_nValue);
	}

	int AtomicInteger::GetAndDecrement()
	{
		return GetAndDecrement(&m_nValue);
	}

	int AtomicInteger::GetAndAdd(int delta)
	{
		return GetAndAdd(&m_nValue, delta);
	}

	int AtomicInteger::AddAndGet(int delta)
	{
		return AddAndGet(&m_nValue, delta);
	}

	int AtomicInteger::IncrementAndGet()
	{
		return IncrementAndGet(&m_nValue);
	}

	int AtomicInteger::DecrementAndGet()
	{
		return DecrementAndGet(&m_nValue);
	}

	int AtomicInteger::operator = (int newValue)
	{
		this->Set(newValue);
		return newValue;
	}

	int AtomicInteger::operator++()
	{
		return this->IncrementAndGet();
	}

	int AtomicInteger::operator++(int)
	{
		return this->GetAndIncrement();
	}

	int AtomicInteger::operator--()
	{
		return this->DecrementAndGet();
	}

	int AtomicInteger::operator--(int)
	{
		return this->GetAndDecrement();
	}

	AtomicInteger::operator int () const
	{
		return this->Get();
	}

	bool AtomicInteger::CompareAndSet(volatile int *ptr, int expect, int update)
	{
		return __sync_bool_compare_and_swap(ptr, expect, update);
	}

	int AtomicInteger::GetAndIncrement(volatile int *ptr)
	{
		return __sync_fetch_and_add(ptr, 1);
	}

	int AtomicInteger::GetAndDecrement(volatile int *ptr)
	{
		return __sync_fetch_and_sub(ptr, 1);
	}

	int AtomicInteger::GetAndAdd(volatile int *ptr, int delta)
	{
		return __sync_fetch_and_add(ptr, delta);
	}

	int AtomicInteger::AddAndGet(volatile int *ptr, int delta)
	{
		return __sync_add_and_fetch(ptr, delta);
	}

	int AtomicInteger::IncrementAndGet(volatile int *ptr)
	{
		return __sync_add_and_fetch(ptr, 1);
	}

	int AtomicInteger::DecrementAndGet(volatile int *ptr)
	{
		return __sync_sub_and_fetch(ptr, 1);
	}
};
