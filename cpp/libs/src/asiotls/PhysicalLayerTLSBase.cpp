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

#include "asiotls/PhysicalLayerTLSBase.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

using namespace asio;
using namespace openpal;
using namespace asio::ssl;

namespace asiotls
{

	PhysicalLayerTLSBase::PhysicalLayerTLSBase(
			openpal::LogRoot& root, 
			asio::io_service& service,			
			asio::ssl::context_base::method method) :

		PhysicalLayerASIO(root, service),
		ctx(method),
		stream(service, ctx)
	{
		
	}

	// ---- Implement the shared client/server actions ----

	void PhysicalLayerTLSBase::DoClose()
	{
		this->Shutdown();		
	}
	
	void PhysicalLayerTLSBase::DoRead(openpal::WSlice& dest)
	{
		uint8_t* pBuff = dest;

		auto callback = [this, pBuff](const std::error_code & ec, size_t  numRead)
		{
			this->OnReadCallback(ec, pBuff, static_cast<uint32_t>(numRead));
		};

		stream.async_read_some(buffer(pBuff, dest.Size()), executor.strand.wrap(callback));
	}
	
	void PhysicalLayerTLSBase::DoWrite(const openpal::RSlice& data)
	{
		auto callback = [this](const std::error_code & code, size_t  numWritten)
		{
			this->OnWriteCallback(code, static_cast<uint32_t>(numWritten));
		};

		async_write(stream, buffer(data, data.Size()), executor.strand.wrap(callback));
	}
	
	void PhysicalLayerTLSBase::DoOpenFailure()
	{
		SIMPLE_LOG_BLOCK(logger, logflags::DBG, "Failed socket open, closing socket");
		this->Shutdown();
	}	

	void PhysicalLayerTLSBase::Shutdown()
	{
		std::error_code ec;

		// TODO - async vs blocking?!
		stream.shutdown(ec);
		if (ec)
		{
			FORMAT_LOG_BLOCK(logger, logflags::WARN, "Error while shutting down TLS stream: %s", ec.message().c_str());
		}		
	}

}

