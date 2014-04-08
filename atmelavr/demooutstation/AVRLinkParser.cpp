
#include "AVRLinkParser.h"

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h> 

namespace arduino {

AVRLinkParser::AVRLinkParser(openpal::LogRoot& root, opendnp3::ILinkContext& context) : 
	txState(SendState::IDLE),
	pContext(&context),
	receiver(root.GetLogger(), &context)	
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));	
}

void AVRLinkParser::Tick()
{
	if(txQueue.IsNotEmpty()) 
	{
		if(UCSR0A & (1<<UDRE0)) // Can we transmit?
		{
			auto& tx = txQueue.Peek();						
			UDR0 = tx.buffer[0];
			tx.buffer.Advance(1);
			
			if(tx.buffer.IsEmpty())
			{
				txQueue.Pop();
				pContext->OnTransmitResult(tx.primary, true);			
			}				
		}		
	}
	
	if(UCSR0A & (1<<RXC0)) // Can we read?
	{
		auto byte = UDR0;
		receiver.WriteBuff()[0] = byte;
		receiver.OnRead(1);
	}
}
	
void AVRLinkParser::QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary)
{
	txQueue.Enqueue(Transmission(buffer, primary));
}

}

