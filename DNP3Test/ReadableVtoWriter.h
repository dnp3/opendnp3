/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef __READABLE_VTO_WRITER_H_
#define __READABLE_VTO_WRITER_H_

#include <DNP3/VtoWriter.h>
#include <DNP3/IVtoEventAcceptor.h>

namespace apl
{
namespace dnp
{

/**
 *  Provides a simple read function that makes testing easier
 */
class ReadableVtoWriter : public VtoWriter, private IVtoEventAcceptor
{
public:
	ReadableVtoWriter(Logger* apLogger, size_t aMaxVtoChunks) : VtoWriter(apLogger, aMaxVtoChunks), mpEvent(NULL)
	{}

	bool Read(VtoEvent& arEvent) {
		mpEvent = &arEvent;
		size_t num = this->Flush(this, 1);
		mpEvent = NULL;
		return num > 0;
	}

private:

	void Update(const VtoData& arEvent, PointClass aClass, size_t aIndex) {
		assert(mpEvent != NULL);
		VtoEvent evt(arEvent, aClass, aIndex);
		*mpEvent = evt;
	}

	VtoEvent* mpEvent;

};

}
}

/* vim: set ts=4 sw=4: */

#endif

