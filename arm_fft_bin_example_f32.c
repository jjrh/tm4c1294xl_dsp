/* ----------------------------------------------------------------------
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * $Date:         17. January 2013
 * $Revision:     V1.4.0
 *
 * Project:       CMSIS DSP Library
 * Title:	     arm_fft_bin_example_f32.c
 *
 * Description:   Example code demonstrating calculation of Max energy bin of
 *                frequency domain of input signal.
 *
 * Target Processor: Cortex-M4/Cortex-M3
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   - Neither the name of ARM LIMITED nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

#include <stdbool.h>
#include "arm_math.h"
#include "arm_const_structs.h"

#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c1294ncpdt.h"

#include "driverlib/uart.h"
#include "utils/uartstdio.h"

/*
 * Constants
 */

// Number of samples for FFT
#define NUM_SAMPLES 128

// Rate to sample input
#define SAMPLING_RATE 44100

// Number of frequencies to use for testing
#define NUM_FREQS 4

/*
 * Forward declaration of functions
 */
void configureADC();
void runFFT();
void TIMER1_Handler();
void ADC0_SampleHandler();
void ConfigureUART();
int freqIndex(int);

/*
 *  Global variables
 */

// Frequencies to check
uint32_t freqs[NUM_FREQS] = { 1000, 2000, 3000, 4000 };


// Pointer to where the next sample should
// go in the input buffer.
uint32_t inputIndex;

// Input data buffer for FFT
float32_t inputData[NUM_SAMPLES];

// RFFT complex output goes here in paired format
// We need to get the complex magnitude to analyze
// each frequency bucket
static float32_t rfftOutput[NUM_SAMPLES];

// "Final" output goes here; the magnitude
// of the raw complex output. We can look at each index
// of this array to see the magnitude of that frequency range.
// Each bucket will cover
//        B = (SAMPLING_RATE / NUM_SAMPLES) Hz,
// where index 0 represents
//        0 <--> B Hz,
// and the last index, ((NUM_SAMPLES / 2) - 1), represents
//        (SAMPLING_RATE / 2) - B) <--> (SAMPLING_RATE / 2) Hz
//
// For example, with a 44100 Hz sampling rate, and a 512 point FFT,
// we have a bucket width of 44100 / 512 ~= 86.1 Hz.
// Bucket 0 will represent 0 <--> 86.1 Hz,
// and Bucket 255 will represent 21964 <--> 22050 Hz.
//
static float32_t magOutput[NUM_SAMPLES/2];

// Used to retrieve data from ADC sequencer
uint32_t adc_value[1];


/* ------------------------------------------------------------------
 * Global variable for system clock
 * ------------------------------------------------------------------- */
uint32_t g_ui32SysClock;

/* ------------------------------------------------------------------
 * Flags for RFFT initialization
 * ------------------------------------------------------------------- */

// Do the forward transform (0), or inverse (1)
uint32_t ifftFlag = 0;

// Used internally by CMSIS Math & DSP libraries
uint32_t doBitReverse = 1;

// Reference index at which max energy of bin occurs
// TODO: This is just used for testing, and can probably be removed
uint32_t peakBucket = 0;

/* ----------------------------------------------------------------------
 * Max magnitude RFFT Bin
 * ------------------------------------------------------------------- */

int32_t main(void)
{
	// Configure system clock.
	g_ui32SysClock = ROM_SysCtlClockFreqSet(SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_CFG_VCO_480, 120000000);

	// Enable FPU
	MAP_FPULazyStackingEnable();
	MAP_FPUEnable();

	// Initialize the UART and write status.
	ConfigureUART();

	UARTprintf("\033[2JFFT Test\n");

	// Initialize input buffer index for FFT data
	inputIndex = 0;

	// Set up ADC sampling and interrupt
	configureADC();


	while(1);                             /* main function does not return */
}

void configureADC()
{
	// Enable peripherals
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	// Wait for peripherals to activate
	MAP_SysCtlDelay(2);

	// Disable sequencer 0 before configuring
	MAP_ADCSequenceDisable(ADC0_BASE, 3);

	// Configure GPIO Pin for ADC
	MAP_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);

	// TODO: Used for measuring timing, can be removed later
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
    MAP_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);

	/*
	 * Configure timer for ADC triggering
	 */
	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	// Load timer for periodic sampling of ADC
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32SysClock/SAMPLING_RATE);

	// Enable ADC triggering
	MAP_TimerControlTrigger(TIMER1_BASE, TIMER_A, true);

	// Trigger ADC on timer A timeout
	MAP_TimerADCEventSet(TIMER1_BASE, TIMER_ADC_TIMEOUT_A);


	/*
	 * Configure ADC
	 */

	// Clear the interrupt raw status bit (should be done early
	// on, because it can take several cycles to clear.)
	MAP_ADCIntClear(ADC0_BASE, 3);

	// ADC0, Seq 0, Timer triggered, Priority 0
	MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);

	// Configure sequencer step 0 for proper input channel, interrupt enable, and end of sequence.
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

	// Enable sequencer
	MAP_ADCSequenceEnable(ADC0_BASE, 3);

	// Enable interrupts for sample conversion complete
	MAP_ADCIntEnable(ADC0_BASE, 3);

	// Enable NVIC interrupt for ADC0 SS3
	MAP_IntEnable(INT_ADC0SS3);

	// Enable all interrupts
	MAP_IntMasterEnable();

	// Start timer
	MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

void ADC0_SampleHandler()
{
	// Turn on debuggin signal
	MAP_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);

	// Clear interrupt signal in ADC
	MAP_ADCIntClear(ADC0_BASE, 3);

	// Get data and store in array
	MAP_ADCSequenceDataGet(ADC0_BASE, 3, adc_value);

	// Cast as floating point and store in input buffer
	inputData[inputIndex++] = (float) adc_value[0];

	// If input buffer is full, run the FFT
	if (inputIndex > NUM_SAMPLES - 1)
	{
		// Reset input buffer index
		inputIndex = 0;

		// Disable interrupt and timer
		MAP_ADCIntDisable(ADC0_BASE, 3);
		MAP_TimerDisable(TIMER1_BASE, TIMER_A);

		// Run the transform
		runFFT();

		// Turn off debuggin signal (before 1 sec delay!)
		MAP_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);

		// Delay for 1 second
		MAP_SysCtlDelay(g_ui32SysClock / 3);

		//Re-enable timer and ADC interrupts to run another FFT
		MAP_ADCIntEnable(ADC0_BASE, 3);
		MAP_TimerEnable(TIMER1_BASE, TIMER_A);

	}
	// Turn off debuggin signal
	MAP_GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
}

void runFFT()
{
	//int32_t startTime, stopTime, totalTime; // Variables for timing testing
	float32_t maxValue;

	// Setup testing timer
	// TODO: This is just used for timing testing
	//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	//	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	//	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock); // 1 second

	// Create a RFFT instance
	arm_rfft_fast_instance_f32 fft;
	arm_rfft_fast_init_f32(&fft,NUM_SAMPLES);

	//	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	//	startTime = TIMER0_TAR_R;

	// Run the FFT

	/* Process the real data through the RFFT module */
	arm_rfft_fast_f32(&fft, inputData, rfftOutput, ifftFlag);

	/* Process the data through the Complex Magnitude Module for
	  calculating the magnitude at each bin */
	arm_cmplx_mag_f32(rfftOutput, magOutput, NUM_SAMPLES / 2);

	// The 0 index of FFT output is the DC component of
	// the input signal. We don't want to consider this when
	// looking for the peak frequency.
	magOutput[0] = 0;

	/* Calculates maxValue and returns corresponding BIN value */
	arm_max_f32(magOutput, NUM_SAMPLES / 2, &maxValue, &peakBucket);

	//	stopTime = TIMER0_TAR_R;
	//	totalTime = startTime - stopTime;
	//	int totalTimeUs = totalTime / 120;

	// Convert peak frequency bucket number to frequency
	int peakFrequency = peakBucket * SAMPLING_RATE / NUM_SAMPLES;

	int i;
	for (i = 0; i < NUM_FREQS; i++)
	{
		// Print each frequency (closest bucket) and its magnitude
		// Note: Print output is cast to integer due to restrictions in UARTprintf function
		//UARTprintf("%d Hz: %d\n", freqs[i], (int) magOutput[freqIndex(freqs[i])]);

	}
	//UARTprintf("\n\n");


	MAP_SysCtlDelay(1); // Only used to get a breakpoint in debugger
}

// Returns the magnitude output buffer index for a
// given frequency in Hz
int freqIndex(int freq)
{
	// This is based on buckets frequency range being
	// CENTERED on n*bucketSize.
	float bucketSize = SAMPLING_RATE / ((float) NUM_SAMPLES);

	return (int)( (freq + (bucketSize/2) ) / bucketSize);
}
//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
	//
	// Enable the GPIO Peripheral used by the UART.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// Enable UART0.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	//
	// Configure GPIO Pins for UART mode.
	//
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//
	// Initialize the UART for console I/O.
	//
	UARTStdioConfig(0, 115200, g_ui32SysClock);
}
