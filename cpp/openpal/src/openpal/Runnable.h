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
#ifndef __RUNNABLE_H_
#define __RUNNABLE_H_

#include "Erasure.h"

#ifndef OPENPAL_MACRO_MAX_RUNNABLE_SIZE
#define OPENPAL_MACRO_MAX_RUNNABLE_SIZE 128  // TODO consider reducing this size for x86 using macros
#endif

namespace openpal
{

// TODO define max somewhere else
const uint32_t MAX_RUNNABLE_SIZE = OPENPAL_MACRO_MAX_RUNNABLE_SIZE;

class Runnable : public Erasure<MAX_RUNNABLE_SIZE>
{
public:

	Runnable();

	Runnable& operator=(const Runnable& other);

	void Run() const;

protected:

	Runnable(Invoke pInvoke_, uint32_t size_);

};

}

#endif
