/*
 *  Funciones de control para el generador virtual
 *  Versión para IMX RT1060
 *
 *  Modificado: 19 abr. 2021
 *      Author: Luis Medina ;)
 */

#ifndef CONTROL_H_
#define CONTROL_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "DfnGlb.h"
#include "math.h"
#include "ProMed.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GPIObib.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
/* Código para diseñar filtros de bloqueo de banda 2º orden
 * con discretización con Tustin+prewarping
 * y[k] = a1*y[k-1]-a2*y[k-2]+b(*u[k]+2*u[k-1]+u[k-2])
import numpy as np
fs = 200            # Frecuencia de muestreo alto nivel
wN = 2*np.pi*fs/2   # Frecuencia de Nyquist
wf = 1.27           # Frecuencia de filtro Bessel 2°
z = 0.87            # Amortiguamiento filtro Bessel 2°
wB = 0.7*wN         # Ancho de banda del filtro
wa = wN             # Frecuencia de prewarping
h = 1/10000         # Paso de integración bajo nivel
eta = np.tan(wa*h/2)
pol = wa**2 + 2*z*wf*wB*wa*eta + wf**2*wB**2*eta**2
a1 = 2*(wa**2-wf**2*wB**2*eta**2)/pol
a2 = (wa**2 - 2*z*wf*wB*wa*eta + wf**2*wB**2*eta**2)/pol
b = wf**2*wB**2*eta**2/pol
print("a1 = %2.7f" % a1)
print("a2 = %2.7f" % a2)
print("b  = %2.7f" % b)
 */
/* Código para diseñar filtros de bloqueo de banda 2º orden
 * con discretización con Tustin+prewarping
 * y[k] = a*y[k-1]-b*y[k-2]+c*u[k]-a*u[k-1]+c*u[k-2]
import numpy as np
f = 60
A = np.tan(np.pi*f/10000)
B = 0.06
print(f"a={-2*(A**2-1)/(A**2+2*A*B+1)}")
print(f"b={(A**2-2*A*B+1)/(A**2+2*A*B+1)}")
print(f"c={(A**2+1)/(A**2+2*A*B+1)}")
 */
// Control de tensión y corriente // -------------------------------------------
// Variables en el marco síncrono ----------
struct VarMarcoSincrono{
	float vCd;
	float vCq;
	float iFd;
	float iFq;
	float iGd;
	float iGq;
};
struct VarMarcoRed{
	float vCd;
	float vCq;
	float vGd;
	float vGq;
	float iGd;
	float iGq;
};

// Variables para protección ---------------
struct VarProt{
	float iFmag;
	float lim_iF;
};

// Secuencia de sincronización -------------
#define TSINC 	2500	// Tiempo de sincronización [250ms] en muestras de 10kHz
#define PLL_kp	0.35f	// Ganancia proporcional del PLL
#define PLL_kih 1e-3f	// Ganancia integral del PLL

// Control de corriente --------------------
struct CI_VariablesControl{
	float iFdr;
	float iFqr;
	float aId;
	float aIq;
};
//#define satM_VI	1.3f	// Ya no se usa, el límite lo impone la batería
#define CI_kp	0.4340f		// kp = 0.4340
#define CI_kih 	0.028935f	// ki = 289.35, h = 1e-4
#define CI_htr 	0.066666f	// h/tr Cte. de tiempo de seguimiento tr = Ti = kp/ki, h=1e-4
#define param_lF 0.049087f	// Valor del inductor del filtro en pu

// Control de tensión ----------------------
struct CV_VariablesControl{
	float vCdr;
	float vCqr;
	float aId;
	float aIq;
};
#define CV_kp	3.125f		// kp=0.125
#define CV_kih	0.4315f		// ki=4315, h=1e-4
#define CV_htr	0.3452f		// Cte. de tiempo de seguimiento tr = 10Ti = 10kp/ki, h=1e-4
#define param_cF 0.031269f	// Valor del capacitor del filtro en pu

// -----------------------------------------------------------------------------

// Batería (BMS) // ------------------------------------------------------------
// Variables de estado de la batería
struct BAT_BMS{
	//float icd;
	float vcd;	// Esta variable es para ajustar el ciclo útil
	float soc;	// Mejor solo usar SOC y proteger iFdq
};
// -----------------------------------------------------------------------------

// Generador síncrono virtual // -----------------------------------------------
// Estado de operación (en sicnronización o emulando)
enum ESTADO{Sincronizacion, Emulando};
// Estructura de variables de estado
struct VSG_VariablesEstado{
	float w;		// Velocidad angular
	float w_prev;
	float xw;		// Velocidad angular filtrada
	float xw_prev;
	float pm;		// Potencia mecánica
	float pm_prev;
	float xe;		// Tensión generada (integrador de PI de reactivos)
	enum ESTADO estado;
};
// Estructura de variables y parámetros de entrada
struct VSG_Entradas{
	float pe;		// Potencia eléctrica
	float pref;		// Potencia activa de referencia
	float qe;		// Potencia reactiva
	float qref;		// Potencia reactiva de referencia
	float wref;		// Velocidad angular de referencia
	float vpcc;		// Tensión en el punto de interconexión
	float H;		// Constante de inercia
	float D;		// Coef. de amortiguamiento
	float kR;		// Ganancia de estatismo
	float Sn;		// Potencia relativa del modelo
};
// Estructura de variables para filtro Bessel 2do orden
struct FLTR_BESSEL2{
	// Salidas
	float y;	// y[k]
	float y1;	// y[k-1]
	float y2;	// y[k-2]
	// Entradas
	float u1;	// u[k-1]
	float u2;	// u[k-2]
};
/*// Estructura de variables para filtro de potencia
struct PQ_FLTR{
	// Potencia pe filtrada
	float pX;		// pf[k]
	float pX_1;		// pf[k-1]
	float pX_2;		// pf[k-2]
	float pU_1;		// pe'[k-1]
	float pU_2;		// pe'[k-2]
	// Potencia qe filtrada
	float qX;		// qf[k]
	float qX_1;		// qf[k-1]
	float qX_2;		// qf[k-2]
	float qU_1;		// qe'[k-1]
	float qU_2;		// qe'[k-2]
};*/
// Definición para FreeRTOS
#define TsMG pdMS_TO_TICKS(5U) // Tiempo de muestreo del modelo (5ms)
// Definiciones para lazo de control de potencia reactiva
#define kpQ  0.0088f	// Ganancia P del regulador de reactivos
#define kiQh 0.008f		// Ganancia I del regulador de reacitivos*h
// Definiciones para lazo de control de potencia activa
#define dos_h	400.0f	// 2/h = 400
#define cs_pm	0.95f	// Coef. pm[k-1] [(1-h/tr)pu]
#define ce_pm	0.05f	// Coef. ent. pm [(h/tr)pu]
#define lim_pm	1.0f	// pm_max [1.0pu base]
#define BndMt	0.0005f // Banda Muerta de w [30mHz/60Hz]
#define Vmin 	0.8f	// Voltaje mín. para Kpm [0.8]
#define cs_xw	0.995f	// Coef. xw[k-1] [(1-h/tw)pu]
#define ce_xw	0.005f	// Coef. ent. xw [(h/tw)pu]
//#define max_xe	0.95*bateria.vcd	// xe_max depende de v_Bat
#define min_xe	0.85f	// xe_min
// Definiciones para filtro anti-aliasing
#define A1_FAA	1.9043769f	// Ts=100us
#define A2_FAA	0.9073521f	// Ts=100us
#define B_FAA	0.0007438f	// Ts=100us
// Definiciones para filtrado de sec(-) de potencia (en lazo de tensión)
//#define Af_fltr2 1.985345f // Ts=100us
//#define Bf_fltr2 0.991001f // Ts=100us
//#define Cf_fltr2 0.995501f // Ts=100us
// -----------------------------------------------------------------------------


/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void LazoCorriente(void);
void LazoTension(void);
void ModeloGenerador(void*);
void Acondicionamiento(void);
void Reinicia_Control(void);
void Sincroniza(void);

#endif /* CONTROL_H_ */
