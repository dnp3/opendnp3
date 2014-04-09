
#include "AVRLinkParser.h"

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>

namespace arduino {
	class AVRLinkParser;
}

arduino::AVRLinkParser* gLinkParser = nullptr;



namespace arduino {
	
AVRLinkParser::AVRLinkParser(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context) : 
	txState(SendState::IDLE),
	pExecutor(&exe),
	pContext(&context),
	receiver(root.GetLogger(), &context)	
{
	gLinkParser = this;
	
	// configure the baud rate
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	
	// turn on rx / tx
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);	
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
	
	// enable rx interrupts
	UCSR0B |= (1 << RXCIE0);
}

void AVRLinkParser::Receive(uint8_t byte)
{	
	auto lambda = [this, byte] {
		receiver.WriteBuff()[0] = byte;	
		receiver.OnRead(1);			
	};
	pExecutor->PostLambda(lambda);
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
	
	/*
	if(UCSR0A & (1<<RXC0)) // Can we read?
	{
		auto byte = UDR0;
		receiver.WriteBuff()[0] = byte;
		receiver.OnRead(1);
	}
	*/
}
	
void AVRLinkParser::QueueTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext, bool primary)
{
	txQueue.Enqueue(Transmission(buffer, primary));
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

