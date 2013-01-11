
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __LINE_READER_H_
#define __LINE_READER_H_

#include <APL/Uncopyable.h>
#include <APL/PhysicalLayerMonitor.h>
#include <APL/CopyableBuffer.h>
#include <string>

namespace apl
{

class IPhysicalLayerAsync;
class IExecutor;

/** Abstracts the process of reading line from a physical layer.
*/
class LineReader : public PhysicalLayerMonitor, private Uncopyable
{
public:
	LineReader(Logger* apLogger, IPhysicalLayerAsync* apPhysical, IExecutor* apExecutor, size_t aBuffSize);

	virtual void AcceptLine(const std::string&) = 0;
	virtual void _Up() = 0;
	virtual void _Down() = 0;

private:
	CopyableBuffer mBuffer;
	size_t mNumBytes;
	bool mHasCR;

	void Read();
	void OnPhysicalLayerOpenSuccessCallback();
	void OnPhysicalLayerOpenFailureCallback() {}
	void OnPhysicalLayerCloseCallback();
	void Reset();

	void _OnReceive(const uint8_t*, size_t aNum);
	void ReadBuffer();
};


}

#endif
