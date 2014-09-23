
#include "AVRLinkParser.h"

#include "CriticalSection.h"

#define F_CPU 16000000UL
#define BAUDRATE 38400 // produces nice and stable 25.04
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1) 

#include "Macros.h"

#include <avr/io.h>
#include <avr/interrupt.h>

AVRLinkParser* gLinkParser = nullptr;

AVRLinkParser::AVRLinkParser(openpal::LogRoot& root, openpal::IExecutor& exe, opendnp3::ILinkContext& context) : 		
	isTransmitting(false),
	pExecutor(&exe),	
	pContext(&context),
	parser(root.GetLogger())
{
	
}

void AVRLinkParser::Init()
{
	gLinkParser = this;
	
	// configure the baud rate
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
			
	// turn on rx / tx
	UCSR0B = BIT(RXEN0)| BIT(TXEN0);
	
	// configure 8 databits
	UCSR0C = BIT(UCSZ00) | BIT(UCSZ01);
	
	// enable rx ready ISR
	UCSR0B |= BIT(RXCIE0);
	// UCSR0B |= (1 << TXCIE0);
}

void AVRLinkParser::Receive(uint8_t byte)
{	
	// comes in on the interrupt
	rxBuffer.Put(byte);
}

bool AVRLinkParser::GetTx(uint8_t& txByte)
{
	// requested on the interrupt
	return txBuffer.Get(txByte);
}

void AVRLinkParser::CheckTx()
{	
	if(isTransmitting)
	{						
		// transfer bytes into the ring buffer
		while(!txBuffer.Full() && !transmission.IsEmpty())
		{			
			uint8_t byte = transmission[0];
			txBuffer.Put(byte);
			transmission.Advance(1);
		}			
		
		// enable data register empty interrupt
		UCSR0B |= BIT(UDRIE0);	
		
		if(transmission.IsEmpty() && txBuffer.Empty())
		{
			isTransmitting = false;						
			pContext->OnTransmitResult(true);
		}		
	}	
}

void AVRLinkParser::CheckRxTx()
{		
	this->CheckTx();
	this->CheckRx();
}

void AVRLinkParser::CheckRx()
{	
	// copy contents of ring buffer to the receiver's write buffer		
	uint32_t count = 0;
	auto buffer = parser.WriteBuff();	
	uint8_t rxByte;
	while(!buffer.IsEmpty() && rxBuffer.Get(rxByte))
	{				
		buffer[0] = rxByte;
		buffer.Advance(1);
		++count;
	}
	
	if(count > 0)
	{
		parser.OnRead(count, pContext);
	}		
}
	
void AVRLinkParser::BeginTransmit(const openpal::ReadOnlyBuffer& buffer, opendnp3::ILinkContext* pContext)
{
	if(isTransmitting)
	{
		auto failure = [pContext](){ pContext->OnTransmitResult(false); };
		pExecutor->PostLambda(failure);
	}
	else	
	{
		isTransmitting = true;
		transmission = buffer;		
		
		this->CheckTx();													
	}	
}

ISR(USART0_UDRE_vect)
{					
	uint8_t txByte;
	if(gLinkParser->GetTx(txByte))
	{
		UDR0 = txByte;
	}	
	else
	{
		// nothing left to transmit. Disable the interrupt
		UCSR0B &= ~BIT(UDRIE0);
	}
}

ISR(USART0_RX_vect)
{
	uint8_t rx;
	rx = UDR0;		
	gLinkParser->Receive(rx);	
}

