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
#ifndef LWPR_OBJECT_H__
#define LWPR_OBJECT_H__
#include "LWPRType.h"
#include "AtomicInteger.h"

namespace LWPR
{
	class Object
	{
	public:
		/**
		 * 对象引用个数加1
		 */
		static Object* Duplicate(Object *ref);
		/**
		 * 对象引用个数减1，当减为0时，释放对象
		 */
		static void Release(Object *ref);

		void IncRef();
		void DecRef();

	protected:
		Object() : m_nRefCount(1) {}
		virtual ~Object();

	private:
		AtomicInteger m_nRefCount;
	};
};

#endif // end of LWPR_OBJECT_H__
