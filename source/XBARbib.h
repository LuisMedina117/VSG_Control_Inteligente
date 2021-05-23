/*
 * 	Biblioteca de funciones de Crossbar XBAR
 * 	Creada para IMX RT1060
 *
 *  Created on: 20 abr. 2021
 *      Author: Luis Medina ;)
 */

#ifndef XBARBIB_H_
#define XBARBIB_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "fsl_common.h"
#include "fsl_xbara.h"
#include "board.h"

// FreeRTOS
#include "FreeRTOS.h"

// Librerías propias
#include "DfnGlb.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void XBAR_Config(void);

#endif /* XBARBIB_H_ */
