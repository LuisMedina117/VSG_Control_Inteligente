/*
 *  Biblioteca de funciones para GPIO
 *  Versión para IMX RT1060
 *
 *  Created on: 17 abr. 2021
 *      Author: Luis Medina ;)
 */
#ifndef GPIOBIB_H
#define GPIOBIB_H

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "board.h"

// FreeRTOS
#include "FreeRTOS.h"

// Librerías propias
#include "DfnGlb.h"
#include "Control.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
// Definiciones del botón de la tarjeta
#define GPIOboton 	BOARD_USER_BUTTON_GPIO
#define PINboton	BOARD_USER_BUTTON_GPIO_PIN
#define IRQboton	BOARD_USER_BUTTON_IRQ
#define ManejadorIRQboton	BOARD_USER_BUTTON_IRQ_HANDLER
// Definiciones del LED de la tarjeta
#define GPIOled		BOARD_USER_LED_GPIO
#define PINled		BOARD_USER_LED_GPIO_PIN
// Definiciones de los pines de medición de tiempo de ejecución
#define GPIOmed1	GPIO1
#define PINmed1		(2U)	// {D9}
#define GPIOmed2	GPIO1
#define PINmed2		(3U)	// {D8}
#define GPIOintc	GPIO1
#define PINintc		(0U)	// {D15}

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void GPIO_Config(void);
void EnciendeLED(void);
void ApagaLED(void);
void ConmutaLED(void);

#endif
