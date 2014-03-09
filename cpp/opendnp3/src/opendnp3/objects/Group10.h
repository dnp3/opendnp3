//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
//
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
//
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef __OPENDNP3_GENERATED_GROUP10_H_
#define __OPENDNP3_GENERATED_GROUP10_H_

#include <openpal/BufferWrapper.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/IDNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3
{

struct Group10Var0
{
	static const GroupVariationID ID;
};

struct Group10Var1
{
	static const GroupVariationID ID;
};

struct Group10Var2
{
	static const GroupVariationID ID;
	typedef BinaryOutputStatus Target;
	static const uint32_t SIZE = 1;
	static Group10Var2 Read(openpal::ReadOnlyBuffer&);
	static void Write(const Group10Var2&, openpal::WriteBuffer&);

	uint8_t flags;
};

struct Group10Var2Serializer : public IDNP3Serializer<BinaryOutputStatus>
{

	static IDNP3Serializer<BinaryOutputStatus>* Inst()
	{
		return &mInstance;
	}

	GroupVariationID ID() const
	{
		return Group10Var2::ID;
	}

	uint32_t Size() const
	{
		return Group10Var2::SIZE;
	}

	typedef BinaryOutputStatus Target;
	BinaryOutputStatus Read(openpal::ReadOnlyBuffer&) const;
	void Write(const BinaryOutputStatus&, openpal::WriteBuffer&) const;

private:

	static Group10Var2Serializer mInstance;
};


}

#endif
