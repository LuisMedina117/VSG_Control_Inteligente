#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

// Librerías de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"
//#include "queue.h"
//#include "event_groups.h"

// Bibliotecas propias
#include "GPIObib.h"
#include "PWMbib.h"
#include "XBARbib.h"
#include "ADCbib.h"
#include "FreeMasterbib.h"
#include "freemaster_cfg.h"

/*******************************************************************************
 * Definiciones
 ******************************************************************************/
// Prioridades de tareas
#define PRIO_ComFM 			(configMAX_PRIORITIES - 1)
#define PRIO_CtrlIntel 		(configMAX_PRIORITIES - 2)
#define PRIO_ModeloVSG 		(configMAX_PRIORITIES - 3)
#define PRIO_LED 			(configMAX_PRIORITIES - 4)

#define PERIODO_LEDvida pdMS_TO_TICKS(500)	// Periodo de parpadeo del LED
#define PERIODO_COMMS 	pdMS_TO_TICKS(1)	// Periodo de comunicación con FreeMaster

/*******************************************************************************
 * Prototipos
 ******************************************************************************/
static void ParpadeoLED(void*);
static void ComFreeMaster(void*);
extern void ModeloGenerador(void*);
extern void ControladorInteligente(void*);

static void TareaTemporal(void*);//######################################################################################

/*******************************************************************************
 * Variables Globales
 ******************************************************************************/
// Semáforos para tareas de control inteligente y emulación
extern SemaphoreHandle_t semaforo_VSG;
extern SemaphoreHandle_t semaforo_AltoNivel;
// Estructura de estado del generador
extern struct VSG_VariablesEstado vsg_ve;

TaskHandle_t Handle_TareaTemp;//#########################################################################################

/*******************************************************************************
 * Código
 ******************************************************************************/
int main(void) {

    // Inicialización del microcontrolador
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_InitBootPins();

    // Inicialización de FreeMaster (Monitoreo)
    FreeMaster_Config();

    // Inicialización de periféricos
    GPIO_Config();
    PWM_Config();
    XBAR_Config();
    ADC_Config();
    ADC_ETC_Config();

    // Crea los semáforos
    semaforo_VSG = xSemaphoreCreateBinary();
    semaforo_AltoNivel = xSemaphoreCreateBinary();

    // Crea las tareas
	xTaskCreate(ParpadeoLED, "Parpadeo LED", configMINIMAL_STACK_SIZE, NULL, PRIO_LED,  NULL);
	xTaskCreate(ComFreeMaster, "Com. FreeMaster", configMINIMAL_STACK_SIZE+100U, NULL, PRIO_ComFM, NULL);
	xTaskCreate(ModeloGenerador, "Modelo VSG", configMINIMAL_STACK_SIZE, NULL, PRIO_ModeloVSG, NULL);
	xTaskCreate(ControladorInteligente, "Ctrl Inteligente", configMINIMAL_STACK_SIZE+100U, NULL, PRIO_CtrlIntel, NULL);

/**/
	if(vsg_ve.estado == Emulando){//#####################################################################################
		xTaskCreate(TareaTemporal, "Inicio", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES,  &Handle_TareaTemp);
	}
/**/
    // Inicia el SO
	vTaskStartScheduler();

    // Ciclo infinito
    while(1) {
        __asm volatile ("nop");
    }
    return 0;
}

// ---------- Tarea temporal de inicilo -----------
static void TareaTemporal(void* param){//################################################################################
	// Si se salta la sincronización para hacer pruebas de desarrollo,
	// habilita el semáforo de control de alto nivel
	if(vsg_ve.estado == Emulando){
		xSemaphoreGive(semaforo_AltoNivel);
	}
	// Elimina la tarea temporal
	vTaskDelete(Handle_TareaTemp);
}

// ---------- Tarea para parpadeo de LED -----------
static void ParpadeoLED(void* param){
	TickType_t tiempo_ej_ant;
	// Inicializa tiempo de ejecución anterior
	tiempo_ej_ant = xTaskGetTickCount();
	// Entra al ciclo infinito de la tarea
	for(;;){
		ConmutaLED();
		vTaskDelayUntil(&tiempo_ej_ant, PERIODO_LEDvida);
	}
}

// ---------- Tarea para comunicación con FreeMaster -----------
static void ComFreeMaster(void* param){
	TickType_t tiempo_ej_ant;
	// Inicializa tiempo de ejecución anterior
	tiempo_ej_ant = xTaskGetTickCount();
	// Entra al ciclo infinito de la tarea
	for(;;){
		FMSTR_Poll();
		FMSTR_Recorder(0);
		vTaskDelayUntil(&tiempo_ej_ant, PERIODO_COMMS);
	}
}
