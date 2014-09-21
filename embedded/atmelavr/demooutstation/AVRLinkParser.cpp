
#include "AVRLinkParser.h"

#include "CriticalSection.h"

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
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
	
	// enable rx & tx interrupts
	UCSR0B |= (1 << RXCIE0);
	UCSR0B |= (1 << TXCIE0);
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
		while(!txBuffer.Full() && !transmission.Empty())
		{			
			uint8_t byte = transmission[0];
			txBuffer.Put(byte);
			transmission.Advance(1);			
		}				
		
		if(transmission.IsEmpty() && txBuffer.Empty())
		{
			isTransmitting = false;			
			auto callback = [this]() { pContext->OnTransmitResult(true); };
			pExecutor->PostLambda(callback);
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
	if(!isTransmitting)
	{
		isTransmitting = true;
		transmission = buffer;
		this->CheckTx();
		
		// to kick off the transmit, we have to actually load the first value
		// subsequent bytes are transmitted on the txReady interrupt handler
		/*
		uint8_t txByte;
		if(txBuffer.Get(txByte))
		{
			UDR0 = txByte;
		}
		*/
	}	
}

}

ISR(USART0_TX_vect)
{			
	uint8_t txByte;
	if(gLinkParser->GetTx(txByte))
	{
		UDR0 = txByte;
	}	
}

ISR(USART0_RX_vect)
{
	uint8_t rx;
	rx = UDR0;		
	gLinkParser->Receive(rx);	
}

