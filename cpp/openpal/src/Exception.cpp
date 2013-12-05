#include <openpal/Exception.h>

#include <sstream>

using namespace std;

namespace openpal
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

std::string IndexOutOfBoundsException::GetMessage(int aIndex)
{
	ostringstream oss;
	oss << "Index out of bounds: " << aIndex;
	return oss.str();
}

}

