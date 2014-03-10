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

#include "ProxyDataObserver.h"

#include <openpal/IExecutor.h>

#include <assert.h>

namespace opendnp3
{

ProxyDataObserver::ProxyDataObserver(IDataObserver* pProxy_, openpal::IExecutor* pExecutor_) :
	pProxy(pProxy_),
	pExecutor(pExecutor_)
{}

void ProxyDataObserver::Start()
{
	pExecutor->Post([this]()
	{
		openpal::Transaction::Start(pProxy);
	});
}

void ProxyDataObserver::End()
{
	pExecutor->Post([this]()
	{
		openpal::Transaction::End(pProxy);
	});
}

void ProxyDataObserver::Update(const Binary& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

void ProxyDataObserver::Update(const Analog& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

void ProxyDataObserver::Update(const Counter& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

void ProxyDataObserver::Update(const FrozenCounter& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

void ProxyDataObserver::Update(const BinaryOutputStatus& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

void ProxyDataObserver::Update(const AnalogOutputStatus& meas, uint16_t index)
{
	pExecutor->Post([ = ]()
	{
		pProxy->Update(meas, index);
	});
}

}


