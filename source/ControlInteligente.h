/*
 *  Biblioteca de funciones para control inteligente
 *  Creada para IMX RT1060
 *
 *  Created on: 28 aug 2021
 *      Author: Luis Medina ;)
 */

#ifndef CONTROLINTELIGENTE_H_
#define CONTROLINTELIGENTE_H_

/*******************************************************************************
 * Librerías
 ******************************************************************************/

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
// Estructura de función de membresía
// - Para funciones tipo Triangular, el soporte es [a,c] y el núcleo es b sin importar
//   el valor de 'Lado'
// - Para funciones tipo 'Borde' el soporte es [a,+inf] y la transición ocurre en [a,b]
//   si el lado es 'Derecho', o el soporte es [-inf,c] con transición en [b,c] si el
//   lado es 'Izquierdo'
enum Tipo {Triangular, Borde};
enum Lado {Izquierdo, Derecho};
struct FuncionMembresia{
	float a;
	float b;
	float c;
    enum Tipo tipo;
	enum Lado lado;
};
// Tipo de variable para variables lingüísticas de 3 térmminos
typedef struct FuncionMembresia VariableLing_3[3];
// Tipo de variables para variables difusificadas
typedef float vardif_3[3];
// Tipos de variable para fuerzas de activación
typedef float fdisparo_3x3[3][3];
typedef float fdisparo_3x3x3[3][3][3];
// Estructura de coeficientes para consecuente
struct Coeficientes_2var{
    // f(x,y) = ax + by + s
    float a;
    float b;
    float s;
};
struct Coeficientes_3var{
    // f(x,y,z) = ax + by + cz + s
    float a;
    float b;
    float c;
    float s;
};
// Tipos de variable para consecuentes de reglas de 3x3 y 3x3x3
typedef struct Coeficientes_2var coef_cons_3x3[9];
typedef struct Coeficientes_3var coef_cons_3x3x3[27];
// Estructura para acondicionamiento de salidas
struct acondicionamiento_salidas{
    // y = max*tanh( x/escala ) + sesgo
    float max;
    float escala;
    float sesgo;
};
/*******************************************************************************
 * Prototipos
 ******************************************************************************/
void Difusificar(const VariableLing_3, float, vardif_3);
void Antecedentes_3x3(vardif_3, vardif_3, fdisparo_3x3);
void Antecedentes_3x3x3(vardif_3, vardif_3, vardif_3, fdisparo_3x3x3);
float Consecuentes_3x3(float, float, fdisparo_3x3, const coef_cons_3x3);
float Consecuentes_3x3x3(float, float, float, fdisparo_3x3x3, const coef_cons_3x3x3);
void ANFIS_3x3(float, float, const VariableLing_3, const VariableLing_3, const coef_cons_3x3*, int, float*);
void ANFIS_3x3x3(float, float, float, const VariableLing_3, const VariableLing_3, const VariableLing_3, const coef_cons_3x3x3*, int, float*);
void AcondicionaSalida(float*, const struct acondicionamiento_salidas*, int);
float Tanh_aprx(float);


#endif /* CONTROLINTELIGENTE_H_ */
