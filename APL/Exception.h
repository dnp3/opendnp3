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
#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_

#include <exception>
#include <string>
#include <sstream>

namespace apl
{

// @section desc Extends the std::exception interface
class Exception : public std::exception
{
public:

	Exception(const std::string& aSource, const std::string& aMessage, int aErrorCode = -1, int aSubCode = -1) throw();
	Exception(const std::string& aSource, int aErrorCode = -1, int aSubCode = -1) throw();
	virtual ~Exception() throw();

	const std::string& Source() const;
	const std::string& Message() const;

	virtual const char* what() const throw();

	int ErrorCode() const {
		return this->mErrorCode;
	}
	int SubCode() const {
		return this->mSubCode;
	}

	std::string GetErrorString() const; //returns a concatentated description of the exception

protected:

	//layer specific error code, defaults to -1;
	int mErrorCode;
	int mSubCode;

	std::string mSource;	// The funciton or module that produced the exception
	std::string mMessage;	// A textual description of the exception
	std::string mWhat;

private:

	void SetWhat();
};

class ObjectException : public Exception
{
public:
	ObjectException(const std::string& aSource, const std::string& aMessage) throw() :
		Exception(aSource, aMessage)
	{}
};

class ReadTimeoutException : public Exception
{
public:
	ReadTimeoutException(const std::string& aSource, const std::string& aMessage) throw() :
		Exception(aSource, aMessage)
	{}
};

class InvalidStateException : public Exception
{
public:
	InvalidStateException(const std::string& aSource, const std::string& aState) throw() :
		Exception(aSource, "Invalid operation for state: " + aState)
	{}
};

class ArgumentException : public Exception
{
public:
	ArgumentException(const std::string& aSource, const std::string& aMessage = "") throw() :
		Exception(aSource, aMessage)
	{}
};

class NotImplementedException : public Exception
{
public:
	NotImplementedException(const std::string& aSource) throw() :
		Exception(aSource, "Function not implemented.")
	{}
};

class IndexOutOfBoundsException : public Exception
{
public:
	IndexOutOfBoundsException(const std::string& aSource) throw() :
		Exception(aSource, "Index out of bounds.")
	{}
};

}

#endif
