#ifndef __APP_LAYER_CHANNEL_H_
#define __APP_LAYER_CHANNEL_H_

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>
#include <openpal/TimeDuration.h>

namespace openpal
{
class IExecutor;
class ITimer;
}

namespace opendnp3
{

class AppLayer;
class ACS_Base;
class APDU;
struct AppControlField;

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class DLL_LOCAL AppLayerChannel : public openpal::Loggable
{
	friend class ACS_Base;
	friend class ACS_Idle;
	friend class ACS_Send;
	friend class ACS_SendBase;
	friend class ACS_SendConfirmed;
	friend class ACS_SendExpectResponse;
	friend class ACS_SendCanceled;
	friend class ACS_WaitForConfirm;
	friend class ACS_WaitForResponseBase;
	friend class ACS_WaitForFirstResponse;
	friend class ACS_WaitForFinalResponse;

public:
	AppLayerChannel(const std::string& arName, openpal::Logger&, AppLayer*, openpal::IExecutor*, openpal::TimeDuration aTimeout);
	virtual ~AppLayerChannel() {}

	// Resets the channel to the initial state
	void Reset();

	// send, wether a response is expected is implicit based on func code
	void Send(APDU&, size_t aNumRetry);
	void Cancel();

	// Events
	void OnSendSuccess();
	void OnSendFailure();
	void OnConfirm(int aSeq);

protected:

	// functions for the states to access
	int Sequence() {
		return mSequence;
	}
	int IncrSequence() {
		return mSequence = NextSeq(mSequence);
	}
	void QueueSend(APDU&);
	void ChangeState(ACS_Base*);
	void SetRetry(size_t aNumRetry) {
		mNumRetry = aNumRetry;
	}
	bool Retry(ACS_Base*);

	virtual void DoSendSuccess() = 0;
	virtual void DoFailure() = 0;

	void DoPartialResponse(APDU& arAPDU);
	void DoFinalResponse(APDU& arAPDU);

	void StartTimer();
	void CancelTimer();

	openpal::Logger& GetLogger() {
		return mLogger;
	}


	AppLayer* mpAppLayer;
	int mSequence;	// Rotating sequence number for the channel
	ACS_Base* mpState;

	static int NextSeq(int s) {
		return (s + 1) % 16;
	}

private:

	void Timeout();

	APDU* mpSendAPDU;
	size_t mNumRetry;
	openpal::IExecutor* mpExecutor;
	openpal::ITimer* mpTimer;
	bool mConfirming;
	const openpal::TimeDuration M_TIMEOUT;
	const std::string M_NAME;
};

}

#endif
