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
#ifndef LWPR_ATOMIC_BOOLEAN_H__
#define LWPR_ATOMIC_BOOLEAN_H__
#include "LWPRType.h"

namespace LWPR
{
	class AtomicBoolean
	{
	public:
		/**
		 * 构造函数
		 */
		AtomicBoolean(bool initialValue = false);

		/**
		 * 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值
		 * @return 如果成功，则返回 true。返回 False 指示实际值与预期值不相等
		 */
		bool CompareAndSet(bool expect, bool update);

		/**
		 * 返回当前值
		 */
		bool Get() const;

		/**
		 * 设置为给定值，并返回以前的值
		 * @return 以前的值
		 */
		bool GetAndSet(bool newValue);

		/**
		 * 无条件地设置为给定值。
		 */
		void Set(bool newValue);

		/**
		 * 类型转化操作符
		 */
		operator bool () const;

	private:
		volatile int m_nValue;
	};
};

#endif // end of LWPR_ATOMIC_BOOLEAN_H__
