
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
#ifndef __VTO_WRITER_TO_BUFFER_TASK_H_
#define __VTO_WRITER_TO_BUFFER_TASK_H_

#include "EventBuffers.h"
#include "MasterTaskBase.h"

namespace opendnp3
{

/**
 * A task to pull data off the VtoWriter queue and transmit it as an
 * APDU.  The actual work is done by Master::TransmitVtoData().
 */
class VtoTransmitTask : public MasterTaskBase
{
public:

	/**
	 * Creates a new VtoTransmitTask instance.  The internal buffer
	 * size is set to be an order of magnitude larger than a single
	 * fragment size, to prevent back-ups in the user application.
	 *
	 * @param log		the Logger that the task should use for
	 * 					message reporting
	 * @param fragSize	the size of one fragment
	 *
	 * @return			a new VtoTransmitTask instance
	 */
	VtoTransmitTask(Logger* log, size_t fragSize, bool aUseNonStandardCode) :
		MasterTaskBase(log),
		mBuffer(fragSize * 10),
		mUseNonStandardCode(aUseNonStandardCode)
	{}

	/**
	 * A default destructor for the VtoTransmitTask.
	 */
	virtual ~VtoTransmitTask() {}

	/**
	 * Configures the APDU provided for use with DNP3 Virtual
	 * Terminal Objects.  Data is pulled out of the
	 * InsertionOrderedEventBuffer<VtoEvent> instance and is
	 * placed into the APDU.
	 *
	 * @param arAPDU	the DNP3 message container that will
	 * 					contain the DNP3 Virtual Terminal Objects
	 */
	void ConfigureRequest(APDU& arAPDU);

	/**
	 * Returns the name of the task, as a string.
	 *
	 * @return			the name of the task
	 */
	std::string Name() const {
		return "VtoTransmitTask";
	}

	/**
	 * The transmission buffer for Virtual Terminal objects.  Each
	 * object in the buffer is assumed to contain 255-octets of
	 * data, so VtoWriter needs to do a good job of packing things
	 * to maintain efficiency.  Also, Master::TransmitVtoData()
	 * should take care to only write up to the size of the
	 * mBuffer.
	 */
	InsertionOrderedEventBuffer<VtoEvent> mBuffer;

	/**
	 * On failure of the Link Layer to deliver the DNP3 message
	 * created in ConfigureRequest(), put the data that was
	 * previously sent back onto the event queue.  This results in
	 * no data loss.
	 */
	void OnFailure();

protected:

	/** FC_WRITE can't be retried so, another code is needed to
	* make a reliable stream in the MASTER -> SLAVE direction
	*/
	bool mUseNonStandardCode;


	/**
	 * No specific subclass behavior for a partial response.
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			TR_CONTINUE as a constant response
	 */
	TaskResult _OnPartialResponse(const APDU& arAPDU);

	/**
	 * Acknowledge that the data previously sent with
	 * ConfigureRequest() was received successfully by the other
	 * station, and remove the data from the mBuffer queue.
	 *
	 * @param arAPDU	the DNP3 message as an APDU instance
	 *
	 * @return			TR_SUCCESS as a constant
	 */
	TaskResult _OnFinalResponse(const APDU& arAPDU);
};

}

/* vim: set ts=4 sw=4: */

#endif

