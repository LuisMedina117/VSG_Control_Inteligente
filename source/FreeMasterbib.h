/*
 * Biblioteca de funciones para FreeMaster
 * Versión par IMX RT1060
 *
 *  Created on: Apr 18, 2021
 *      Author: Luis Medina ;)
 */

#ifndef FREEMASTERBIB_H_
#define FREEMASTERBIB_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

#include "ProMed.h"
#include "Control.h"


/*******************************************************************************
 * Definiciones
 ******************************************************************************/
#define LPUART_FreeMaster	BOARD_DEBUG_UART_BASEADDR
#define IRQFreeMaster 		BOARD_UART_IRQ
#define ManejadorIRQlpuart	BOARD_UART_IRQ_HANDLER

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void FreeMaster_Config(void);

#endif /* FREEMASTERBIB_H_ */
