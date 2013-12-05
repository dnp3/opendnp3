#ifndef __MOCK_APP_LAYER_H_
#define __MOCK_APP_LAYER_H_

#include <opendnp3/AppInterfaces.h>
#include <opendnp3/APDUConstants.h>
#include <opendnp3/APDU.h>

#include <openpal/Loggable.h>

#include <queue>

namespace opendnp3
{

/**	@section desc Test class to mock async app layer for master/slave */
class MockAppLayer : public IAppLayer, public openpal::Loggable
{
public:
	MockAppLayer(openpal::Logger);
	virtual ~MockAppLayer() {}


	void SetUser(IAppUser*);

	void SendResponse(APDU&);
	void SendUnsolicited(APDU&);
	void SendRequest(APDU&);
	void CancelResponse();

	bool NothingToRead();

	size_t mNumCancel;

	void EnableAutoSendCallback(bool aIsSuccess);
	void DisableAutoSendCallback();

	APDU Read();
	size_t Count() {
		return mFragments.size();
	}
	FunctionCode ReadFunction();
	size_t NumAPDU() {
		return mFragments.size();
	}

private:

	void DoSendUnsol();
	void DoSendSol();

	IAppUser* mpUser;
	bool mAutoSendCallback;
	bool mIsSuccess;
	std::deque<APDU> mFragments;
};

}

#endif

