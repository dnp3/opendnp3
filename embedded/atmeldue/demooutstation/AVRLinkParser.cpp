
#include "AVRLinkParser.h"

#include "CriticalSection.h"

namespace arduino {
	class AVRLinkParser;
}

arduino::AVRLinkParser* gLinkParser = nullptr;



namespace arduino {
	
AVRLinkParser::AVRLinkParser(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context) : 	
	pExecutor(&exe),
	pContext(&context),
	parser(root.GetLogger())
{
	
}

void AVRLinkParser::Init()
{
	gLinkParser = this;
	
	// TODO - Configure the USART and enable rx/tx interrupts	
}

void AVRLinkParser::Receive(uint8_t byte)
{	
	// comes in on the interrupt
	rxBuffer.Put(byte);
}

void AVRLinkParser::CheckTransmit()
{	
	// TODO - ARM specific transmit check	
	
	/*
	if(txQueue.IsNotEmpty() && (UCSR0A & (1<<UDRE0))) 
	{		
		auto pTx = txQueue.Peek();
		UDR0 = pTx->buffer[0];
		pTx->buffer.Advance(1);
		
		if(pTx->buffer.IsEmpty())
		{
			txQueue.Pop();
			auto pri = pTx->primary;
			auto callback = [this, pri]() { pContext->OnTransmitResult(pri, true); };
			pExecutor->PostLambda(callback);
		}
	}
	*/	
}

void AVRLinkParser::ProcessRx()
{
	auto num = CopyRxBuffer();
	if(num > 0)
	{
		parser.OnRead(num, pContext);
	}
}

uint32_t AVRLinkParser::CopyRxBuffer()
{	
	// disable interrupts and copy contents of ring buffer to the receiver's write buffer
	CriticalSection cs; 
	auto buffer = parser.WriteBuff();
	return rxBuffer.Read(buffer);
}
	
void AVRLinkParser::QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary)
{
	txQueue.Enqueue(Transmission(buffer, primary));
	this->CheckTransmit();
}

}

/* TODO - ARM specific USART rx/tx interrupt handlers
ISR(USART0_TX_vect)
{	
	if(gLinkParser)
	{
		gLinkParser->CheckTransmit();
	}
}

ISR(USART0_RX_vect)
{
	uint8_t rx;
	rx = UDR0;	
	if(gLinkParser)
	{
		gLinkParser->Receive(rx);
	}	
}
*/

