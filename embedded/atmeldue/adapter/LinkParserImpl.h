
#ifndef __LINK_PARSER_H_
#define __LINK_PARSER_H_

#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/link/ILinkContext.h>

#include <opendnp3/link/LinkLayerParser.h>

#include <openpal/logging/LogRoot.h>

#include <openpal/container/Settable.h>
#include <openpal/container/Queue.h>
#include <openpal/container/RingBuffer.h>
#include <openpal/executor/IExecutor.h>

typedef int (*ReadFunc)(uint8_t *c);
typedef int (*WriteFunc)(const uint8_t c);

class LinkParserImpl : public opendnp3::ILinkRouter
{
	public:

	LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context, ReadFunc read_, WriteFunc write_);
	
	virtual void QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary) final override;	
	
	
	
	void CheckTxRx();
	
	
	
	private:
	
	void Receive(uint8_t byte);
	
	void CheckTx();	
	void CheckRx();
	
	ReadFunc read;
	WriteFunc write;
	
	uint32_t CopyRxBuffer();
		
	struct Transmission
	{
		Transmission(const openpal::ReadOnlyBuffer& buffer_, bool primary_) :
			buffer(buffer_),
			primary(primary_)
			{}

		Transmission() : buffer(), primary(false)
		{}

		openpal::ReadOnlyBuffer buffer;
		bool primary;
	};
	
	openpal::Queue<Transmission, uint8_t> txQueue;
	openpal::RingBuffer rxBuffer;
	
	openpal::Settable<openpal::ReadOnlyBuffer> primaryTx;
	openpal::Settable<openpal::ReadOnlyBuffer> secondaryTx;
	
	openpal::IExecutor* pExecutor;
	opendnp3::ILinkContext* pContext;		
	opendnp3::LinkLayerParser parser;
};

#endif

