/*
 *  Biblioteca de funciones para ADC y ADC_ETC
 *  Creada para IMX RT1060
 *
 *  Created on: 20 abr. 2021
 *      Author: Luis Medina ;)
 */

#ifndef ADCBIB_H_
#define ADCBIB_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "board.h"

// FreeRTOS
#include "FreeRTOS.h"

// Librerías propias
#include "DfnGlb.h"
#include "Control.h"
#include "ProMed.h"
#include "GPIObib.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
// Canales de muestreo para formas de onda de tensión y corriente
#define Canal_vCa 15U	// ADC1
#define Canal_vCb  0U	// ADC1
#define Canal_vCc  9U	// ADC1
#define Canal_iFa 12U	// ADC2
#define Canal_iFb  6U	// ADC2
#define Canal_iFc  5U	// ADC2
#define Canal_iGa  8U	// ADC1
#define Canal_iGb  7U	// ADC2
#define Canal_iGc 11U	// ADC1
#define Canal_extra 12U	// ADC2

// Grupos de control
#define Grupo_vCa  0U	// ADC1
#define Grupo_vCb  1U	// ADC1
#define Grupo_vCc  2U	// ADC1
#define Grupo_iFa  0U	// ADC2
#define Grupo_iFb  1U	// ADC2
#define Grupo_iFc  2U	// ADC2
#define Grupo_iGa  3U	// ADC1
#define Grupo_iGb  3U	// ADC2
#define Grupo_iGc  4U	// ADC1
#define Grupo_extra 4U	// ADC2

// Manejador de interrupciones
#define ManejadorIRQadc_etc	ADC_ETC_IRQ0_IRQHandler

// Offset de mediciones
#define OFFSET_VC 2048
#define OFFSET_IF 2048
#define OFFSET_IG 2048


/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void ADC_Config(void);
void ADC_ETC_Config(void);

#endif /* ADCBIB_H_ */
