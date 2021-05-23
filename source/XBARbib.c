/*
 * 	Funciones de Crossbar XBAR
 * 	Archivo creado para IMX RT1060
 *
 *  Created on: 20 abr. 2021
 *      Author: Luis Medina ;)
 */

#include "XBARbib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Configura XBAR para los periféricos requeridos -----------
void XBAR_Config(void){
	// Inicializa el reloj para XBAR
    XBARA_Init(XBARA1);
    // Configura conexión de disparo de hardware de PWM1 con disparo de ADC_ETC
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputFlexpwm1Pwm1OutTrig01, kXBARA1_OutputAdcEtcXbar0Trig0);
    // Si se usara un comparador para detección de fallas para PWM, se configuraría aquí la conexión
    // Como no se usará la detección de fallas, se mantiene el pin en bajo
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicLow, kXBARA1_OutputFlexpwm1Fault0);
    //XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputAcmp2Out, kXBARA1_OutputFlexpwm1Fault0);
}
