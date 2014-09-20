
#include "LinkParserImpl.h"

#include "CriticalSection.h"
	
LinkParserImpl::LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context, void (*startTxFun_)(void)) :	
	startTxFun(startTxFun_),
	txQueue(2),	
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
		
}
	
	this->channelHAL.disableTxReadyISR();
	
	if(txQueue.IsNotEmpty())
	{
		auto pTx = txQueue.Peek();
		if(pTx->buffer.IsEmpty())
		{
			txQueue.Pop();
			auto pri = pTx->primary;
			if(txQueue.IsNotEmpty())
			{
				this->channelHAL.enableTxReadyISR();										
			}			
			pContext->OnTransmitResult(pri, true);						
		}
	}
}

/*				
	if(txQueue.IsNotEmpty()) 
	{		
		auto pTx = txQueue.Peek();
		if(pTx->buffer.Size() > 0)
		{
			byte = pTx->buffer[0];
			pTx->buffer.Advance(1);
			return true;	
		}
		else
		{
			return false;
		}						
												
		return true;
	}	
	else
	{		
		return false;
	}
}
*/

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
	
void LinkParserImpl::QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary)
{
	if(transmission.IsSet())
	{
		auto callback = [pContext]()
		pExecutor->
		pContext->OnTransmitResult(false);
	}
}

