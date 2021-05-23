/*
 *  Funciones para ADC y ADC_ETC
 * 	Archivo creado para IMX RT1060
 *
 *  Modificado: 30 abr. 2021
 *      Author: Luis Medina
 */

#include "ADCbib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern int16_t VCa_i16, VCb_i16, VCc_i16, IFa_i16, IFb_i16, IFc_i16, IGa_i16, IGb_i16, IGc_i16;
extern struct VSG_VariablesEstado vsg_ve;

/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Configura convertidor ADC -----------
void ADC_Config(void){
	// Estructura de configuración para ADC
	adc_config_t ADCconfig;

    // Obtiene la configuración estándar
    ADC_GetDefaultConfig(&ADCconfig);
    // Desactiva reloj asíncrono
    ADCconfig.enableAsynchronousClockOutput = false;
    // Selecciona reloj de periféricos IPG (f_IPG = 600/4 = 150MHz)
    ADCconfig.clockSource = kADC_ClockSourceIPG;
    // Selecciona divisor del reloj (f_ADCK = f_IPG/4 = 37.5MHz) [Máximo nominal 40MHz en modo alta v.] // En el ejemplo del motor lo ponen a 66MHz pero a 10 bits y sin modo alta v.
    ADCconfig.clockDriver = kADC_ClockDriver4;
    // Habilita modo de alta velocidad
    ADCconfig.enableHighSpeed = true;
    // Ajusta resolución a 12 bits
    ADCconfig.resolution = kADC_Resolution12Bit;

    // Inicializa convertidores
    ADC_Init(ADC1, &ADCconfig);
    ADC_Init(ADC2, &ADCconfig);

    // Desactiva medición promedio
    ADC_SetHardwareAverageConfig(ADC1, kADC_HardwareAverageDiasable);
    ADC_SetHardwareAverageConfig(ADC2, kADC_HardwareAverageDiasable);

    // Calibra los convertidores
    ADC_DoAutoCalibration(ADC1);
    ADC_DoAutoCalibration(ADC2);

    // Habilita disparo por hardware
    ADC_EnableHardwareTrigger(ADC1, true);
    ADC_EnableHardwareTrigger(ADC2, true);

    // Habilita selección de canal por medio de ADC_ETC
    ADC1->HC[Grupo_vCa] = ADC_HC_ADCH(16U);
    ADC1->HC[Grupo_vCb] = ADC_HC_ADCH(16U);
    ADC1->HC[Grupo_vCc] = ADC_HC_ADCH(16U);
    ADC2->HC[Grupo_iFa] = ADC_HC_ADCH(16U);
    ADC2->HC[Grupo_iFb] = ADC_HC_ADCH(16U);
    ADC2->HC[Grupo_iFc] = ADC_HC_ADCH(16U);
    ADC1->HC[Grupo_iGa] = ADC_HC_ADCH(16U);
    ADC2->HC[Grupo_iGb] = ADC_HC_ADCH(16U);
    ADC1->HC[Grupo_iGc] = ADC_HC_ADCH(16U);
    ADC2->HC[Grupo_extra] = ADC_HC_ADCH(16U);
    // El último registro de control no se usa pero se necesita
    // tener un número par de registros

}

// ---------- Configura control de disparos (ETC) para ADC -----------
void ADC_ETC_Config(void){
	// Estructura de configuración para ADC_ETC
    adc_etc_config_t ADC_ETCconfig;
    // Estructura de configuración de disparos
    adc_etc_trigger_config_t Disp_config;
    // Estructura de configuración de cadena de disparos
    adc_etc_trigger_chain_config_t Cadena_config;

    // Obtiene configuración estándar
    ADC_ETC_GetDefaultConfig(&ADC_ETCconfig);
    // Habilita el disparador 0 (para sinc. con PWM)
    ADC_ETCconfig.XBARtriggerMask = 1U;
    // Se desactiva Bypass de TSC para poder usar ADC2
    ADC_ETCconfig.enableTSCBypass = false;

    // Inicializa el control de disparos
    ADC_ETC_Init(ADC_ETC, &ADC_ETCconfig);

    // Cofigura los disparos (trigger0 para ADC1 y trigger4 para ADC2)
    Disp_config.enableSyncMode = true;			// Habilita control simultáneo de ADC1 y ADC2
    Disp_config.enableSWTriggerMode = false;	// Deshabilita disparo por software
    Disp_config.triggerChainLength = 4U;		// Tamaño de cadena 5
    Disp_config.triggerPriority = 0U;			// Prioridad de disparo
    Disp_config.initialDelay = 0U;				// Sin retraso
    Disp_config.sampleIntervalDelay = 0U;		// No se usan intervalos de disparos (se usa B2B)
    // Carga la configuración
    ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &Disp_config);
	// Para el grupo 4 ya no es necesario indicar el modo sync
    Disp_config.enableSyncMode = false;
    ADC_ETC_SetTriggerConfig(ADC_ETC, 4U, &Disp_config);

    // Configura la cadena de disparos
    Cadena_config.enableB2BMode = true;		// Habilita disparos consecutivos (back to back)
    Cadena_config.InterruptEnable = kADC_ETC_InterruptDisable;	// Deshabilita interrupción
    // Canales de tensión vC
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_vCa;
    Cadena_config.ADCChannelSelect = Canal_vCa;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, Grupo_vCa, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_vCb;
    Cadena_config.ADCChannelSelect = Canal_vCb;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, Grupo_vCb, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_vCc;
    Cadena_config.ADCChannelSelect = Canal_vCc;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, Grupo_vCc, &Cadena_config);
    // Canales de corriente iF
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iFa;
    Cadena_config.ADCChannelSelect = Canal_iFa;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, Grupo_iFa, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iFb;
    Cadena_config.ADCChannelSelect = Canal_iFb;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, Grupo_iFb, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iFc;
    Cadena_config.ADCChannelSelect = Canal_iFc;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, Grupo_iFc, &Cadena_config);
    // Canales de corriente iG
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iGa;
    Cadena_config.ADCChannelSelect = Canal_iGa;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, Grupo_iGa, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iGb;
    Cadena_config.ADCChannelSelect = Canal_iGb;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, Grupo_iGb, &Cadena_config);
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_iGc;
    Cadena_config.ADCChannelSelect = Canal_iGc;
    Cadena_config.InterruptEnable = kADC_ETC_Done0InterruptEnable;	// Habilita interrupción Done0 para última señal
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, Grupo_iGc, &Cadena_config);
    // Canal extra para obtener un número par de registros
    Cadena_config.ADCHCRegisterSelect = 1U << Grupo_extra;
    Cadena_config.ADCChannelSelect = Canal_extra;
    Cadena_config.InterruptEnable = kADC_ETC_InterruptDisable;
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 4U, Grupo_extra, &Cadena_config);

    //Ajusta prioridad de interrupción
    NVIC_SetPriority(ADC_ETC_IRQ0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    // Habilita interrupción
    EnableIRQ(ADC_ETC_IRQ0_IRQn);
}

void ManejadorIRQadc_etc(void){
	// Restablece la bandera de interrupción
	ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);

	// Guarda los resultados de la conversión
	VCa_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, Grupo_vCa) - OFFSET_VC;
	VCb_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, Grupo_vCb) - OFFSET_VC;
	VCc_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, Grupo_vCc) - OFFSET_VC;
	IFa_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, Grupo_iFa) - OFFSET_IF;
	IFb_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, Grupo_iFb) - OFFSET_IF;
	IFc_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, Grupo_iFc) - OFFSET_IF;
	IGa_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, Grupo_iGa) - OFFSET_IG;
	IGb_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 4U, Grupo_iGb) - OFFSET_IG;
	IGc_i16 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, Grupo_iGc) - OFFSET_IG;

	// Habilita controles si el inversor ya está sincronizado
	if(vsg_ve.estado == Emulando){
		LazoTension();
		LazoCorriente();
		ActualizaCU();
		Acondicionamiento();
	}
	else{
		Sincroniza();
	}

	// Barrera de sincronización
    SDK_ISR_EXIT_BARRIER;
}
