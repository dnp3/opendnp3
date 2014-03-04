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
#ifndef __SUBJECT_BASE_H_
#define __SUBJECT_BASE_H_

#include <vector>
#include <mutex>
#include <functional>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

/**
* Helper class designed to be used with private inheritance. Allows observers to be notified when some
* state changes.
*/
class SubjectBase
{

public:
	SubjectBase();
	virtual ~SubjectBase() {}

	// implement the ISubject interface
	void AddObserver(std::function<void ()> aCallback);

	void AddObserver(openpal::IExecutor* apExecutor, std::function<void ()> aCallback);

protected:

	void NotifyObservers();

private:
	std::mutex mSubjectMutex;
	std::vector< std::function<void ()> > mObservers;
};

}

#endif

