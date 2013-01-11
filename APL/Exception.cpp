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
#include "Exception.h"

#include <sstream>

using namespace std;

namespace apl
{

Exception::Exception(const string& aSource, const string& aMessage, int aErrorCode, int aSubCode) throw() :
	mErrorCode(aErrorCode), mSubCode(aSubCode), mSource(aSource), mMessage(aMessage)
{
	this->SetWhat();
}

Exception::Exception(const std::string& aSource, int aErrorCode, int aSubCode) throw() :
	mErrorCode(aErrorCode), mSubCode(aSubCode), mSource(aSource), mMessage("")
{
	this->SetWhat();
}

void Exception::SetWhat()
{
	ostringstream oss;
	oss << mSource << ": " << mMessage << ", " << mErrorCode;
	mWhat = oss.str();
}

Exception::~Exception() throw()
{

}

const std::string& Exception :: Source() const
{
	return this->mSource;
}

const std::string& Exception :: Message() const
{
	return this->mMessage;
}

string Exception::GetErrorString() const
{
	return mWhat;
}

const char* Exception :: what() const throw()
{
	return mWhat.c_str();
}

}

