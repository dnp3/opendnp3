
#ifndef __LINK_PARSER_H_
#define __LINK_PARSER_H_

#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/link/ILinkContext.h>

#include <opendnp3/link/LinkLayerParser.h>

#include <openpal/logging/LogRoot.h>

#include <openpal/container/Settable.h>
#include <openpal/container/Queue.h>
#include <openpal/executor/IExecutor.h>
#include <openpal/container/RingBuffer.h>

class LinkParserImpl : public opendnp3::ILinkRouter
{
	public:

	LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context, void (*startTxFun_)(void));
	
	// called from the upper part of the stack, from the main loop
	virtual void BeginTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext) final override;	
	
	// called from the uart ISR when rxready
	void PutRx(uint8_t byteIn);
	
	// called from uart ISR when txready	
	bool GetTx(uint8_t& byteOut);
	
	// Call from the main loop
	// gets rx bytes out of the rxBuffer
	// puts tx bytes into the txBuffer
	void CheckRxTx();
			
	private:
	
	void CheckRx();
	void CheckTx();
	
	bool isTransmitting;
	openpal::ReadOnlyBuffer transmission;
	
	openpal::RingBuffer<16> rxBuffer;
	openpal::RingBuffer<16> txBuffer;
					
	uint32_t FlushRxBuffer();		
	
	void (*startTxFun)(void);			
	
	openpal::IExecutor* pExecutor;
	opendnp3::ILinkContext* pContext;		
	opendnp3::LinkLayerParser parser;
};

#endif

