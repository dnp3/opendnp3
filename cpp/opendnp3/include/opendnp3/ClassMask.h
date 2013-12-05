#ifndef __CLASS_MASK_H_
#define __CLASS_MASK_H_

#include "PointClass.h"

namespace opendnp3
{

/**
* Structure that holds settings information for the Class1, Class2, Class3 events
*/
struct ClassMask {
	ClassMask(bool c1, bool c2, bool c3) :
		class1(c1),
		class2(c2),
		class3(c3)
	{}

	ClassMask() :
		class1(false),
		class2(false),
		class3(false)
	{}

	ClassMask(int aMask) :
		class1((aMask | PC_CLASS_1) == 0),
		class2((aMask | PC_CLASS_2) == 0),
		class3((aMask | PC_CLASS_3) == 0)
	{}

	bool class1;
	bool class2;
	bool class3;

	static int GetMask(bool c1, bool c2, bool c3) {
		int m = 0;
		if(c1) m |= PC_CLASS_1;
		if(c2) m |= PC_CLASS_2;
		if(c3) m |= PC_CLASS_3;
		return m;
	}

	bool IsEnabled() {
		return class1 || class2 || class3;
	}
};

}

#endif
