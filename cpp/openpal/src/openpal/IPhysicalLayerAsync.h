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
#ifndef _I_PHYSICAL_LAYER_ASYNC_H_
#define _I_PHYSICAL_LAYER_ASYNC_H_

#include "AsyncLayerInterfaces.h"

#include <openpal/BufferWrapper.h>

namespace openpal
{

class IExecutor;
class IHandlerAsync;

class IChannelState
{

public:

	virtual bool CanOpen() const = 0;

	virtual bool CanClose() const = 0;

	virtual bool CanRead() const = 0;

	virtual bool CanWrite() const = 0;

	/** @return True if the layer is performing an async read */
	virtual bool IsReading() const = 0;

	/** @return True if the layer is performing an async write */
	virtual bool IsWriting() const = 0;

	/** @return True if the layer is performing an asynchronously closing */
	virtual bool IsClosing() const = 0;

	/** @return True if the layer is in the closed with no other activity */
	virtual bool IsClosed() const = 0;

	/** @return True if the layer is performing an asynchronously opening */
	virtual bool IsOpening() const = 0;

	/** @return True if the layer is open for read/write */
	virtual bool IsOpen() const = 0;

	/** @return A string representing the state of the physical layer */
	virtual std::string ConvertStateToString() const = 0;
};


/**
 * Defines an asynchronous interface for serial/tcp/?
 */
class IPhysicalLayerAsync : public IChannelState
{
public:

	virtual ~IPhysicalLayerAsync() {}

	/**
	* @return the executor associated with this layer
	*/
	virtual openpal::IExecutor* GetExecutor() = 0;

	/**
	 * Starts an open operation.
	 *
	 * Callback is either IHandlerAsync::OnLowerLayerUp or
	 * IHandlerAsync::OnOpenFailure.
	 */
	virtual void AsyncOpen() = 0;

	/**
	 * Starts a close operation.
	 *
	 * Callback is IHandlerAsync::OnLowerLayerDown.  Callback occurs
	 * after all Asynchronous operations have occured.  If the user
	 * code has an outstanding read or write, those handlers will not
	 * be called.
	 */
	virtual void AsyncClose() = 0;

	/**
	 * Starts a send operation.
	 *
	 * Callback is IHandlerAsync::OnSendSuccess or a failure will
	 * result in the layer closing.
	 *
	 * @param arBuffer		The buffer from which the write operation
	 * 						will write data. The underlying buffer must
	 *						remain available until the write callback or
	 *                      close occurs.
	 */
	virtual void AsyncWrite(const ReadOnlyBuffer& arBuffer) = 0;

	/**
	 * Starts a read operation.
	 *
	 * Use SetHandler to provide a callback that is called by
	 * OnReceive(const ReadOnlyBuffer&) or a failure will
	 * result in the layer closing.
	 *
	 * @param arBuffer		Read into the underlying buffer
	 *                      defined by the wrapper.  The underlying buffer
	 *                      must remain available until the read callback
	 */
	virtual void AsyncRead(WriteBuffer& arBuffer) = 0;

	/**
	 * Set the handler interface for callbacks. A read interface has
	 * been added so the max size is no longer required.
	 *
	 * @param apHandler		Class that will process asynchronous
	 * 						callbacks
	 */
	virtual void SetHandler(IHandlerAsync* apHandler) = 0;
};

};



#endif
