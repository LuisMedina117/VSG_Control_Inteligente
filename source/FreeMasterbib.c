/*
 *  Funciones para FreeMaster
 *  Versión para IMX RT1060
 *
 *  Modificado: 18 abr. 2021
 *      Author: Luis Medina ;)
 */

#include "FreeMasterbib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern struct MOD_IDX m_vsg;

extern int16_t refFM;

extern int16_t VCa_i16, VCb_i16, VCc_i16;
extern int16_t IFa_i16, IFb_i16, IFc_i16, IGa_i16, IGb_i16, IGc_i16;

extern struct VectorRef vref_vsg;
extern struct VectorSinc vIr;
extern struct VectorSinc iGr;

extern struct VarMarcoSincrono var_dq;
extern struct CI_VariablesControl lazo_I;
extern struct CV_VariablesControl lazo_V;
extern struct VSG_VariablesEstado vsg_ve;
extern struct VSG_Entradas vsg_en;

/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Configura FreeMaster con LPUART -----------
void FreeMaster_Config(void){
	// Estructura de configuración para LPUART
    lpuart_config_t config;

    // Inicializa el LPUART
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx = false;
    config.enableRx = false;
    LPUART_Init((LPUART_Type*)LPUART_FreeMaster, &config, BOARD_DEBUG_UART_CLK_FREQ);

    // Selecciona LPUART como módulo de comunicación para FreeMaster
    FMSTR_SerialSetBaseAddress((LPUART_Type*)LPUART_FreeMaster);

    // Habilita las interrupciones
    LPUART_EnableInterrupts((LPUART_Type*)LPUART_FreeMaster, kLPUART_RxDataRegFullInterruptEnable | kLPUART_RxOverrunInterruptEnable);
    EnableIRQ(IRQFreeMaster);
    NVIC_SetPriority(BOARD_UART_IRQ,4);

    // Inicializa FreeMaster
    FMSTR_Init();
}

// ---------- Manejador para interrupciones del LPUART -----------
void BOARD_UART_IRQ_HANDLER (void){
    // Llama al manejador de interrupciones de FreeMaster
    FMSTR_SerialIsr();
    // No hace falta limpiar la bandera de interrupción manualmente
}

// ---------- Declara tabla de variables -------------------------
// Target Side Addressing (TSA) no es necesario pero sirve para definir previamente
// cómo se deben interpretar las variables (tipo, formato, signo, etc)
FMSTR_TSA_TABLE_BEGIN(tabla_variables)

// Índice de modulación
FMSTR_TSA_STRUCT(struct MOD_IDX)
FMSTR_TSA_MEMBER(struct MOD_IDX, faseA,	FMSTR_TSA_UINT16)
FMSTR_TSA_MEMBER(struct MOD_IDX, faseB,	FMSTR_TSA_UINT16)
FMSTR_TSA_MEMBER(struct MOD_IDX, faseC,	FMSTR_TSA_UINT16)
FMSTR_TSA_RO_VAR(m_vsg, FMSTR_TSA_USERTYPE(struct MOD_IDX))

// Variable de referencia para FreeMaster
FMSTR_TSA_RO_VAR(refFM, FMSTR_TSA_SINT16)

// Formas de onda de tensión y corriente
FMSTR_TSA_RO_VAR(VCa_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(VCb_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(VCc_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IFa_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IFb_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IFc_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IGa_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IGb_i16, FMSTR_TSA_SINT16)
FMSTR_TSA_RO_VAR(IGc_i16, FMSTR_TSA_SINT16)

//Vectores síncronos para modulación SVPWM
FMSTR_TSA_STRUCT(struct VectorRef)
FMSTR_TSA_MEMBER(struct VectorRef, fase_rel,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VectorRef, sector,		FMSTR_TSA_UINT8)
FMSTR_TSA_RO_VAR(vref_vsg, FMSTR_TSA_USERTYPE(struct VectorRef))
FMSTR_TSA_STRUCT(struct VectorSinc)
FMSTR_TSA_MEMBER(struct VectorSinc, magnitud,	FMSTR_TSA_UINT16)
FMSTR_TSA_MEMBER(struct VectorSinc, fase,		FMSTR_TSA_UINT16)
FMSTR_TSA_RO_VAR(vIr, FMSTR_TSA_USERTYPE(struct VectorSinc))

// Estructura de variables en marco de ref. síncrono
FMSTR_TSA_STRUCT(struct VarMarcoSincrono)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, vCd,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, vCq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, iFd,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, iFq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, iGd,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VarMarcoSincrono, iGq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_RO_VAR(var_dq, FMSTR_TSA_USERTYPE(struct VarMarcoSincrono))

// Estructura de variables de control de corriente
FMSTR_TSA_STRUCT(struct CI_VariablesControl)
FMSTR_TSA_MEMBER(struct CI_VariablesControl, iFdr,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CI_VariablesControl, iFqr,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CI_VariablesControl, aIq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CI_VariablesControl, aIq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(lazo_I, FMSTR_TSA_USERTYPE(struct CI_VariablesControl))

// Estructura de variables de control de tensión
FMSTR_TSA_STRUCT(struct CV_VariablesControl)
FMSTR_TSA_MEMBER(struct CV_VariablesControl, vCdr,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CV_VariablesControl, vCqr,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CV_VariablesControl, aId,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct CV_VariablesControl, aIq,	FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(lazo_V, FMSTR_TSA_USERTYPE(struct CV_VariablesControl))

// Estructura de variables de estado del VSG
FMSTR_TSA_RO_VAR(vsg_ve, FMSTR_TSA_USERTYPE(struct VSG_VE))
FMSTR_TSA_STRUCT(struct VSG_VariablesEstado)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, w, 		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, w_prev,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, xw,		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, xw_prev,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, pm,		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, pm_prev,	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, xe,		FMSTR_TSA_FLOAT)
FMSTR_TSA_ENUM(enum ESTADO)
FMSTR_TSA_CONST(Sincronizacion)
FMSTR_TSA_CONST(Emulando)
FMSTR_TSA_MEMBER(struct VSG_VariablesEstado, estado, FMSTR_TSA_USERTYPE(ESTADO))

// Estructura de variables de entrada del VSG
FMSTR_TSA_RW_VAR(vsg_en, FMSTR_TSA_USERTYPE(struct VSG_EN))
FMSTR_TSA_STRUCT(struct VSG_Entradas)
FMSTR_TSA_MEMBER(struct VSG_Entradas, pe, 		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, pref, 	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, qe, 		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, qref, 	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, wref, 	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, vpcc, 	FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, H,		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, D,		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, kR,		FMSTR_TSA_FLOAT)
FMSTR_TSA_MEMBER(struct VSG_Entradas, Sn,		FMSTR_TSA_FLOAT)

FMSTR_TSA_TABLE_END()


FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(tabla_variables)
FMSTR_TSA_TABLE_LIST_END()

