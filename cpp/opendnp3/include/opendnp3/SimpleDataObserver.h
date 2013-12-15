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
#ifndef __SIMPLE_DATA_OBSERVER_H_
#define	__SIMPLE_DATA_OBSERVER_H_

#include "IDataObserver.h"
#include "Singleton.h"

#include <iostream>
#include <string>
#include <functional>

namespace opendnp3
{

/**
* Mock data observer that sends string representation of measurements to a functor
*/
class SimpleDataObserver : public IDataObserver
{
public:
	/**
	* Primary constructor
	* @param aOutputFunc functor that will receive messages
	*/
	SimpleDataObserver(std::function<void (const std::string&)> aOutputFunc);

	void Update(const Binary& arPoint, uint32_t) final;
	void Update(const Analog& arPoint, uint32_t) final;
	void Update(const Counter& arPoint, uint32_t) final;
	void Update(const ControlStatus& arPoint, uint32_t) final;
	void Update(const SetpointStatus& arPoint, uint32_t) final;

protected:

	//concrete class will implement these
	void Start() final;

	void End() final;

private:
	std::function<void (std::string)> mOutputFunc;

};

/// Singleton IDataObserver that ignores all messages
class NullDataObserver : public SimpleDataObserver
{
private:
	static NullDataObserver mInstance;
protected:
	NullDataObserver() : SimpleDataObserver([](const std::string&) {})
	{}
public:
	static NullDataObserver* Inst() {
		return &mInstance;
	}
};

/// Singleton IDataObserver that prints all measurements to the console
class PrintingDataObserver : public SimpleDataObserver
{
private:
	static PrintingDataObserver mInstance;
protected:
	PrintingDataObserver() : SimpleDataObserver([](const std::string& arStr) {
		std::cout << arStr << std::endl;
	})
	{}
public:
	static PrintingDataObserver* Inst() {
		return &mInstance;
	}
};

}

#endif
