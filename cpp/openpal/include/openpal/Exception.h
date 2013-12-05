#ifndef __EXCEPTION_H_
#define __EXCEPTION_H_

#include <exception>
#include <string>
#include <sstream>

namespace openpal
{

/// @section desc Extends the std::exception interface
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

	static std::string GetMessage(int aIndex);

public:
	IndexOutOfBoundsException(const std::string& aSource, int aIndex) throw() :
		Exception(aSource, GetMessage(aIndex))
	{}
};

class ParameterException : public Exception
{
public:
	ParameterException(const std::string& aSource, const std::string& aMessage, int aErrorCode = -1) throw() :
		Exception(aSource, aMessage, aErrorCode) {}
};

class NotSupportedException : public Exception
{
public:
	NotSupportedException(const std::string& aSource, const std::string& aMessage, int aErrorCode = -1) throw() :
		Exception(aSource, aMessage, aErrorCode) {}
};

}

#endif
