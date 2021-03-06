/*
 *  Funciones para control inteligente
 *  Archivo creado para IMX RT1060
 *
 *  Created on: 29 aug 2021
 *      Author: Luis Medina ;)
 */

#include "ControlInteligente.h"
#include "stdio.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Código
 ******************************************************************************/
// -------- Capa 1. Difusificación ---------------------------------------------
// Difusifica la entrada 'x' evaluado en la función de membresía
// de los términos lingüísticos de la variable. Esta función asume
// que el número de términos es 3.
void Difusificar(const VariableLing_3 variable, float x, vardif_3 resultado){
    int i;
    //for(i=0; i<variable.num_terminos; i++){
    for(i=0; i<3; i++){
        // Función de membresía triangular
        if(variable[i].tipo == Triangular){
            if(x<variable[i].a)
                resultado[i] = 0;
            else if(variable[i].a<=x && x<variable[i].b)
                resultado[i] = (x-variable[i].a)/(variable[i].b-variable[i].a);
            else if(variable[i].b<=x && x<variable[i].c)
                resultado[i] = (variable[i].c-x)/(variable[i].c-variable[i].b);
            else //x>c
                resultado[i] = 0;
        }
        // Función de membresía de borde
        else{
            if(variable[i].lado == Derecho){
                if(x<variable[i].a) //x<a
                    resultado[i] = 0;
                else if(variable[i].a<=x && x<variable[i].b) //a<=x<b
                    resultado[i] = (x-variable[i].a)/(variable[i].b-variable[i].a);
                else //x>=b
                    resultado[i] = 1;
            }
            else{ //lado==Izquierdo
                if(x<variable[i].b) //x<b
                    resultado[i] = 1;
                else if(variable[i].b<=x && x<variable[i].c) //b<=x<c
                    resultado[i] = (variable[i].c-x)/(variable[i].c-variable[i].b);
                else //x>c
                    resultado[i] = 0;
            }
        }
    }
}

// -------- Capas 2 y 3. Fuerza de disparo -------------------------------------
// Esta función calcula los antecedentes normalizados para
// una base de reglas de 3x3.
void Antecedentes_3x3(vardif_3 x1, vardif_3 x2, fdisparo_3x3 resultado){
    int i,j;
    float acumulador = 0;
    // Evalua antecedente de cada regla
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            resultado[i][j] = x1[i]*x2[j];
            acumulador += resultado[i][j];
        }
    }
    // Normaliza el resultado y obtiene fuerza de disparo
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(resultado[i][j] != 0)
                resultado[i][j] = resultado[i][j]/acumulador;
        }
    }
}
// Esta función calcula los antecedentes normalizados para
// una base de reglas de 3x3x3.
void Antecedentes_3x3x3(vardif_3 x1, vardif_3 x2, vardif_3 x3, fdisparo_3x3x3 resultado){
    int i,j,k;
    float acumulador = 0;
    // Evalua antecedente de cada regla
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            for(k=0;k<3;k++){
                resultado[i][j][k] = x1[i]*x2[j]*x3[k];
                acumulador += resultado[i][j][k];
            }
        }
    }
    // Normaliza el resultado y obtiene fuerza de disparo
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            for(k=0;k<3;k++){
                if(resultado[i][j][k] != 0)
                    resultado[i][j][k] = resultado[i][j][k]/acumulador;
            }
        }
    }
}

// -------- Capa 4 y 5. Consecuentes y reducción -------------------------------
// Esta función calcula los consecuentes de cada regla y obtiene
// el resultado final de la inferencia. (Caso de 2 variables con
// 3 términos lingüísticos cada una)
float Consecuentes_3x3(float x1, float x2, fdisparo_3x3 antecedente, const coef_cons_3x3 coeficientes){
    int i,j,m;
    float resultado = 0;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(antecedente[i][j] > 0.01){
                m = i*3+j;
                resultado += (coeficientes[m].a*x1 + coeficientes[m].b*x2 + coeficientes[m].s)*antecedente[i][j];
            }
        }
    }
    return resultado;
}
// Esta función calcula los consecuentes de cada regla y obtiene
// el resultado final de la inferencia. (Caso de 3 variables con
// 3 términos lingüísticos cada una)
float Consecuentes_3x3x3(float x1, float x2, float x3, fdisparo_3x3x3 antecedente, const coef_cons_3x3x3 coeficientes){
    int i,j,k,m;
    float resultado = 0;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            for(k=0;k<3;k++){
                if(antecedente[i][j][k] > 0.01){
                    m = i*9+j*3+k;
                    resultado += (coeficientes[m].a*x1 + coeficientes[m].b*x2 + coeficientes[m].c*x3 + coeficientes[m].s)*antecedente[i][j][k];
                }
            }
        }
    }
    return resultado;
}

// -------- Red ANFIS ----------------------------------------------------------
// Esta función implementa una red ANFIS para el caso con 2
// variables de entrada con 3 términos lingüísticos cada una
void ANFIS_3x3(float x1, float x2, const VariableLing_3 variable1, const VariableLing_3 variable2, const coef_cons_3x3* coeficientes, int num_salidas, float* resultado){
    // El número de politicaE debe coincidir con el número de
    // arreglos de coeficientes y el número de elementos del
    // arreglo de resultado
    int i;
    vardif_3 x1_dif, x2_dif;
    fdisparo_3x3 disparo_reglas;
    // Capa 1
    Difusificar(variable1, x1, x1_dif);
    Difusificar(variable2, x2, x2_dif);
    // Capas 2 y 3
    Antecedentes_3x3(x1_dif, x2_dif, disparo_reglas);
    // Capas 4 y 5
    for(i=0;i<num_salidas;i++){
        resultado[i] = Consecuentes_3x3(x1, x2, disparo_reglas, coeficientes[i]);
    }
}
// Esta función implementa una red ANFIS para el caso con 3
// variables de entrada con 3 términos lingüísticos cada una
void ANFIS_3x3x3(float x1, float x2, float x3, const VariableLing_3 variable1, const VariableLing_3 variable2, const VariableLing_3 variable3, const coef_cons_3x3x3* coeficientes, int num_salidas, float* resultado){
    // El número de politicaE debe coincidir con el número de
    // arreglos de coeficientes y el número de elementos del
    // arreglo de resultado
    int i;
    vardif_3 x1_dif, x2_dif, x3_dif;
    fdisparo_3x3x3 disparo_reglas;
    // Capa 1
    Difusificar(variable1, x1, x1_dif);
    Difusificar(variable2, x2, x2_dif);
    Difusificar(variable3, x3, x3_dif);
    // Capas 2 y 3
    Antecedentes_3x3x3(x1_dif, x2_dif, x3_dif, disparo_reglas);
    // Capas 4 y 5
    for(i=0;i<num_salidas;i++){
        resultado[i] = Consecuentes_3x3x3(x1, x2, x3, disparo_reglas, coeficientes[i]);
    }
}

// -------- Acondicionamiento de salidas ---------------------------------------
// Función para acondicionar salidas con el ajuste de escala, imposición de
// límites con la función tanh y adición del sesgo
// y = max*tanh( x/escala ) + sesgo
void AcondicionaSalida(float* salida, const struct acondicionamiento_salidas* condiciones, int num_salidas){
    int i;
    for (i=0;i<num_salidas;i++){
        salida[i] = condiciones[i].max*Tanh_aprx(salida[i]/condiciones[i].escala) + condiciones[i].sesgo;
    }
};

// -------- Aproximación de la función tanh ------------------------------------
// Aproximación de la función tanh mediante 9 segmentos con
// funciones cuadráticas. El error absoluto de la aproximación
// es inferior a 0.0003
float Tanh_aprx(float x){
    float s, y;
    if(x>0)
        s = x;
    else
        s = -x;
    if(x<0.25f) //[0,0.25)
        y = -0.12135f*s*s + 1.01189f*s -0.00024f;
    else if(x<0.55f) //[0.25,0.55)
        y = -0.32178f*s*s + 1.11084f*s -0.01291f;
    else if(x<1.0f) //[0.55,1.00)
        y = -0.37136f*s*s + 1.15485f*s -0.02215f;
    else if(x<1.35f) //[1.0,1.35)
        y = -0.26267f*s*s + 0.93729f*s +0.08720f;
    else if(x<1.75f) //[1.35,1.75)
        y = -0.15165f*s*s + 0.63714f*s +0.29056f;
    else if(x<2.25f) //[1.75,2.25)
        y = -0.06909f*s*s + 0.34862f*s +0.54315f;
    else if(x<2.95f) //[2.25,2.95)
        y = -0.02231f*s*s + 0.13892f*s +0.77867f;
    else if(x<2.95f) //[2.95,4.40)
        y = -0.00298f*s*s + 0.02504f*s +0.94693f;
    else
        y = 1.0f;
    if(x>0)
        return y;
    else
        return -y;
}
