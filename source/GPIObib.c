/*
 * 	Funciones y variables para configurar GPIO
 * 	Versión para IMX RT1060
 *
 *  Created on: 17 abr. 2021
 *      Author: Luis Medina ;)
 */

#include "GPIObib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Código
 ******************************************************************************/
// ---------- Configura GPIO -----------
void GPIO_Config(void){
	// Estructuras de configuración de pines
	gpio_pin_config_t boton_config = { kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge };
    gpio_pin_config_t out_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };

	// Inicializa pines GPIO y relojes correspondientes
    GPIO_PinInit(GPIOboton, PINboton, &boton_config);
    GPIO_PinInit(GPIOled, PINled, &out_config);
    GPIO_PinInit(GPIOmed1, PINmed1, &out_config);
    GPIO_PinInit(GPIOmed2, PINmed2, &out_config);
    GPIO_PinInit(GPIOintc, PINintc, &out_config);

    // Ajusta prioridad de interrupciones
	NVIC_SetPriority(IRQboton, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2U);

    // Habilita interrupciones
    GPIO_PortEnableInterrupts(GPIOboton, 1U << PINboton);
    EnableIRQ(IRQboton);
}

// ---------- Manejador para el botón -----------
void ManejadorIRQboton(void){
	// Restablece la bandera
    GPIO_PortClearInterruptFlags(GPIOboton, 1U << PINboton);

    // Reestablece variables de estado y control y regresa a modo de sincronización
    Reinicia_Control();
    ConmutaLED();

    SDK_ISR_EXIT_BARRIER;
}

// ---------- Función para encender LED -----------
void EnciendeLED(void){
	GPIO_PinWrite(GPIOled, PINled, 1U);
}

// ---------- Función para apagar LED -----------
void ApagaLED(void){
	GPIO_PinWrite(GPIOled, PINled, 0U);
}

// ---------- Función para conmutar LED -----------
void ConmutaLED(void){
	GPIO_PortToggle(GPIOled, 1U << PINled);
}
