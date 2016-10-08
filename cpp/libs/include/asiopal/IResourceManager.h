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
#ifndef ASIOPAL_IRESOURCEMANAGER_H
#define ASIOPAL_IRESOURCEMANAGER_H

#include <memory>
#include <functional>

namespace asiopal
{

typedef std::function<void()> shutdown_action_t;

/**
*	Anything that can be shutdown
*/
class IResource
{
public:

	virtual ~IResource() {}

	virtual void BeginShutdown() = 0;

	void SetShutdownAction(shutdown_action_t action)
	{
		this->on_shutdown = action;
	}

protected:

	void OnShutdown()
	{
		if (on_shutdown)
		{
			on_shutdown();
			on_shutdown = nullptr;
		}
	}

private:

	shutdown_action_t on_shutdown;
};

/**
*	Tracks otherwise orphaned resources running on a thread pool
*/
class IResourceManager
{

public:

	/// Attach a resource to the manager
	/// Returns false if the manager is shutting down, and starts the shutdown of the resource immediately
	virtual bool Attach(std::shared_ptr<IResource> resource) = 0;

	/// Detach the resource from the manager
	virtual void Detach(std::shared_ptr<IResource> resource) = 0;

};

}

#endif
