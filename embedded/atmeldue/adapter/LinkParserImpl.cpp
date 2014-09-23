
#include "LinkParserImpl.h"
	
LinkParserImpl::LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkSession& context, void (*startTxFun_)(void)) :	
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
	auto writeBuffer = parser.WriteBuff();
	auto count = rxBuffer.GetMany(writeBuffer);
		
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
		if(txBuffer.PutMany(transmission) > 0)
		{
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

void LinkParserImpl::BeginTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkSession* pContext)
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

