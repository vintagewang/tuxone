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
#include "Object.h"

namespace LWPR
{
	Object* Object::Duplicate(Object *ref)
	{
		if(ref)
		{
			ref->IncRef();
		}
		return ref;
	}

	void Object::Release(Object *ref)
	{
		if(ref)
		{
			ref->DecRef();
		}
	}

	Object::~Object()
	{
		// 如果没有如下判断，则对象可以在栈上定义
		//assert(m_nRefCount.Get() == 0);
	}

	void Object::IncRef()
	{
		assert(m_nRefCount >= 1);
		m_nRefCount++;
	}

	void Object::DecRef()
	{
		assert(m_nRefCount > 0);
		if(--m_nRefCount == 0)
		{
			delete this;
		}
	}
};
