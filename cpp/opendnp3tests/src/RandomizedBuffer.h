#ifndef __RANDOMIZED_BUFFER_H_
#define __RANDOMIZED_BUFFER_H_

#include <opendnp3/CopyableBuffer.h>
#include "Random.h"

namespace opendnp3
{

class RandomizedBuffer : public CopyableBuffer
{

public:

	RandomizedBuffer(size_t aSize);

	void Randomize();

private:
	Random<uint8_t> rand;
};

}

#endif


