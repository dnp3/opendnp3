
#include "LinkParserImpl.h"

#include "CriticalSection.h"
	
LinkParserImpl::LinkParserImpl(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context, ReadFunc read_, WriteFunc write_) : 	
	read(read_),
	write(write_),
	txQueue(2),
	rxBuffer(8),
	pExecutor(&exe),
	pContext(&context),
	parser(root.GetLogger())
{
	
}

void LinkParserImpl::CheckTxRx()
{
	this->CheckRx();
	this->CheckTx();
}

void LinkParserImpl::CheckTx()
{					
	if(txQueue.IsNotEmpty()) 
	{		
		auto pTx = txQueue.Peek();
		auto byte = pTx->buffer[0];
		if((*write)(byte) == 0)
		{
			pTx->buffer.Advance(1);
			
			if(pTx->buffer.IsEmpty())
			{
				txQueue.Pop();
				auto pri = pTx->primary;
				auto callback = [this, pri]() { pContext->OnTransmitResult(pri, true); };
				pExecutor->PostLambda(callback);
			}	
		}								
	}	
}

void LinkParserImpl::CheckRx()
{
	uint8_t rx;
	if((*read)(&rx) == 0)
	{
		rxBuffer.Put(rx);
		auto buffer = parser.WriteBuff();
		auto count = rxBuffer.Read(buffer);
		parser.OnRead(count, pContext);
	}	
}
	
void LinkParserImpl::QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary)
{
	txQueue.Enqueue(Transmission(buffer, primary));
	// this->CheckTx();
}

