
#include "LinkParserImpl.h"

#include "CriticalSection.h"
	
LinkParserImpl::LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context, void (*startTxFun_)(void)) :	
	isTransmitting(false),
	startTxFun(startTxFun_),	
	pExecutor(&exe),
	pContext(&context),
	parser(root.GetLogger())
{
	
}

bool LinkParserImpl::GetTx(uint8_t& byteOut)
{		
	return txBuffer.Get(byteOut);
}

void LinkParserImpl::PutRx(uint8_t byteIn)
{
	rxBuffer.Put(byteIn);
}

void LinkParserImpl::CheckRxTx()
{	
	this->CheckRx();
	this->CheckTx();	
}
	
void LinkParserImpl::CheckRx()
{
	// flush the rx buffer
	auto count = this->FlushRxBuffer();	
		
	// if we received some bytes, invoke the parser
	if(count > 0)									
	{
		parser.OnRead(count, pContext);	
	}
}

void LinkParserImpl::CheckTx()
{		
	if(isTransmitting) // we're in the middle of transmit
	{
		while(!txBuffer.Full() && !transmission.IsEmpty())
		{
			auto txByte = transmission[0];
			transmission.Advance(1);
			txBuffer.Put(txByte);
			this->startTxFun();
		}
				
		if(transmission.IsEmpty() && txBuffer.Empty())
		{
			isTransmitting = false;
			auto callback = [this]() { this->pContext->OnTransmitResult(true); };
			pExecutor->PostLambda(callback);
		}
	}
}

uint32_t LinkParserImpl::FlushRxBuffer()
{
	uint32_t count = 0;
	auto buffer = parser.WriteBuff();	
	while(buffer.IsNotEmpty() && rxBuffer.Get(buffer[0]))
	{		
		buffer.Advance(1);
		++count;
	}
	return count;
}
	
void LinkParserImpl::BeginTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext)
{
	if(isTransmitting)
	{
		auto callback = [pContext]() { pContext->OnTransmitResult(false); };
		pExecutor->PostLambda(callback);
		
	}
	else
	{
		isTransmitting = true;
		transmission = buffer;
		this->CheckTx();		
	}	
}

