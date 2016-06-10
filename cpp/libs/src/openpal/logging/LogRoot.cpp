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
#include "openpal/logging/LogRoot.h"

#include "openpal/logging/LogEntry.h"
#include "openpal/logging/LogMacros.h"
#include "openpal/logging/ILogHandler.h"
#include "openpal/logging/LogMacros.h"

#include "openpal/logging/StringFormatting.h"

namespace openpal
{

LogRoot::LogRoot(ILogHandler* handler, char const* alias, LogFilters filters) : LogRoot(handler, alias, filters, false)
{

}

LogRoot::LogRoot(ILogHandler* handler, char const* alias, LogFilters filters, bool reuseAlias) :
	logger(this),
	m_handler(handler),
	m_filters(filters),
	m_alias((reuseAlias ? alias : AllocateCopy(alias)))
{}

LogRoot::LogRoot(const LogRoot& copy, char const* alias) :
	logger(this),
	m_handler(copy.m_handler),
	m_filters(copy.m_filters),
	m_alias(AllocateCopy(alias))
{

}

LogRoot::LogRoot(LogRoot&& other) : LogRoot(other.m_handler, other.m_alias, other.m_filters, true)
{
	other.m_alias = nullptr;
	other.m_handler = nullptr;
	other.m_filters = 0;
}

LogRoot LogRoot::Clone(char const* alias) const
{
	return LogRoot(this->m_handler, alias, this->m_filters);
}

LogRoot LogRoot::Clone(char const* alias, LogFilters filters) const
{
	return LogRoot(this->m_handler, alias, filters);
}

LogRoot::~LogRoot()
{
	delete[] m_alias;
}

void LogRoot::Rename(char const* alias)
{
	delete[] m_alias;
	m_alias = AllocateCopy(alias);
}

const char* LogRoot::GetId() const
{
	return m_alias;
}

void LogRoot::Log(const LogFilters& filters, char const* location, char const* message, int errorCode)
{
	if (m_handler)
	{
		LogEntry le(m_alias, filters, location, message, errorCode);
		m_handler->Log(le);
	}
}

bool LogRoot::IsEnabled(const LogFilters& rhs) const
{
	return m_handler && (this->m_filters & rhs);
}

bool LogRoot::HasAny(const LogFilters& rhs) const
{
	return this->m_filters & rhs;
}

void LogRoot::SetFilters(const LogFilters& filters)
{
	m_filters = filters;
}

const LogFilters& LogRoot::GetFilters() const
{
	return m_filters;
}

}
