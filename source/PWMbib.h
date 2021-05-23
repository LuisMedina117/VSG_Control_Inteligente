/*
 *  Biblioteca de funciones para PWM
 *  Versión para IMX RT1060
 *
 *  Created on: 18 abr. 2021
 *      Author: Luis Medina ;)
 */
#ifndef PWMBIB_H_
#define PWMBIB_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_pwm.h"
#include "pin_mux.h"
#include "board.h"

// FreeRTOS
#include "FreeRTOS.h"

// Librerías propias
#include "DfnGlb.h"
#include "ProMed.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
#define FRECUENCIA_PWM	10000U	// Frecuencia de modulación PWM [Hz]
#define TIEMPO_MUERTO	500U	// Tiempo muerto de conmutación [ns]

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void PWM_Config(void);
void PWM_ActualizaCU(void);

#endif /* PWMBIB_H_ */
