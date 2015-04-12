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

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup FrequencyBin Frequency Bin Example
 *
 * \par Description
 * \par
 * Demonstrates the calculation of the maximum energy bin in the frequency
 * domain of the input signal with the use of Complex FFT, Complex
 * Magnitude, and Maximum functions.
 *
 * \par Algorithm:
 * \par
 * The input test signal contains a 10 kHz signal with uniformly distributed white noise.
 * Calculating the FFT of the input signal will give us the maximum energy of the
 * bin corresponding to the input frequency of 10 kHz.
 *
 * \par Block Diagram:
 * \image html FFTBin.gif "Block Diagram"
 * \par
 * The figure below shows the time domain signal of 10 kHz signal with
 * uniformly distributed white noise, and the next figure shows the input
 * in the frequency domain. The bin with maximum energy corresponds to 10 kHz signal.
 * \par
 * \image html FFTBinInput.gif "Input signal in Time domain"
 * \image html FFTBinOutput.gif "Input signal in Frequency domain"
 *
 * \par Variables Description:
 * \par
 * \li \c testInput_f32_10khz points to the input data
 * \li \c testOutput points to the output data
 * \li \c fftSize length of FFT
 * \li \c ifftFlag flag for the selection of CFFT/CIFFT
 * \li \c doBitReverse Flag for selection of normal order or bit reversed order
 * \li \c refIndex reference index value at which maximum energy of bin ocuurs
 * \li \c testIndex calculated index value at which maximum energy of bin ocuurs
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_cfft_f32()
 * - arm_cmplx_mag_f32()
 * - arm_max_f32()
 *
 * <b> Refer  </b>
 * \link arm_fft_bin_example_f32.c \endlink
 *
 */


/** \example arm_fft_bin_example_f32.c
 */


#include "arm_math.h"
#include "arm_const_structs.h"

#include <stdbool.h>

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

#include "inc/hw_memmap.h"
#include "inc/tm4c1294ncpdt.h"

#define TEST_LENGTH_SAMPLES 512

// Rate to sample analog input
#define SAMPLING_RATE 44100
//#define SAMPLING_RATE 16000

// Forward declaration of functions
void configureADC();
void runFFT();
void TIMER1_Handler();
void ADC0_SampleHandler();

/* -------------------------------------------------------------------
 * External Input and Output buffer Declarations for FFT Bin Example
 * ------------------------------------------------------------------- */
//extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
//extern float32_t testInput_f32_44khz_256[TEST_LENGTH_SAMPLES];

uint32_t inputIndex;
float32_t inputData[TEST_LENGTH_SAMPLES];

static float32_t rfftOutput[TEST_LENGTH_SAMPLES];
static float32_t testOutput_44khz[TEST_LENGTH_SAMPLES/2];

// Used to get ADC data from sequencer
uint32_t adc_value[1];

/* ------------------------------------------------------------------
 * Global variable for system clock
 * ------------------------------------------------------------------- */
uint32_t g_ui32SysClock;

/* ------------------------------------------------------------------
 * Global variables for FFT Bin Example
 * ------------------------------------------------------------------- */
//uint32_t fftSize = 1024;
uint32_t fftSize = TEST_LENGTH_SAMPLES;

uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;

/* ----------------------------------------------------------------------
 * Max magnitude FFT Bin test
 * ------------------------------------------------------------------- */

int32_t main(void)
{

	g_ui32SysClock = ROM_SysCtlClockFreqSet(SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_CFG_VCO_480, 120000000);

	// Reset index for FFT data
	inputIndex = 0;

	MAP_FPULazyStackingEnable();
	MAP_FPUEnable();

	// Set up ADC sampling and interrupt
	configureADC();

	while(1);                             /* main function does not return */
}

void configureADC()
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	MAP_SysCtlDelay(2);

	// Disable sequencer 0
	MAP_ADCSequenceDisable(ADC0_BASE, 3);

	//
	// Configure GPIO Pin
	//
	MAP_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);

	//  **************************
	// Configure timer
	//  **************************
	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

	// Load timer for periodic sampling of ADC
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32SysClock/SAMPLING_RATE);

	// Enable ADC triggering
	MAP_TimerControlTrigger(TIMER1_BASE, TIMER_A, true);

	// Trigger ADC on timer A timeout
	MAP_TimerADCEventSet(TIMER1_BASE, TIMER_ADC_TIMEOUT_A);

	// **************************
	// Configure ADC
	// **************************
	// Clear the interrupt raw status bit (should be done early
	// on, because it can take several cycles to clear.)
	MAP_ADCIntClear(ADC0_BASE, 3);

	// ADC0, Seq 0, Timer triggered, Priority 0
	MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
	//	MAP_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

	/*// Set all 8 sequencer steps to sample from analog channel 5 (PD7)
	int i;
	for(i = 0; i < 1; i++)
	{
	  MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, i, ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);
	}

	// Configure step 7 to trigger interrupt, and be the end of sequence
//	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);
	 */
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);

	MAP_ADCSequenceEnable(ADC0_BASE, 3);

	// Enable interrupts when sample conversion complete
	MAP_ADCIntEnable(ADC0_BASE, 3);

	// Enable NVIC interrupt for ADC0 SS0
	MAP_IntEnable(INT_ADC0SS3);

	MAP_IntMasterEnable();

	MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

void ADC0_SampleHandler()
{
	MAP_ADCIntClear(ADC0_BASE, 3);

	MAP_ADCSequenceDataGet(ADC0_BASE, 3, adc_value);

	inputData[inputIndex++] = (float) adc_value[0];

	if (inputIndex > TEST_LENGTH_SAMPLES - 1)
	{
		inputIndex = 0;

		// 'Permanently disable interrupt'
		// We will only run the fft once for now
		MAP_ADCIntDisable(ADC0_BASE, 3);
		MAP_TimerDisable(TIMER1_BASE, TIMER_A);

		runFFT();

		// Delay for 1 second
		MAP_SysCtlDelay(g_ui32SysClock / 3);

		//Re-enable timer and ADC interrupts to run another FFT
		MAP_ADCIntEnable(ADC0_BASE, 3);
		MAP_TimerEnable(TIMER1_BASE, TIMER_A);

	}

}

void runFFT()
{
	int32_t startTime, stopTime, totalTime;
	float32_t maxValue;

	// Setup timer
	// This is just used for timing during test!!
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
//	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock); // 1 second

	// Create a RFFT instance
	arm_rfft_fast_instance_f32 fft;
	arm_rfft_fast_init_f32(&fft,fftSize);

	// Run FFT
//	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
//	startTime = TIMER0_TAR_R;

	/* Process the data through the CFFT/CIFFT module */
	//arm_cfft_f32(&arm_cfft_sR_f32_len256, inputComplex, ifftFlag, doBitReverse);

	/* Process the real data through the RFFT module */
	arm_rfft_fast_f32(&fft, inputData, rfftOutput, ifftFlag);

	/* Process the data through the Complex Magnitude Module for
	  calculating the magnitude at each bin */
	arm_cmplx_mag_f32(rfftOutput, testOutput_44khz, fftSize / 2);

	// The 0 index of FFT output is the DC component of
	// the input signal. We don't want to consider this when
	// looking for the peak frequency.
	testOutput_44khz[0] = 0;

	/* Calculates maxValue and returns corresponding BIN value */
	arm_max_f32(testOutput_44khz, TEST_LENGTH_SAMPLES / 2, &maxValue, &testIndex);

//	stopTime = TIMER0_TAR_R;

//	totalTime = startTime - stopTime;

//	int totalTimeUs = totalTime / 120;
	//int peakFrequency = testIndex * 22050 / 128;
	int peakFrequency = testIndex * SAMPLING_RATE / TEST_LENGTH_SAMPLES;

	MAP_SysCtlDelay(1);


}

/** \endlink */
