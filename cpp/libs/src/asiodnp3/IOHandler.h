/*
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
#ifndef ASIODNP3_IOHANDLER_H
#define ASIODNP3_IOHANDLER_H

#include "opendnp3/link/ILinkTx.h"

#include "asiodnp3/IChannelListener.h"

#include "asiopal/IO.h"
#include "asiopal/IAsyncChannel.h"

#include <memory>

namespace asiodnp3
{

/**

Manages I/O for a number of link contexts

*/	
class IOHandler : public opendnp3::ILinkTx
{

public:

	IOHandler(
		std::shared_ptr<asiopal::IO> io,
		std::shared_ptr<IChannelListener> listener
	);

	/// --- implement ILinkTx ---
	
	virtual void BeginTransmit(const openpal::RSlice& data, opendnp3::ILinkSession& context) override;

protected:

private:

	std::shared_ptr<asiopal::IO> io;
	std::shared_ptr<IChannelListener> listener;
	
	// current value of the channel, may be null
	std::unique_ptr<asiopal::IAsyncChannel> channel;
};

}

#endif

