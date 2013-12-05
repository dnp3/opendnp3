#ifndef __BUFFER_HELPERS_H_
#define __BUFFER_HELPERS_H_

#include <opendnp3/CopyableBuffer.h>
#include <string>

namespace opendnp3
{


class ByteStr : public CopyableBuffer
{

public:
	ByteStr(size_t aLength);
	ByteStr(size_t aLength, uint8_t aSeed);
	ByteStr(const uint8_t* apData, size_t aLength);
	ByteStr(const std::string& aChars);
	bool operator==(const ByteStr& arRHS) const;
};

/**
 * A sequence of hex values in the form "01 02 03 04" that
 * are stored as a ByteStr.
 *
 */
class HexSequence : public ByteStr
{
public:
	HexSequence(const std::string& aSequence);

	operator openpal::ReadOnlyBuffer() { return this->ToReadOnly(); }

private:
	std::string RemoveSpaces(const std::string& aSequence);
	void RemoveSpacesInPlace(std::string& aSequence);
	static size_t Validate(const std::string& aSequence);
};


}

#endif
