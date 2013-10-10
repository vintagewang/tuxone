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
#ifndef LWPR_THREAD_LOCAL_H__
#define LWPR_THREAD_LOCAL_H__
#include "LWPRType.h"
#include "Thread.h"

namespace LWPR
{
	class TLSValue
	{
	public:
		virtual ~TLSValue() {};
	};

	class ThreadLocal
	{
	public:
		ThreadLocal();
		virtual ~ThreadLocal();

		TLSValue* GetValue();
		void SetValue(TLSValue* value);

	private:
		static void KeyDestructor(void* value);
		THREAD_KEY_T	m_Key;
	};
};

#endif // end of LWPR_THREAD_LOCAL_H__
