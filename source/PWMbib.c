/*
 *  Funciones y variables para configurar PWM
 *  Versión para IMX RT1060
 *
 *  Created on: 18 abr. 2021
 *      Author: Luis Medina ;)
 */

#include "PWMbib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint16_t ModuloPWM;
extern struct MOD_IDX m_vsg;

/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Configura PWM -----------
void PWM_Config(void){
	// Estructura de configuración
    pwm_config_t PWMconfig;
    // Variables para configuración de contador
    uint16_t Tmuerto;

    // Habilita reloj para PWM
    CCM->CCGR4 = (CCM->CCGR4 & ~(CCM_CCGR4_CG8_MASK)) | CCM_CCGR4_CG8(0x3);

    // Genera la configuración para el módulo maestro (módulo 0)
    PWM_GetDefaultConfig(&PWMconfig);
    PWMconfig.pairOperation = kPWM_ComplementaryPwmA;		// Modo complementario
    PWMconfig.reloadLogic = kPWM_ReloadPwmFullCycle;		// Recarga de registros en cada ciclo

    // Inicializa módulo maestro (módulo 0)
    PWM_Init(PWM1, kPWM_Module_0, &PWMconfig);

    // Modifica configuración para módulos 1 y 3
    PWMconfig.clockSource = kPWM_Submodule0Clock;			// Mismo reloj que módulo maestro
    PWMconfig.reloadSelect = kPWM_MasterReload;				// Conteo sincronizado con el maestro
    PWMconfig.initializationControl = kPWM_Initialize_MasterSync;	// Inicialización sincronizada

    // Inicializa módulos 1 y 3
    PWM_Init(PWM1, kPWM_Module_1, &PWMconfig);
    PWM_Init(PWM1, kPWM_Module_3, &PWMconfig);

    // Habilita disparo por hardware en el módulo 0 con VAL4 para sincronización de ADC
    PWM1->SM[0U].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1U << 4U);

    // Inicializa valores de módulo y tiempo muerto
    ModuloPWM = CLOCK_GetFreq(kCLOCK_IpgClk) / FRECUENCIA_PWM;
    Tmuerto = ((TIEMPO_MUERTO * (CLOCK_GetFreq(kCLOCK_IpgClk) / 1000000U)) / 1000U);

    // Valor inicial del contador
    PWM1->SM[0U].INIT = PWM_INIT_INIT((uint16_t)(-(ModuloPWM/2U)));
    PWM1->SM[1U].INIT = PWM_INIT_INIT((uint16_t)(-(ModuloPWM/2U)));
    PWM1->SM[3U].INIT = PWM_INIT_INIT((uint16_t)(-(ModuloPWM/2U)));

    // Valor cero del cotador
    PWM1->SM[0U].VAL0 = PWM_VAL0_VAL0(0U);
    PWM1->SM[1U].VAL0 = PWM_VAL0_VAL0(0U);
    PWM1->SM[3U].VAL0 = PWM_VAL0_VAL0(0U);

    // Valor final del contador
    PWM1->SM[0U].VAL1 = PWM_VAL1_VAL1((uint16_t)((ModuloPWM/2U) - 1U));
    PWM1->SM[1U].VAL1 = PWM_VAL1_VAL1((uint16_t)((ModuloPWM/2U) - 1U));
    PWM1->SM[3U].VAL1 = PWM_VAL1_VAL1((uint16_t)((ModuloPWM/2U) - 1U));

    // Ciclo útil inicial (0)
    PWM1->SM[0U].VAL2 = PWM_VAL2_VAL2(0U);
    PWM1->SM[1U].VAL2 = PWM_VAL2_VAL2(0U);
    PWM1->SM[3U].VAL2 = PWM_VAL2_VAL2(0U);
    PWM1->SM[0U].VAL3 = PWM_VAL3_VAL3(6000U);
    PWM1->SM[1U].VAL3 = PWM_VAL3_VAL3(6000U);
    PWM1->SM[3U].VAL3 = PWM_VAL3_VAL3(6000U);

    // Valor de referencia para disparos por hardware
    // El ADC se dispara cuando el contador se reinicia, se incluye un
    // retraso para muestreo efectivo de corriente cuando CU~100% como
    // recomendación de notas de aplicación de NXP
    PWM1->SM[0U].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(ModuloPWM/2U)+10U)));
    PWM1->SM[1U].VAL4 = PWM_VAL4_VAL4(0U);
    PWM1->SM[3U].VAL4 = PWM_VAL4_VAL4(0U);
    PWM1->SM[0U].VAL5 = PWM_VAL5_VAL5(0U);
    PWM1->SM[1U].VAL5 = PWM_VAL5_VAL5(0U);
    PWM1->SM[3U].VAL5 = PWM_VAL5_VAL5(0U);

    // Configuración del tiempo muerto
    // Flanco de subida
    PWM1->SM[0U].DTCNT0 = PWM_DTCNT0_DTCNT0(Tmuerto);
    PWM1->SM[1U].DTCNT0 = PWM_DTCNT0_DTCNT0(Tmuerto);
    PWM1->SM[3U].DTCNT0 = PWM_DTCNT0_DTCNT0(Tmuerto);
    // Flanco de bajada
    PWM1->SM[0U].DTCNT1 = PWM_DTCNT1_DTCNT1(Tmuerto);
    PWM1->SM[1U].DTCNT1 = PWM_DTCNT1_DTCNT1(Tmuerto);
    PWM1->SM[3U].DTCNT1 = PWM_DTCNT1_DTCNT1(Tmuerto);

    // Configuración de detección de fallas
    // Canales A y B se desactivan cuando ocurre la falla 0
    PWM1->SM[0U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(1U));
    PWM1->SM[1U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(1U));
    PWM1->SM[3U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(1U));
    PWM1->SM[0U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(1U));
    PWM1->SM[1U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(1U));
    PWM1->SM[3U].DISMAP[0U] = ((PWM1->SM[0U].DISMAP[0U] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(1U));
    // La falla 0 se activa con lógica alta (1) y se limpia la bandera en automático
    PWM1->FCTRL = (PWM1->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(1U);
    PWM1->FCTRL = (PWM1->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(1U);
    // Limpia las banderas de falla
    PWM1->FSTS = (PWM1->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);
    // Los PWM se rehabilitan al ciclo siguiente de eliminar la falla
    PWM1->FSTS = (PWM1->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(1U);

    // Habilita salidas PWM
    PWM1->OUTEN = (PWM1->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK)
					| PWM_OUTEN_PWMA_EN(kPWM_Control_Module_0|kPWM_Control_Module_1|kPWM_Control_Module_3);
    PWM1->OUTEN = (PWM1->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK)
					| PWM_OUTEN_PWMB_EN(kPWM_Control_Module_0|kPWM_Control_Module_1|kPWM_Control_Module_3);

    // Inicia conteo
    PWM1->MCTRL = (PWM1->MCTRL & ~PWM_MCTRL_CLDOK_MASK)
					| PWM_MCTRL_CLDOK(kPWM_Control_Module_0|kPWM_Control_Module_1|kPWM_Control_Module_3);
    PWM1->MCTRL = (PWM1->MCTRL & ~PWM_MCTRL_LDOK_MASK)
					| PWM_MCTRL_LDOK(kPWM_Control_Module_0|kPWM_Control_Module_1|kPWM_Control_Module_3);
    PWM1->MCTRL = (PWM1->MCTRL & ~PWM_MCTRL_RUN_MASK)
					| PWM_MCTRL_RUN(kPWM_Control_Module_0|kPWM_Control_Module_1|kPWM_Control_Module_3);
}

// ---------- Ajuste de ciclo útil de los módulos -----------
// El ciclo útil puede ser de 0 a 2^15
// Los pulsos se generan alineados al centro
void PWM_ActualizaCU(void){
	// Valor de comparación
	uint16_t cuenta_PWM;

	// Calcula el valor de comparación correspondiente (fase A)
	cuenta_PWM = (uint16_t)( (((uint32_t)m_vsg.faseA)*((uint32_t)ModuloPWM))/32768U );
	if(cuenta_PWM > ModuloPWM)
		cuenta_PWM = ModuloPWM;
	// Ajusta los valores de comparación de los flancos izquierdo y derecho
    PWM1->SM[0U].VAL2 = PWM_VAL2_VAL2( (uint16_t)(-(cuenta_PWM/2U)) );
    PWM1->SM[0U].VAL3 = PWM_VAL3_VAL3( (uint16_t)(cuenta_PWM/2U) );

	// Calcula el valor de comparación correspondiente (fase B)
	cuenta_PWM = (uint16_t)( (((uint32_t)m_vsg.faseB)*((uint32_t)ModuloPWM))/32768U );
	if(cuenta_PWM > ModuloPWM)
		cuenta_PWM = ModuloPWM;
	// Ajusta los valores de comparación de los flancos izquierdo y derecho
    PWM1->SM[1U].VAL2 = PWM_VAL2_VAL2( (uint16_t)(-(cuenta_PWM/2U)) );
    PWM1->SM[1U].VAL3 = PWM_VAL3_VAL3( (uint16_t)(cuenta_PWM/2U) );

	// Calcula el valor de comparación correspondiente (fase C)
	cuenta_PWM = (uint16_t)( (((uint32_t)m_vsg.faseC)*((uint32_t)ModuloPWM))/32768U );
	if(cuenta_PWM > ModuloPWM)
		cuenta_PWM = ModuloPWM;
	// Ajusta los valores de comparación de los flancos izquierdo y derecho
    PWM1->SM[3U].VAL2 = PWM_VAL2_VAL2( (uint16_t)(-(cuenta_PWM/2U)) );
    PWM1->SM[3U].VAL3 = PWM_VAL3_VAL3( (uint16_t)(cuenta_PWM/2U) );

	// Habilita bit de carga de nuevos valores
	PWM1->MCTRL |= PWM_MCTRL_LDOK_MASK;
}
