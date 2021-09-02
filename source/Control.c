/*
 *	Biblioteca de funciones de control para el inversor
 *	Versión para IMX RT1060
 *
 *  Modificado: 1 sep. 2021
 *      Author: Luis Medina ;)
 */

#include "Control.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
// Semáforos para tareas de control inteligente y emulación
SemaphoreHandle_t semaforo_VSG;
SemaphoreHandle_t semaforo_AltoNivel;

// Vector de tensión generada: mag [0,32768] fase [0-3600]
extern struct VectorSinc vIr;

// Vector interno de referencia
extern struct VectorRef vref_vsg;

// Estructura de variables de protección
struct VarProt var_prot = {
	.iFmag	=	0.0f,
	.lim_iF	=	2.0f
};
// Estructura de variables en el marco síncrono
struct VarMarcoSincrono var_dq = {
	.vCd	=	0.0f,
	.vCq	=	0.0f,
	.iFd	=	0.0f,
	.iFq	=	0.0f,
	.iGd	=	0.0f,
	.iGq	=	0.0f,
};
// Estructura de variables de la batería
struct BAT_BMS bateria = {
	.vcd	=	1.3f,
	.soc	=	0.5f
};
// Estructura de variables de control de corriente
struct CI_VariablesControl lazo_I = {
	.iFdr	=	0.0f,
	.iFqr	=	0.0f,
	.aId	=	0.0f,
	.aIq	=	0.0f
};
// Estructura de variables de control de tensión
struct CV_VariablesControl lazo_V = {
	.vCdr	=	1.0f,
	.vCqr	=	0.0f,
	.aId	=	0.0f,
	.aIq	=	0.0f
};
// Estructura de variables de estado del generador
struct VSG_VariablesEstado vsg_ve = {
	.w			=	1.0f,	// Velocidad angular inicial, 1pu
	.w_prev 	=	1.0f,
	.xw 		=	1.0f,	// Velocidad angular filtrada inicial, 1pu
	.xw_prev 	=	1.0f,
	.pm 		=	0,		// Potencia mecánica inicial, 0pu
	.pm_prev	=	0,
	.xe			=	1.0f,	// Tensión inicial, 1pu
	.estado = Emulando // Inicializa en modo de sincronización
};
// Estructura de entradas y parámetros del generador
struct VSG_Entradas vsg_en = {
	.pe		=	0.0f,	// Potencia eléctrica, 0pu
	.pref	=	0.0f,	// Potencia P de ref., 0pu
	.qe		=	0.0f,	// Potencia reactiva, 0pu
	.qref	=	0.0f,	// Potencia Q de ref., 0pu
	.qri	=	0.0f,	// Potencia Q de ref. (c.i.), 0pu
	.wref	=	1.0f,	// Velocidad angular de ref., 0pu
	.vpcc	=	1.0f,	// Vpcc, 1pu
	.H		=	6.0f,	// Constante de inercia, 6pu
	.D		=	15.0f,	// Coef. de amortiguamiento, 15pu
	.kR		=	20.0f,	// Ganancia de estatismo, 20pu
	.Sn		=	1.0f	// Potencia relativa del modelo 1.0
};
// Estructuras de variables con filtro anti-aliasing
// Magnitud de corriente del inversor
struct FLTR_BESSEL2 iF_faa = {
	.y = 0.0f,
	.y1 = 0.0f,
	.y2 = 0.0f,
	.u1 = 0.0f,
	.u2 = 0.0f
};
// Magnitud de tensión del inversor
struct FLTR_BESSEL2 vI_faa = {
	.y = 0.0f,
	.y1 = 0.0f,
	.y2 = 0.0f,
	.u1 = 0.0f,
	.u2 = 0.0f
};
// Magnitud de tensión de salida
struct FLTR_BESSEL2 vC_faa = {
	.y = 0.0f,
	.y1 = 0.0f,
	.y2 = 0.0f,
	.u1 = 0.0f,
	.u2 = 0.0f
};
// Potencia activa
struct FLTR_BESSEL2 pe_faa = {
	.y = 0.0f,
	.y1 = 0.0f,
	.y2 = 0.0f,
	.u1 = 0.0f,
	.u2 = 0.0f
};
// Potencia reactiva
struct FLTR_BESSEL2 qe_faa = {
	.y = 0.0f,
	.y1 = 0.0f,
	.y2 = 0.0f,
	.u1 = 0.0f,
	.u2 = 0.0f,
};
// Control Inteligente
extern const VariableLing_3 vl_soc;
extern const VariableLing_3 vl_wf;
extern const VariableLing_3 vl_xe;
extern const VariableLing_3 vl_iF;
extern const VariableLing_3 vl_vI;
extern const VariableLing_3 vl_wd;
extern const VariableLing_3 vl_Ta;
extern const VariableLing_3 vl_xp;
extern const coef_cons_3x3 coef_pE[3];
extern const coef_cons_3x3x3 coef_pP[1];
extern const coef_cons_3x3x3 coef_pR[4];
extern const struct acondicionamiento_salidas salidas_pE[3];
extern const struct acondicionamiento_salidas salidas_pP[1];
extern const struct acondicionamiento_salidas salidas_pR[4];

/*******************************************************************************
 * Código
 ******************************************************************************/
// -------------------- Lazo de control de corriente ---------------------
// Nota: si se observa más oscilación de la esperada, probablemente por el retraso de tiempo,
// se puede reducir el retraso desplazando el disparo del ADC del inicio de la rampa de PWM
// a la mitad de la rampa. Así el tiempo que pasa desde el muestreo hasta el cambio en el ciclo
// útil se reduce a la mitad.
void LazoCorriente(void){
	float temp, errorId, errorIq;
	float vIdr, vIqr;

	// Ponte pin GPIO en alto (para medición de tiempo de ejecución)
	//GPIO_PinWrite(GPIOmed1, PINmed1, 1U);

	// Controlador eje d -----------------------------------------
	errorId = lazo_I.iFdr - var_dq.iFd;
	vIdr = CI_kp*errorId + lazo_I.aId;
	// Términos de prealimentación (feedforward) y desacoplamiento
	vIdr += var_dq.vCd - var_dq.iFq*param_lF*vsg_ve.w;

	// Controlador eje q -----------------------------------------
	errorIq = lazo_I.iFqr - var_dq.iFq;
	vIqr = CI_kp*errorIq + lazo_I.aIq;
	// Términos de prealimentación (feedforward) y desacoplamiento
	vIqr += var_dq.vCq + var_dq.iFd*param_lF*vsg_ve.w;
/*
	// Verifica saturación ---------------------------------------
	temp = sqrtf(vIdr*vIdr + vIqr*vIqr);
	if (temp >= bateria.vcd)
		vIr.magnitud = bateria.vcd*32768.0f;
	else
		vIr.magnitud = temp*32768.0f;
	// Calcula magnitud relativa para backtracking
	temp = (vIr.magnitud/32768.0f)/temp;

	// Actualiza acción integral con backtracking ----------------
	lazo_I.aId = lazo_I.aId + CI_kih*errorId + CI_htr*( temp*vIdr - vIdr );
	lazo_I.aIq = lazo_I.aIq + CI_kih*errorIq + CI_htr*( temp*vIqr - vIqr );

	// Corrige magnitud de tensión -------------------------------
	// Corrige magnitud de la base del VSG a la base actual de la batería (voltaje actual vcd)
	// {en teoría siempre se tiene vcd_puVSG > 1.0_puVSG, o sea v_Bat>480sqrt(2)}
	vIr.magnitud = vIr.magnitud/bateria.vcd;
	if(vIr.magnitud >= 32768U)	// Esto junto con la instrucción anterior asegura magnitud < 1.0pu de vcd (para evitar sobremodulación)
		vIr.magnitud = 32767U;
	// {Antes de esta instrucción, el val. máx. de vIr.mag puede ser 32768, por lo que esto solo cambia el 32768 a 32767}

	// Calcula ángulo de fase ------------------------------------
	temp = atan2f(vIqr,vIdr)*572.9577f;
	if (temp >= 300) // Le resta 30º para compensar que genera tensión de fase pero mide de línea
		vIr.fase = temp-300.0f;
	else
		vIr.fase = temp+3300.0f;
*/
	// Ponte pin GPIO en bajo (para medición de tiempo de ejecución)
	//GPIO_PinWrite(GPIOmed1, PINmed1, 0U);
}

// -------------------- Lazo de control de tensión ---------------------
/* y=(kp+ki/s)u -> y[k] = y[k-1] +(-kp+ki*h/2)u[k-1] +(kp+ki*h/2)u[k] */
void LazoTension(){
	float errorVd, errorVq;
	float iFdr, iFqr;
	float temp1, temp2;

	// Ponte pin GPIO en alto (para medición de tiempo de ejecución)
//	GPIO_PinWrite(GPIOmed2, PINmed2, 1U);

	// Transforma las variables medidas al marco de referencia síncrono
	calculaDQ();

	// Controlador eje d -----------------------------------------
	errorVd = lazo_V.vCdr - var_dq.vCd;
	iFdr = CV_kp*errorVd + lazo_V.aId;
	// Términos de prealimentación (feedforward) y desacoplamiento
	iFdr += var_dq.iGd - var_dq.vCq*param_cF*vsg_ve.w;

	// Controlador eje q -----------------------------------------
	errorVq = lazo_V.vCqr - var_dq.vCq;
	iFqr = CV_kp*errorVq + lazo_V.aIq;
	// Términos de prealimentación (feedforward) y desacoplamiento
	iFqr += var_dq.iGq + var_dq.vCd*param_cF*vsg_ve.w;
/*
	// Verifica saturación ---------------------------------------
	temp1 = sqrtf(iFdr*iFdr + iFqr*iFqr);
	if (temp1 >= var_prot.lim_iF)
		temp2 = var_prot.lim_iF;
	else
		temp2 = temp1;
	// Reduce proporcionalmente las componentes
	lazo_I.iFdr = iFdr*temp2/temp1;
	lazo_I.iFqr = iFqr*temp2/temp1;
*/
	// Actualiza acción integral con backtracking ----------------
	lazo_V.aId = lazo_V.aId + CV_kih*errorVd + CV_htr*( lazo_I.iFdr - iFdr );
	lazo_V.aIq = lazo_V.aIq + CV_kih*errorVq + CV_htr*( lazo_I.iFqr - iFqr );

	/*// Cálculo y filtrado (sec-) de potencia (para modelo VSG)
	temp1 = var_dq.vCd*var_dq.iFd+var_dq.vCq*var_dq.iFq;
	pq_fltr.pX = Af_fltr2*pq_fltr.pX_1 -Bf_fltr2*pq_fltr.pX_2
				+Cf_fltr2*(temp1+pq_fltr.pU_2) -Af_fltr2*pq_fltr.pU_1;
	temp2 = var_dq.vCq*var_dq.iFd-var_dq.vCd*var_dq.iFq;
	pq_fltr.qX = Af_fltr2*pq_fltr.qX_1 -Bf_fltr2*pq_fltr.qX_2
			+Cf_fltr2*(temp2+pq_fltr.qU_2) -Af_fltr2*pq_fltr.qU_1;
	pq_fltr.pX_2 = pq_fltr.pX_1;
	pq_fltr.pX_1 = pq_fltr.pX;
	pq_fltr.pU_2 = pq_fltr.pU_1;
	pq_fltr.pU_1 = temp1;
	pq_fltr.qX_2 = pq_fltr.qX_1;
	pq_fltr.qX_1 = pq_fltr.qX;
	pq_fltr.qU_2 = pq_fltr.qU_1;
	pq_fltr.qU_1 = temp2;*/

	// Pone pin GPIO en bajo (para medición de tiempo de ejecución)
//	GPIO_PinWrite(GPIOmed2, PINmed2, 0U);
}

// -------------------- Función de acondicionamiento --------------------
// Esta función calcula las entradas para el modelo VSG e implementa filtro anti-aliasing
// y[k] = a1*y[k-1]-a2*y[k-2]+b(*u[k]+2*u[k-1]+u[k-2])
void Acondicionamiento(void){
	float temp;

	// Potencia activa --- pe = vCd*iGd + vCq*iGq
	temp = var_dq.vCd*var_dq.iGd +var_dq.vCq*var_dq.iGq;
	pe_faa.y = A1_FAA*pe_faa.y1 -A2_FAA*pe_faa.y2 +B_FAA*( temp +2*pe_faa.u1 +pe_faa.u2);
	pe_faa.y2 = pe_faa.y1;
	pe_faa.y1 = pe_faa.y;
	pe_faa.u2 = pe_faa.u1;
	pe_faa.u1 = temp;

	// Potencia reactiva --- qe = vCq*iGd - vCd*iGq
	temp = var_dq.vCq*var_dq.iGd -var_dq.vCd*var_dq.iGq;
	qe_faa.y = A1_FAA*qe_faa.y1 -A2_FAA*qe_faa.y2 +B_FAA*( temp +2*qe_faa.u1 +qe_faa.u2);
	qe_faa.y2 = qe_faa.y1;
	qe_faa.y1 = qe_faa.y;
	qe_faa.u2 = qe_faa.u1;
	qe_faa.u1 = temp;

	// Corriente del inversor (mag. relativa) --- iF' = sqrt(iFd^2 + iFq^2)/lim_IF
	temp = var_prot.iFmag/var_prot.lim_iF;
	iF_faa.y = A1_FAA*iF_faa.y1 -A2_FAA*iF_faa.y2 +B_FAA*( temp +2*iF_faa.u1 +iF_faa.u2);
	iF_faa.y2 = iF_faa.y1;
	iF_faa.y1 = iF_faa.y;
	iF_faa.u2 = iF_faa.u1;
	iF_faa.u1 = temp;

	// Tensión del inversor (mag. relativa) --- vI' = vIr/vBat
	temp = vIr.magnitud/32768.0f; //Este valor ya es vIr/vBat
	vI_faa.y = A1_FAA*vI_faa.y1 -A2_FAA*vI_faa.y2 +B_FAA*( temp +2*vI_faa.u1 +vI_faa.u2);
	vI_faa.y2 = vI_faa.y1;
	vI_faa.y1 = vI_faa.y;
	vI_faa.u2 = vI_faa.u1;
	vI_faa.u1 = temp;

	// Tensión de salida --- vC = sqrt(vCd^2 + vCq^2)
	temp = sqrtf(var_dq.vCd*var_dq.vCd +var_dq.vCq*var_dq.vCq);
	vC_faa.y = A1_FAA*vC_faa.y1 -A2_FAA*vC_faa.y2 +B_FAA*( temp +2*vC_faa.u1 +vC_faa.u2);
	vC_faa.y2 = vC_faa.y1;
	vC_faa.y1 = vC_faa.y;
	vC_faa.u2 = vC_faa.u1;
	vC_faa.u1 = temp;

}

// -------------------- Control de bajo nivel --------------------
// Esta función llama a las funciones de control de bajo nivel cuando el inversor
// se encuentra sincronizado (o sea, cuando el estado actual es 'Emulando'), de
// otra forma, ejecuta la seucencia de sincronización cuando el inersor no está
// en modo de espera
void ControlBajoNivel(void){
	// Ponte pin GPIO en alto (para medición de tiempo de ejecución)
	GPIO_PinWrite(GPIOmed1, PINmed1, 1U);
	if(vsg_ve.estado == Emulando){
		LazoTension();
		LazoCorriente();
		ActualizaCU();
		Acondicionamiento();
		ProteccionCorriente();
	}
	else if(vsg_ve.estado == Sincronizacion){
		Sincroniza();
	}
	// Ponte pin GPIO en bajo (para medición de tiempo de ejecución)
	GPIO_PinWrite(GPIOmed1, PINmed1, 0U);
}

// -------------------- Lazo de emulación del generador virtual --------------------
// La tarea de control inteligente se debe evaluar primero, esta puede estar esperando
// un semáforo binario que se active cuando termine el control inteligente
void ModeloGenerador(void* param){
	float temp1, temp2;

	// Entra al ciclo infinito de la tarea
	while(1){
		if(xSemaphoreTake(semaforo_VSG, portMAX_DELAY) == pdTRUE){

			// Ponte pin GPIO en alto (para medición de tiempo de ejecución)
			//GPIO_PinWrite(GPIOmed2, PINmed2, 1U);

		// Ecuación de oscilación ------------------------
			// Ta = (Kpm*pm-pe)/(w*Sn), Kpm = vpcc^2 if(vpcc<0.8pu) else 1.0pu
			if(vsg_en.vpcc<Vmin)
				temp1 = ((vsg_en.vpcc*vsg_en.vpcc)*vsg_ve.pm_prev-vsg_en.pe)/(vsg_ve.w_prev*vsg_en.Sn);
			else
				temp1 = (vsg_ve.pm_prev-vsg_en.pe)/(vsg_ve.w_prev*vsg_en.Sn);
			// Td = D(w-wf)
			temp2 = vsg_en.D*(vsg_ve.w_prev-vsg_ve.xw_prev);
			// w = wprev + (Ta-Td)/(2/h*H)
			//vsg_ve.w = vsg_ve.w_prev + (temp1-temp2)/(dos_h*vsg_en.H);

		// Control de velocidad --------------------------
			// errw = BM(wref-w)
			temp1 = (vsg_en.wref-vsg_ve.w_prev);
			if(temp1>0){
				if(temp1<BndMt)
					temp1 = 0;
				else
					temp1 -= BndMt;
			}else{
				if(temp1>-BndMt)
					temp1 = 0;
				else
					temp1 += BndMt;
			}
			vsg_ve.pm = cs_pm*vsg_ve.pm_prev+ce_pm*(vsg_en.pref+vsg_en.kR*temp1);
			if(vsg_ve.pm > lim_pm)
				vsg_ve.pm = lim_pm;
			else if(vsg_ve.pm < -lim_pm)
				vsg_ve.pm = -lim_pm;

		// Filtrado para amortiguamiento -----------------
			// xw = (1-h/tw)*xwprev+(h/tw)*wprev
			vsg_ve.xw = cs_xw*vsg_ve.xw_prev+ce_xw*vsg_ve.w_prev;

		// Actualización de potencia pe y qe y vpcc ------------
			vsg_en.vpcc = vC_faa.y;
			vsg_en.pe = pe_faa.y;
			vsg_en.qe = qe_faa.y;

		// Lazo de reactivos -----------------------------
			// errq = qref-qe
			temp1 = vsg_en.qref+vsg_en.qri-vsg_en.qe;
			lazo_V.vCdr = kpQ*temp1 + vsg_ve.xe;
			// Verifica límites de Ea
			if(lazo_V.vCdr > 0.95*bateria.vcd)
				lazo_V.vCdr = 0.95*bateria.vcd;
			else if(lazo_V.vCdr < min_xe)
				lazo_V.vCdr = min_xe;
			lazo_V.vCqr = 0;
			// Integración condicional
			vsg_ve.xe = vsg_ve.xe + kiQh*temp1;
			if(vsg_ve.xe > 0.95*bateria.vcd)
				vsg_ve.xe = 0.95*bateria.vcd;
			else if(vsg_ve.xe < min_xe)
				vsg_ve.xe = min_xe;

		// Actualización de variables de estado ----------
			vsg_ve.w_prev = vsg_ve.w;
			vsg_ve.xw_prev = vsg_ve.xw;
			vsg_ve.pm_prev = vsg_ve.pm;
			//vsg_ve.xe se actualiza sobre sí mismo

			// Ponte pin GPIO en bajo (para medición de tiempo de ejecución)
			//GPIO_PinWrite(GPIOmed2, PINmed2, 0U);
		}
	}
}

// -------------------- Tarea del controlador inteligente --------------------
void ControladorInteligente(void* param){
	float wd, Ta;
    float politicaE[3], politicaP[1], politicaR[4];
	TickType_t tiempo_ej_ant;

	// Inicializa tiempo de ejecución anterior
	tiempo_ej_ant = xTaskGetTickCount();

    // Este ciclo infinito externo se queda siempre a la espera del semáforo
    // que habilita la ejecución del control de alto nivel. Cuando se obtiene
    // el semáforo, la tarea permanece en continua ejecución periódica dentro
    // del ciclo infinito interno. Si el modo cambia a 'Espera', sale del
    // ciclo interno de ejecución y se queda en el ciclo externo esperando
    // que se vuelva a habilitar el semáforo
    while(1){
    	// Espera al semáforo para habilitar la ejecución
    	if(xSemaphoreTake(semaforo_AltoNivel, portMAX_DELAY) == pdTRUE){
    		while(1){
    			// Aborta ejecución si el estado actual no es 'Emulando'
    			if(vsg_ve.estado != Emulando)
    				break;

    			// Ponte pin GPIO en alto (para medición de tiempo de ejecución)
				GPIO_PinWrite(GPIOmed2, PINmed2, 1U);

				// Política de Energía: [Pref, kR, xe] = f(soc,wf)
				ANFIS_3x3(bateria.soc, vsg_ve.xw, vl_soc, vl_wf, coef_pE, 3, politicaE);
				AcondicionaSalida(politicaE, salidas_pE, 3);
				// Política de Potencia: [xp] = f(xe,iF,vI)
				ANFIS_3x3x3(politicaE[2], iF_faa.y, vI_faa.y, vl_xe, vl_iF, vl_vI, coef_pP, 1, politicaP);
				AcondicionaSalida(politicaP, salidas_pP, 1);
				// Política de Respuesta Dinámica
				wd = vsg_ve.w-vsg_ve.xw;
				Ta = (vsg_ve.pm-vsg_en.pe)/vsg_ve.w;	// Ta en base nominal
				ANFIS_3x3x3(wd, Ta, politicaP[0], vl_wd, vl_Ta, vl_xp, coef_pR, 4, politicaR);
				AcondicionaSalida(politicaR, salidas_pR, 4);
				// Nuevos parámetros para el generador
				vsg_en.pref = politicaE[0];
				vsg_en.kR   = politicaE[1];
				vsg_en.H    = politicaR[0];
				vsg_en.D    = politicaR[1];
				vsg_en.Sn   = politicaR[2];
				vsg_en.qri  = politicaR[3];

				// Ponte pin GPIO en bajo (para medición de tiempo de ejecución)
				GPIO_PinWrite(GPIOmed2, PINmed2, 0U);

				//Habilita semáforo para ejecución del modelo del generador
				xSemaphoreGive(semaforo_VSG);

				// Programa próxima ejecución en el siguiente periodo de muestreo
				vTaskDelayUntil(&tiempo_ej_ant, TsMG);
    		}
    	}
    }
}

// -------------------- Función de suspensión/reinicio --------------------
// Esta función pone el generador en modo de espera si se encuentra emulando
// o reanuda (reinicia) el control si se encuentra en modo de espera
void Suspende_Control(void){
	if (vsg_ve.estado == Emulando){
		// Variables del lazo de control de corriente
		lazo_I.iFdr	= 0.0f;
		lazo_I.iFqr	= 0.0f;
		lazo_I.aId	= 0.0f;
		lazo_I.aIq	= 0.0f;
		// Variables del lazo de control de tensión
		lazo_V.vCdr	= 1.0f;
		lazo_V.vCqr	= 0.0f;
		lazo_V.aId	= 0.0f;
		lazo_V.aIq	= 0.0f;
		// Variables de estado del generador
		vsg_ve.w		= 1.0f;
		vsg_ve.w_prev 	= 1.0f;
		vsg_ve.xw 		= 1.0f;
		vsg_ve.xw_prev 	= 1.0f;
		vsg_ve.pm 		= 0.0f;
		vsg_ve.pm_prev	= 0.0f;
		vsg_ve.xe		= 1.0f;
		vsg_ve.estado = Espera;
		// Entradas y parámetros del generador
		vsg_en.pe	= 0.0f;
		vsg_en.pref	= 0.0f;
		vsg_en.qe	= 0.0f;
		vsg_en.qref	= 0.0f;
		vsg_en.qri	= 0.0f;
		vsg_en.wref	= 1.0f;
		vsg_en.vpcc = 1.0f;
		vsg_en.H	= 6.0f;
		vsg_en.D	= 15.0f;
		vsg_en.kR	= 20.0f;
		vsg_en.Sn	= 1.0f;
		// Variables de filtrado de alto nivel
		pe_faa.y2 = 0.0f;
		pe_faa.y1 = 0.0f;
		pe_faa.y  = 0.0f;
		pe_faa.u2 = 0.0f;
		pe_faa.u1 = 0.0f;
		qe_faa.y2 = 0.0f;
		qe_faa.y1 = 0.0f;
		qe_faa.y  = 0.0f;
		qe_faa.u2 = 0.0f;
		qe_faa.u1 = 0.0f;
		iF_faa.y2 = 0.0f;
		iF_faa.y1 = 0.0f;
		iF_faa.y  = 0.0f;
		iF_faa.u2 = 0.0f;
		iF_faa.u1 = 0.0f;
		vI_faa.y2 = 0.0f;
		vI_faa.y1 = 0.0f;
		vI_faa.y  = 0.0f;
		vI_faa.u2 = 0.0f;
		vI_faa.u1 = 0.0f;
		vC_faa.y2 = 0.0f;
		vC_faa.y1 = 0.0f;
		vC_faa.y  = 0.0f;
		vC_faa.u2 = 0.0f;
		vC_faa.u1 = 0.0f;
		// Abre temporalmente interruptor
		GPIO_PinWrite(GPIOintc, PINintc, 0U);
	}
	else if(vsg_ve.estado == Espera){
		vsg_ve.estado = Sincronizacion;
	}
}

// -------------------- Función de sincronización --------------------
void Sincroniza(void){
	static int16_t contador = 0;
	static float w_in = 1.0f, e_in = 1.0f;
	float theta;
	BaseType_t xHigherPriorityTaskWoken;

	// Cálculo de magnitud promedio del vector de tensión
	e_in = (PLL_Tf*e_in) + (1.0f-PLL_Tf)*sqrtf(var_dq.vCd*var_dq.vCd+var_dq.vCq*var_dq.vCq);

	// Función de PLL
	theta = atan2f(var_dq.vCq, var_dq.vCd);
	vsg_ve.w = w_in + PLL_kp*theta;
	w_in += PLL_kih*theta;

	// Evalua si se cumple el lapso de sincronización
	contador += 1;
	if(contador >= TSINC){
		// Inicializa frecuencia
		vsg_ve.w = w_in;
		vsg_ve.w_prev = w_in;
		// Inicializa tensión
		vsg_ve.xe = e_in;
		// Cambia a estado de emulación
		vsg_ve.estado = Emulando;
		// Habilita control de alto nivel
		xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(semaforo_AltoNivel, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // Si presenta problemas, mover estas líneas al final
		// Cierra interruptor
		GPIO_PinWrite(GPIOintc, PINintc, 1U);
		// Reinicia variables
		w_in = 1.0f;
		e_in = 1.0f;
		contador = 0;
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
}

// -------------------- Función de protección de corriente --------------------
// Esta función ajusta el límite dinámico de corriente del inversor
void ProteccionCorriente(void){
	static uint8_t cont_div = 0U;
	static uint16_t cont_sc = 0U, cont_ret = 0U;
	static bool recuperacion = false;
	cont_div++;
	// Divide la frecuencia entre 100 (10kHz/100 = 100Hz)
	if(cont_div > 100U){
		cont_div = 0U;

		// Si la corriente supera la nominal, comienza conteo de sobrecarga
		// y reinicia conteo de retraso para recuperación
		if(var_prot.iFmag > 1.02f){ // Tolerancia de 2% de Inom
			cont_sc++;
			if(cont_ret>0U)
				cont_ret = 0U;
		}

		// Si iF<=Inom y el contador de sobrecarga > 0:
		else if(cont_sc > 0U){
			// Reduce el contador de sobrecarga la cumplir el retraso
			if(cont_ret >= Psc_Tret){
				cont_sc--;
				// Cambia a modo de recuperación
				if(!recuperacion)
					recuperacion = true;
			}
			// Incrementa el conteo de retraso
			else
				cont_ret++;
		}

		// Si estaba activo el modo de recuperación y se reduce el conteo
		// de cobrecarga a cero, cambia a modo normal
		if(recuperacion && cont_sc <= 0U)
			recuperacion = false;

		// Curva de sobrecarga
		var_prot.iFmag = sqrtf(var_dq.iFd*var_dq.iFd + var_dq.iFq*var_dq.iFq);
		// Si está en periodo de recuperación, el límite es el nominal
		if(recuperacion == true)
			var_prot.lim_iF = 1.0f;
		// Si se usa la sobrecarga, ajusta el límite dinámico
		else if(cont_sc > 0U){
			if(cont_sc <= Psc_T200)
				var_prot.lim_iF = 2.0f;
			else if(cont_sc <= Psc_T150)
				var_prot.lim_iF = 2.0f  - 0.5f*((float)(cont_sc-Psc_T200))/((float)(Psc_T150-Psc_T200));
			else if(cont_sc <= Psc_T120)
				var_prot.lim_iF = 1.5f  - 0.3f*((float)(cont_sc-Psc_T150))/((float)(Psc_T120-Psc_T150));
			// Si se termina el tiempo de sobrecarga, cambia a modo de recuperación
			else{
				var_prot.lim_iF = 1.0f;
				recuperacion = true;
			}
		}
	}
}
