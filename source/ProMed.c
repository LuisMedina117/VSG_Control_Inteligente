/*
 * 	Funciones de procesamiento de señales y mediciones
 * 	Versión para IMX RT1060
 *
 *  Modificado: 30 abr. 2021
 *      Author: Luis Medina ;)
 */

#include "ProMed.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
// Valores medidos de tensiones y corrientes [-2048,2047]
// Tensión, rango completo adc = 1.5pu
int16_t VCa_i16, VCb_i16, VCc_i16;
// Corriente, rango completo adc = 2.5pu
int16_t IFa_i16, IFb_i16, IFc_i16, IGa_i16, IGb_i16, IGc_i16;

// Variables transformadas en el marco de referencia sícrono
extern struct VarMarcoSincrono var_dq;
// Variables de estado del generador síncrono virtual
extern struct VSG_VariablesEstado vsg_ve;

// Vectores de referencia
struct VectorRef vref_vsg = { .fase_rel = 0, .sector = 0 };	// Vector síncrono de referencia del VSG

// Índices de modulación (ciclo útil)
struct MOD_IDX m_vsg = { .faseA = 0, .faseB = 0, .faseC = 0 };
// La base es 2^15 (32768)

// Magnitud de tensión generada (32768=1.0pu, 26000~0.8pu)
struct VectorSinc vIr = { .magnitud = 26000U, .fase = 0 };

// Referencia para recorder FreeMaster
int16_t refFM = 0;

// Tabla de búsqueda de (1<<15)*sin(x), x E [0,60) pesa 1kB
extern const uint16_t tabla_sen[512];
// Tabla de búsqueda de (1<<15)*sin(x), x E [0,90)
extern const uint16_t tabla_sen90[1024];


/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Actualización de ciclo útil de tensión generada -----------
void ActualizaCU(void){
	uint8_t sector_V = 0;	//Sector de vector de tensión
	uint16_t pos_V = 0;		//Posición de vector de tensión
	uint16_t t1, t2, t0;
	uint8_t dif_sectores;

	// Calcula sector y posición respecto al vector de referencia
	dif_sectores = vIr.fase/600U;
	sector_V = vref_vsg.sector + dif_sectores;
	pos_V = (uint16_t)(vref_vsg.fase_rel*488.924f) + ( (vIr.fase-dif_sectores*600U)*64U )/75U; //Equivale a +(fase_V%600)*512/600
	if(pos_V >= 512U){
		pos_V -= 512U;
		sector_V += 1;
	}
	if(sector_V >= 6U)
		sector_V -= 6U;

	// Calcula el ciclo útil de cada fase con la magnitud correspondiente
	t1 = ( (uint32_t)(tabla_sen[511U-pos_V])*(uint32_t)(vIr.magnitud) )>>15U;	//Equivale a valor_tabla*magnitud/32768
	t2 = ( (uint32_t)(tabla_sen[pos_V])*(uint32_t)(vIr.magnitud) )>>15U;
	t0 = 32768U - (t1+t2);
	// Se probó también interpolación pero no se notó una diferencia apreciable

	switch(sector_V){
	case secI:
		m_vsg.faseA = t0/2 + t1 + t2;
		m_vsg.faseB = t0/2 + t2;
		m_vsg.faseC = t0/2;
		break;
	case secII:
		m_vsg.faseA = t0/2 + t1;
		m_vsg.faseB = t0/2 + t1 + t2;
		m_vsg.faseC = t0/2;
		break;
	case secIII:
		m_vsg.faseA = t0/2;
		m_vsg.faseB = t0/2 + t1 + t2;
		m_vsg.faseC = t0/2 + t2;
		break;
	case secIV:
		m_vsg.faseA = t0/2;
		m_vsg.faseB = t0/2 + t1;
		m_vsg.faseC = t0/2 + t1 + t2;
		break;
	case secV:
		m_vsg.faseA = t0/2 + t2;
		m_vsg.faseB = t0/2;
		m_vsg.faseC = t0/2 + t1 + t2;
		break;
	case secVI:
		m_vsg.faseA = t0/2 + t1 + t2;
		m_vsg.faseB = t0/2;
		m_vsg.faseC = t0/2 + t1;
		break;
	}

	// Calcula el nuevo ángulo de fase del vector de referencia integrando la frecuencia actual
	vref_vsg.fase_rel += 376.9911e-4f*vsg_ve.w;
	// Si supera pi/3, actualiza sector y ajusta fase relativa
	if(vref_vsg.fase_rel >= 1.047198f){
		vref_vsg.fase_rel -= 1.047198f;
		vref_vsg.sector++;
		if(vref_vsg.sector >= 6U)
			vref_vsg.sector -= 6U;
	}

	// Actualiza registros de ciclo útil para modulación PWM
	PWM_ActualizaCU();
}

// ---------- Transforma variables al marco de referencia síncrono -----------
/* xd = 2/sqrt(3) * ( sen(T+60)*xa + sen(T)*xb )
 * xq = 2/sqrt(3) * (-sen(T-30)*xa + sen(T+90)*xb )
 * */
void calculaDQ(void){
/*
	static int16_t a, b, c, d;
	// Calcula coeficientes para la transformación
	a = seno_LookUp(600);
	b = seno_LookUp(0);
	c = -seno_LookUp(3300);
	d = seno_LookUp(900);
	// Actualiza referencia para FreeMaster
	refFM = b;
	// Transforma corrientes IF
	var_dq.iFd = ( (float)a*(float)IFa_i16 + (float)b*(float)IFb_i16 )/581179e2f; // 581179e2f={[2/sqrt(3)]/[32768*2048]}^-1
	var_dq.iFq = ( (float)c*(float)IFa_i16 + (float)d*(float)IFb_i16 )/581179e2f;
	// Transforma corrientes IG
	var_dq.iGd = ( (float)a*(float)IGa_i16 + (float)b*(float)IGb_i16 )/581179e2f; // 581179e2f={[2/sqrt(3)]/[32768*2048]}^-1
	var_dq.iGq = ( (float)c*(float)IGa_i16 + (float)d*(float)IGb_i16 )/581179e2f;
	// En este caso se considera que se están midiendo tensiones de fase, si se estuvieran midiendo tensiones
	// de línea, se podría aquí adelantar 30º el marco de referencia para ajustar el defasamiento
	// Transforma tensiones VC
	var_dq.vCd = ( (float)a*(float)VCa_i16 + (float)b*(float)VCb_i16 )/581179e2f; // 581179e2f={[2/sqrt(3)]/[32768*2048]}^-1
	var_dq.vCq = ( (float)c*(float)VCa_i16 + (float)d*(float)VCb_i16 )/581179e2f;
*/
	int16_t a, b, c, d, e, f;
	a = seno_LookUp(900);	//cos(x+0)
	b = seno_LookUp(3300);	//cos(x-120)
	c = seno_LookUp(2100);	//cos(x+120)
	d = -seno_LookUp(0);	//-sen(x+0)
	e = -seno_LookUp(2400);	//-sen(x+120)
	f = -seno_LookUp(1200);	//-sen(x-120)
	// Actualiza referencia para FreeMaster
	refFM = -d;
	// Transforma corrientes iF
	var_dq.iFd = ((float)a*(float)IFa_i16 + (float)b*(float)IFb_i16 + (float)c*(float)IFc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	var_dq.iFq = ((float)d*(float)IFa_i16 + (float)e*(float)IFb_i16 + (float)f*(float)IFc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	// Transforma corrientes iG
	var_dq.iGd = ((float)a*(float)IGa_i16 + (float)b*(float)IGb_i16 + (float)c*(float)IGc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	var_dq.iGq = ((float)d*(float)IGa_i16 + (float)e*(float)IGb_i16 + (float)f*(float)IGc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	// Transforma tensiones vC
	// En este caso se considera que se están midiendo tensiones de fase, pero si se estuvieran midiendo tensiones
	// de línea, se podría adelantar aquí 30º el marco de referencia para ajustar el defasamiento
	var_dq.vCd = ((float)a*(float)VCa_i16 + (float)b*(float)VCb_i16 + (float)c*(float)VCc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	var_dq.vCq = ((float)d*(float)VCa_i16 + (float)e*(float)VCb_i16 + (float)f*(float)VCc_i16 )/100663.4e3f; // 100663.4e3f=(32768*2048)/(2/3)
	// Ajuste de ganancia de medición
	// Ganancia de tensión = 1.5 (Escala completa ADC = ±2048adc = amplitud de 1.5pu)
	// Ganancia de corriente = 2.5 (Escala completa ADC = ±2048adc = amplitud de 2.5pu)
	var_dq.iFd = var_dq.iFd*2.5f;
	var_dq.iFq = var_dq.iFq*2.5f;
	var_dq.iGd = var_dq.iGd*2.5f;
	var_dq.iGq = var_dq.iGq*2.5f;
	var_dq.vCd = var_dq.vCd*1.5f;
	var_dq.vCq = var_dq.vCq*1.5f;
}

// ---------- Calcula seno(theta+fase) -----------
// fase = ángulo respecto a referencia (theta*10): {0,1,...,3600}
int16_t seno_LookUp(uint16_t fase){
	uint16_t angulo;
	uint8_t sector;
	// Verifica que el ángulo sea menor a 360
	if(fase>3600)
		return 0;
	// Calcula sectores de adelanto
	sector = (uint8_t)(fase/600U);
	// Convierte fase de grados a unidades de desp. en tabla
	angulo = ((fase-(sector*600U))*64U)/75U; // angulo = fase_rel*512/600, fase_rel=fase respecto al sector
	// Calcula ángulo y sector
	angulo = angulo + (uint16_t)(vref_vsg.fase_rel*488.924f);
	if(angulo >= 512U){
		angulo -= 512U;
		sector += 1;
	}
	sector = sector + vref_vsg.sector;
	if(sector >= 6U)
		sector -= 6U;
	// Calcula seno con tabla de búsqueda
	switch(sector){
	case 0:
		// 0 < ángulo < 60, [0 a 511]
		return tabla_sen90[angulo];
	case 1:
		// 60 < ángulo < 90, [512 a 767]
		if(angulo < 256)
			return tabla_sen90[angulo + 512U];
		// 90 < ángulo < 120, [767 a 512]
		else
			return tabla_sen90[1023U - angulo];// [767-(ángulo-256)]
	case 2:
		// 120 < ángulo < 180, [511 a 0]
		return tabla_sen90[511U-angulo];
	case 3:
		// 180 < ángulo < 240, [0 a 511]
		return -tabla_sen90[angulo];
	case 4:
		// 240 < ángulo < 270, [512 a 767]
		if(angulo < 256)
			return -tabla_sen90[angulo + 512U];
		// 270 < ángulo < 300, [767 a 512]
		else
			return -tabla_sen90[1023U - angulo];// [767-(ángulo-256)]
	case 5:
		// 300 < ángulo < 360, [511 a 0]
		return -tabla_sen90[511U-angulo];
	}
	// No debe alzanzar esta parte
	return 0;
}

// ---------- Satura a valor seleccionado -----------
// Forza la variable a permanecer en el intervalo [-limite, limite]
float satura(float variable, float limite){
	if(variable >= limite)
		return limite;
	else if(variable <= -limite)
		return -limite;
	else
		return variable;
}

// ---------- Filtro de variables -----------
// Filtro de variables para modelo del VSG
void Filtro_variables(void){
}
