/*
 * demooutstation.cpp
 *
 * Created: 8/27/2014 7:55:11 AM
 *  Author: jadamcrain
 */ 

#include "sam.h"
#include "sam3x8e.h"


#include "ExecutorImpl.h"
#include "LinkParserImpl.h"
#include "CommandHandlerImpl.h"
#include "Macros.h"

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/IOutstationApplication.h>

#include <openpal/logging/LogRoot.h>

using namespace opendnp3;
using namespace openpal;

void ToggleLED();

const uint32_t LED_MASK = (1u << 27); 

const uint32_t MAX_FRAG_SIZE = 2048;

ExecutorImpl* gpExecutor = nullptr;
LinkParserImpl* gpParser = nullptr;

bool gValueLED = true;

// systick ISR handler
void SysTick_Handler(void)
{
	gpExecutor->Tick();		
}

void DisableTxISR()
{
	UART->UART_IDR = UART_IDR_TXRDY;
}

void EnableTxISR()
{
	UART->UART_IER = UART_IER_TXRDY;
}

void DisableRxISR()
{
	UART->UART_IDR = UART_IDR_RXRDY;
}

void EnableRxISR()
{
	UART->UART_IER = UART_IER_RXRDY;
}

// rx/tx ready UART ISR handler
void UART_Handler(void)
{
	// is the receive ready?
	if(UART->UART_SR & UART_SR_TXRDY)
	{
		uint8_t rxByte = UART->UART_RHR;
		gpParser->PutRx(rxByte);
	}
	
	// is the transmit ready?
	if(UART->UART_SR & UART_SR_TXRDY)
	{
		uint8_t txByte;
		if(gpParser->GetTx(txByte))
		{
			//send the character
			UART->UART_THR = txByte;
		}
		else
		{
			DisableTxISR();
		}
	}
}

void ConfigureUart(void)
{
	uint32_t ul_sr;
	
	// ==> Pin configuration
	// Disable interrupts on Rx and Tx
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Disable the PIO of the Rx and Tx pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// Read current peripheral AB select register and set the Rx and Tx pins to 0 (Peripheral A function)
	ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= ~(PIO_PA8A_URXD | PIO_PA9A_UTXD) & ul_sr;
	
	// Enable the pull up on the Rx and Tx pin
	PIOA->PIO_PUER = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	// ==> Actual uart configuration
	// Enable the peripheral uart controller
	PMC->PMC_PCER0 = 1 << ID_UART;
	
	// Reset and disable receiver & transmitter
	UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	
	// Set the baudrate to 115200
	UART->UART_BRGR = 45; // 84000000 / 16 * x = BaudRate (write x into UART_BRGR)
	// UART->UART_BRGR = 547; // <- 9600 baud
	
	// No Parity
	UART->UART_MR = UART_MR_PAR_NO;
	
	// Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
		
	// Disable / Enable interrupts on rxready	
	UART->UART_IDR = 0xFFFFFFFF;
	NVIC_EnableIRQ((IRQn_Type) ID_UART);	
	UART->UART_IER = UART_IER_RXRDY | UART_IER_TXRDY;
	//UART->UART_IER = UART_IER_RXRDY;	// | UART_IER_TXRDY;	
	
	// Enable receiver and trasmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;	
}

int main(void)
{
    // Initialize the SAM system
    SystemInit();
		
	// Output Enable Register
	REG_PIOB_OER = LED_MASK;
		
	ExecutorImpl exe(5,5);
	gpExecutor = &exe;
	
	LogRoot root(nullptr, "root", 0);
	
	TransportStack stack(root, &exe, MAX_FRAG_SIZE, nullptr, LinkConfig(false, false));
	
	// 5 static binaries, 0 others
	DynamicallyAllocatedDatabase buffers(DatabaseTemplate(10,0,0,10));
	// allow a max of 5 events
	DynamicallyAllocatedEventBuffer eventBuffers(20);
		
	Database database(buffers.GetFacade());
	
	OutstationConfig config;
	config.params.maxRxFragSize = MAX_FRAG_SIZE;
	config.params.maxTxFragSize = MAX_FRAG_SIZE;
	config.eventBufferConfig = EventBufferConfig(5);
	config.params.allowUnsolicited = true;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;	

	// object that handles command (CROB / analog output) requests
	// This example can toggle an LED
	CommandHandlerImpl commandHandler;		

	Outstation outstation(config, exe, root, stack.transport, commandHandler, DefaultOutstationApplication::Instance(), database, eventBuffers.GetFacade());

	stack.transport.SetAppLayer(&outstation);		
	
	LinkParserImpl parser(root, exe, stack.link, EnableTxISR);
	gpParser = &parser;
	
	stack.link.SetRouter(parser);
	stack.link.OnLowerLayerUp();
		
	// Setup SysTick Timer for 1 msec interrupts
	assert(SysTick_Config(84000000 / 1000) == 0);
	
	ConfigureUart();				
	
	REG_PIOB_CODR = LED_MASK; // Clear Output Data Register, turns LED off				
	auto lambda = []() { REG_PIOB_SODR = LED_MASK; };
	exe.Start(TimeDuration::Milliseconds(3000), Action0::Bind(lambda));
	
	int loopCount = 0;
	
	for (;;)
	{	
		parser.CheckRxTx();
			
		exe.Run();
						
		// WFE stands for "wait for event". This seems to have stopped the board resets		
		__WFE();
		
		// The systick is at 1000hz, so this reduces the LED toggle to 10hz
		if(loopCount == 1000)
		{
			loopCount = 0;
			ToggleLED();
		}
		
		++loopCount;
	}

	return 0;
}

void ToggleLED()
{	
	// Toggle the LED
	if(gValueLED)
	{
		REG_PIOB_SODR = LED_MASK; // Set Output Data Register, turns LED on
	}
	else
	{
		REG_PIOB_CODR = LED_MASK; // Clear Output Data Register, turns LED off
	}
	gValueLED= !gValueLED;			
}

