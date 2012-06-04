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
#ifndef LWPR_ATOMIC_INTEGER_H__
#define LWPR_ATOMIC_INTEGER_H__
#include "LWPRType.h"

namespace LWPR
{
	class AtomicIntegerImpl;

	class AtomicInteger
	{
	public:

		/**
		 * 构造函数
		 */
		AtomicInteger(int initialValue = 0);

		/**
		 * 析构函数
		 */
		~AtomicInteger();

		/**
		 * 获取当前值
		 */
		int Get() const;

		/**
		 * 设置为给定值
		 */
		void Set(int newValue);

		/**
		 * 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值
		 * @return 如果成功，则返回 true。返回 False 指示实际值与预期值不相等
		 */
		bool CompareAndSet(int expect, int update);

		/**
		 * 以原子方式将当前值加 1
		 * @return 以前的值
		 */
		int GetAndIncrement();

		/**
		 * 以原子方式将当前值减 1
		 * @return 以前的值
		 */
		int GetAndDecrement();

		/**
		 * 以原子方式将给定值与当前值相加
		 * @return 以前的值
		 */
		int GetAndAdd(int delta);

		/**
		 * 以原子方式将给定值与当前值相加
		 * @param delta 要加上的值
		 * @return 更新的值
		 */
		int AddAndGet(int delta);

		/**
		 * 以原子方式将当前值加 1
		 * @return 更新的值
		 */
		int IncrementAndGet();

		/**
		 * 以原子方式将当前值减 1
		 * @return 更新的值
		 */
		int DecrementAndGet();

		/**
		 * i = 2
		 */
		int operator=(int newValue);

		/**
		 * ++i
		 */
		int operator++();

		/**
		 * i++
		 */
		int operator++(int);

		/**
		 * --i
		 */
		int operator--();

		/**
		 * i--
		 */
		int operator--(int);

		/**
		 * 类型转化操作符
		 */
		operator int () const;

		//////////////////////////////////////////////////////////////////////////

		/**
		 * 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值
		 * @return 如果成功，则返回 true。返回 False 指示实际值与预期值不相等
		 */
		static bool CompareAndSet(volatile int *ptr, int expect, int update);

		/**
		 * 以原子方式将当前值加 1
		 * @return 以前的值
		 */
		static int GetAndIncrement(volatile int *ptr);

		/**
		 * 以原子方式将当前值减 1
		 * @return 以前的值
		 */
		static int GetAndDecrement(volatile int *ptr);

		/**
		 * 以原子方式将给定值与当前值相加
		 * @return 以前的值
		 */
		static int GetAndAdd(volatile int *ptr, int delta);

		/**
		 * 以原子方式将给定值与当前值相加
		 * @param delta 要加上的值
		 * @return 更新的值
		 */
		static int AddAndGet(volatile int *ptr, int delta);

		/**
		 * 以原子方式将当前值加 1
		 * @return 更新的值
		 */
		static int IncrementAndGet(volatile int *ptr);

		/**
		 * 以原子方式将当前值减 1
		 * @return 更新的值
		 */
		static int DecrementAndGet(volatile int *ptr);

	private:
		volatile int m_nValue;
	};
};

#endif // end of LWPR_ATOMIC_INTEGER_H__
