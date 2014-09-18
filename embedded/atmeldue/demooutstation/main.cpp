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

void ToggleLED(IExecutor* pExecutor);

const uint32_t LED_MASK = (1u << 27); 

const uint32_t MAX_FRAG_SIZE = 2048;

ExecutorImpl* gpExecutor = nullptr;
bool gValue = true;

void SysTick_Handler(void)
{
	gpExecutor->Tick();		
}

void configure_uart(void)
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
	
	// No Parity
	UART->UART_MR = UART_MR_PAR_NO;
	
	// Disable PDC channel requests
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	// Disable / Enable interrupts on end of receive
	UART->UART_IDR = 0xFFFFFFFF;
	NVIC_EnableIRQ((IRQn_Type) ID_UART);
	UART->UART_IER = UART_IER_RXRDY;
	
	// Enable receiver and trasmitter
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;	
}

int uart_getchar(uint8_t *c)
{
    // Check if the receiver is ready
    if((UART->UART_SR & UART_SR_RXRDY) == 0)
    return 1;
 
    // Read the character
    *c = (uint8_t) UART->UART_RHR;
    return 0;
}
 
int uart_putchar(const uint8_t c)
{
    // Check if the transmitter is ready
    if(!(UART->UART_SR & UART_SR_TXRDY))
    return 1;
 
    // Send the character
    UART->UART_THR = c;
    return 0;
}

void UART_Handler(void)
{
	uint8_t c;
	
	// Check if the interrupt source is receive ready
	if(UART->UART_IMR & UART_IMR_RXRDY)
	{
		if(uart_getchar(&c) == 0)
		{
			uart_putchar(c);
		}
	}
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
	DynamicallyAllocatedDatabase buffers(5);
	// allow a max of 5 events
	DynamicallyAllocatedEventBuffer eventBuffers(5);
		
	Database database(buffers.GetFacade());
	
	OutstationConfig config;
	config.params.maxRxFragSize = MAX_FRAG_SIZE;
	config.params.maxTxFragSize = MAX_FRAG_SIZE;
	config.eventBufferConfig = EventBufferConfig(5);
	config.params.allowUnsolicited = true;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;	

	// Object that handles command (CROB / analog output) requests
	// This example can toggle an LED
	CommandHandlerImpl commandHandler;
	
	DefaultOutstationApplication application;

	Outstation outstation(config, exe, root, stack.transport, commandHandler, application, database, eventBuffers.GetFacade());

	stack.transport.SetAppLayer(&outstation);
	
	LinkParserImpl parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);
	stack.link.OnLowerLayerUp();
		
	// Setup SysTick Timer for 1 msec interrupts
	if (SysTick_Config(84000000 / 1000)) {
		while (1) {  // trap error
		}
	}
	
	configure_uart();
	
	// enable USART RX/TX interrupts
	parser.Init();	
	
	ToggleLED(&exe);				
	
	for (;;)
	{		
		exe.Run();
		
		parser.ProcessRx();
		
		// TODO - put the device into a sleep mode that the system timer or usart can wake it from
	}

	return 0;
}

void ToggleLED(IExecutor* pExecutor)
{	
	// Toggle the LED
	if(gValue)
	{
		REG_PIOB_SODR = LED_MASK; // Set Output Data Register, turns LED on
	}
	else
	{
		REG_PIOB_CODR = LED_MASK; // Clear Output Data Register, turns LED off
	}
	gValue = !gValue;
		
	// schedule the next toggle
	auto lambda = [pExecutor]() { ToggleLED(pExecutor); };
	pExecutor->Start(TimeDuration::Milliseconds(200), Action0::Bind(lambda));
}

