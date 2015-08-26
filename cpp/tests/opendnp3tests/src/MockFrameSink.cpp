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
#include "MockFrameSink.h"

using namespace openpal;

namespace opendnp3
{

MockFrameSink::MockFrameSink() : m_num_frames(0), mLowerOnline(false)
{}

bool MockFrameSink::OnLowerLayerUp()
{
	mLowerOnline = true;
	return true;
}

bool MockFrameSink::OnLowerLayerDown()
{
	mLowerOnline = false;
	return true;
}

void MockFrameSink::Reset()
{
	this->ClearBuffer();
	m_num_frames = 0;
}

bool MockFrameSink::CheckLast(LinkFunction aCode, bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	return (m_code == aCode) && (aIsMaster == m_master) && (m_src == aSrc) && (m_dest == aDest);
}

bool MockFrameSink::CheckLastWithFCB(LinkFunction aCode, bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	return (m_fcb == aFcb) && CheckLast(aCode, aIsMaster, aDest, aSrc);
}

bool MockFrameSink::CheckLastWithDFC(LinkFunction aCode, bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	return  (m_fcvdfc == aIsRcvBuffFull) && CheckLast(aCode, aIsMaster, aDest, aSrc);
}

bool MockFrameSink::OnTransmitResult(bool success)
{
	return true;
}

bool MockFrameSink::OnFrame(LinkFunction func, bool isMaster, bool fcb, bool fcvdfc, uint16_t dest, uint16_t source, const openpal::RSlice& userdata)
{
	++m_num_frames;

	this->m_code = func;
	this->m_master = isMaster;
	this->m_fcb = fcb;
	this->m_fcvdfc = fcvdfc;
	this->m_dest = dest;
	this->m_src = source;
	
	if (userdata.IsNotEmpty())
	{		
		this->WriteToBuffer(userdata);
	}

	return true;
}

void MockFrameSink::AddAction(std::function<void ()> fun)
{
	m_actions.push_back(fun);
}

void MockFrameSink::ExecuteAction()
{
	if(m_actions.size() > 0)
	{
		auto f = m_actions.front();
		m_actions.pop_front();
		f();
	}
}

}

