
#ifndef __AVR_LINK_PARSER_H_
#define __AVR_LINK_PARSER_H_

#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/link/ILinkContext.h>


#include <opendnp3/link/LinkLayerParser.h>

#include <openpal/logging/LogRoot.h>

#include <openpal/container/Settable.h>
#include <openpal/container/Queue.h>
#include <openpal/container/RingBuffer.h>
#include <openpal/executor/IExecutor.h>

class AVRLinkParser : public opendnp3::ILinkRouter
{
	public:

	AVRLinkParser(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context);
	
	virtual void BeginTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext) final override;	
	
	// called from the rxReady ISR
	void Receive(uint8_t rxByte);
	
	// called from the txReady ISR
	bool GetTx(uint8_t& txByte);
	
	void Init();
			
	void CheckRxTx();
	
	private:
	
	void CheckTx();
	void CheckRx();
							
	openpal::RingBuffer<16> rxBuffer;
	openpal::RingBuffer<16> txBuffer;
	
	bool isTransmitting;
	openpal::ReadOnlyBuffer transmission;		
	
	openpal::IExecutor* pExecutor;
	opendnp3::ILinkContext* pContext;		
	opendnp3::LinkLayerParser parser;
};

#endif

