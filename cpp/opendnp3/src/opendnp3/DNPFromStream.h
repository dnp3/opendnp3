#ifndef __DNP_FROM_STREAM_H_
#define __DNP_FROM_STREAM_H_

#include <openpal/Visibility.h>

#ifdef WIN32
//disable the type converstion warnings
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#endif

namespace opendnp3
{

/** Templates for read dnp3 data types from stream.
	Used in the dnp3 object definitions to define how
	they deserialize themeselves.
*/
class DLL_LOCAL DNPFromStream
{
public:

	//templated conversion functions
	template <typename T>
	static typename T::DataType ReadQ(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQT(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadBinaryQV(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadBinaryQVT(const uint8_t* apPos, const T* apObj);

	template <typename T>
	static typename T::DataType ReadQVT(const uint8_t* apPos, const T* apObj);
};

template <typename T>
inline typename T::DataType DNPFromStream::ReadQ(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadBinaryQV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQualityValue(apObj->mFlag.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadBinaryQVT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQualityValue(apObj->mFlag.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetValue(apObj->mValue.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQV(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetValue(apObj->mValue.Get(apPos));
	return ret;
}

template <typename T>
inline typename T::DataType DNPFromStream::ReadQVT(const uint8_t* apPos, const T* apObj)
{
	typename T::DataType ret;
	ret.SetQuality(apObj->mFlag.Get(apPos));
	ret.SetValue(apObj->mValue.Get(apPos));
	ret.SetTime(apObj->mTime.Get(apPos));
	return ret;
}

}

#ifdef WIN32
//disable thes type converstion warnings
#pragma warning ( pop )
#endif

#endif

