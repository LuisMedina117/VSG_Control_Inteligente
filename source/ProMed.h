/*
 *	Biblioteca de funciones de procesamiento de señales y mediciones
 *	Versión para IMX RT1060
 *
 *  Modificado: 30 abr. 2021
 *      Author: Luis Medina ;)
 */

#ifndef PROMED_H_
#define PROMED_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/
#include "stdint.h"
#include "DfnGlb.h"
#include "Control.h"
#include "PWMbib.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/

// Modulación PWM con vectores espaciales // -------------------
// Vectores síncronos ----------------------
struct VectorRef{
	float fase_rel;		// Fase relativa respecto al último sector (en radianes)
	uint8_t sector;		// Último sector recorrido por el vector
};
struct VectorSinc{
	uint16_t magnitud;	// Magnitud en pu (1pu = 2^15)
	uint16_t fase;		// Fase relativa al vector de referencia (en grados*10)
};

// Índices de modulación -------------------
struct MOD_IDX{
	uint16_t faseA;		// Ciclo útil fase A (0 a 2^15)
	uint16_t faseB;		// Ciclo útil fase B (0 a 2^15)
	uint16_t faseC;		// Ciclo útil fase C (0 a 2^15)
};

// Enumeración de sectores ----------------------
enum sectores{secI,secII,secIII,secIV,secV,secVI};

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void ActualizaCU(void);
void calculaDQ(void);
int16_t seno_LookUp(uint16_t);
float satura(float,float);
void Filtro_variables(void);

#endif /* PROMED_H_ */
