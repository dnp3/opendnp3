//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __DATA_INTERFACES_H_
#define	__DATA_INTERFACES_H_

#include "DataTypes.h"
#include "ITransactable.h"


namespace apl
{
/**
   A DataObserver is the key interface between a communication stack and
   the "application" code. The application is responsible for measuring or
   calculating data and then pushing it into this interface when it is ready
   to publish it to the communication stack. That data needs to be strongly typed
   and passed by index. As with all ITransactables it should be used with the
   exception safe Transaction object.
*/
class IDataObserver : public ITransactable
{
public:

	virtual ~IDataObserver() {}

	// NVII enforces a policy of using these functions only after
	// a transaction has been initiated
	void Update(const Binary&, size_t aIndex);			//!< push a change to the owner of the database, must have transaction started
	void Update(const Analog&, size_t aIndex);			//!< push a change to the owner of the database, must have transaction started
	void Update(const Counter&, size_t aIndex);			//!< push a change to the owner of the database, must have transaction started
	void Update(const ControlStatus&, size_t aIndex);	//!< push a change to the owner of the database, must have transaction started
	void Update(const SetpointStatus&, size_t aIndex);	//!< push a change to the owner of the database, must have transaction started

protected:

	//concrete class will implement these
	virtual void _Update(const Binary& arPoint, size_t) = 0;
	virtual void _Update(const Analog& arPoint, size_t) = 0;
	virtual void _Update(const Counter& arPoint, size_t) = 0;
	virtual void _Update(const ControlStatus& arPoint, size_t) = 0;
	virtual void _Update(const SetpointStatus& arPoint, size_t) = 0;


};

//Inline the simple public interface functions
inline void IDataObserver::Update(const Binary& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const Analog& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const Counter& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const ControlStatus& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}
inline void IDataObserver::Update(const SetpointStatus& arPoint, size_t aIndex)
{
	assert(this->InProgress());
	this->_Update(arPoint, aIndex);
}


}

#endif
