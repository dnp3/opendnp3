#ifndef __BUFFER_TYPES_H_
#define __BUFFER_TYPES_H_

#include <opendnp3/DataTypes.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * An enumeration that lists the various types of buffers that a
 * SlaveEventBuffer implements.  Note that each has a corresponding
 * data type in the enumeration DataTypes, with the exception of
 * BT_VTO.
 */
enum BufferTypes {
	BT_BINARY,
	BT_ANALOG,
	BT_COUNTER,
	BT_VTO,
	BT_INVALID		/* used for unit testing only, keep as last */
};

/**
 * Maps a DataTypes enumeration value to a BufferTypes enumeration
 * value.
 *
 * @param aType			the DataTypes enumeration value
 *
 * @return				the corresponding BufferTypes enumeration
 * 						value
 *
 * @throw ArgumentException		if the aType value is invalid
 */
BufferTypes DLL_LOCAL Convert(DataTypes aType);

}

/* vim: set ts=4 sw=4: */

#endif
