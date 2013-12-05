#ifndef __LOWER_LAYER_TO_PHYS_ADAPTER_H_
#define __LOWER_LAYER_TO_PHYS_ADAPTER_H_


#include <openpal/IHandlerAsync.h>
#include <openpal/AsyncLayerInterfaces.h>

namespace openpal
{
class IPhysicalLayerAsync;
}

namespace opendnp3
{

/** Class for turning an async physical layer into an ILowerLayer
*/
class LowerLayerToPhysAdapter : public openpal::IHandlerAsync, public openpal::ILowerLayer
{
public:
	LowerLayerToPhysAdapter(openpal::Logger, openpal::IPhysicalLayerAsync*, bool aAutoRead = true);
	~LowerLayerToPhysAdapter();

	size_t GetNumOpenFailure() {
		return mNumOpenFailure;
	}
	bool OpenFailureEquals(size_t aNum) {
		return GetNumOpenFailure() == aNum;
	}

	void StartRead();



private:

	virtual std::string RecvString() const {
		return "Adapter <-";
	}
	virtual std::string SendString() const {
		return "Adapter ->";
	}

	bool mAutoRead;
	size_t mNumOpenFailure;

	static const size_t BUFFER_SIZE = 1 << 16; // 65,536

	uint8_t mpBuff[BUFFER_SIZE]; // Temporary buffer since IPhysicalLayerAsync now directly supports a read operation

	/* Implement IAsyncHandler */
	void _OnOpenFailure();

	/* Implement IUpperLayer */
	void _OnReceive(const openpal::ReadOnlyBuffer& arBuffer);
	void _OnSendSuccess();
	void _OnSendFailure();
	void _OnLowerLayerUp();
	void _OnLowerLayerDown();
	void _OnLowerLayerShutdown();

	openpal::IPhysicalLayerAsync* mpPhys;

	/* Implement ILowerLayer */
	void _Send(const openpal::ReadOnlyBuffer& arBuffer);
};

}

#endif
