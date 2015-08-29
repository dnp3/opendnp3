/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef ASIODNP3_DESTRUCTORHOOK_H
#define ASIODNP3_DESTRUCTORHOOK_H

#include <vector>
#include <functional>

namespace asiodnp3
{

/**
* Provides callback capabilities upon destruction. Useful for tying the lifecycle of some resource
* to the lifecycle of the inherited object
*/
class DestructorHook
{

	public:

	typedef std::function<void()> Action;
	
	DestructorHook();

	virtual ~DestructorHook();

	/**
	* Adds a destructor callback that get dispatched to the executor
	* @param action Callback that will be invoked on the executor
	*/
	

	template <class T>
	void DeleteOnDestruct(T* pPointer)
	{
		auto lambda = [pPointer]() { delete pPointer; };
		this->AddDestructorHook(lambda);
	}

	protected:

	void AddDestructorHook(const std::function<void()>& action);	

	private:
	std::vector<std::function<void()>> actions;
};

}

#endif

