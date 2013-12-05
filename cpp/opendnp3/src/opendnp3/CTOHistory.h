#ifndef __CTO_HISTORY_H_
#define __CTO_HISTORY_H_

#include <openpal/Visibility.h>

namespace opendnp3
{

class DLL_LOCAL CTOHistory
{
public:
	CTOHistory() :
		mCTO(-1),
		mHeaderNum(0),
		mTimeHeaderNum(0) {

	}

	void Reset() {
		mCTO = -1;
		mHeaderNum = 0;
		mTimeHeaderNum = 0;
	}

	void NextHeader() {
		++mHeaderNum;
	}

	void SetCTO(int64_t aTime) {
		mCTO = aTime;
		mTimeHeaderNum = mHeaderNum;
	}

	bool GetCTO(int64_t& arTime) {
		if(mCTO >= 0 && mTimeHeaderNum == (mHeaderNum - 1)) {
			arTime = mCTO;
			return true;
		}
		else return false;
	}

private:

	int64_t mCTO;
	size_t mHeaderNum;
	size_t mTimeHeaderNum;

};

}

#endif
